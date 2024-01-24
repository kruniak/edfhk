// credits to https://github.com/gibbed
// code from https://github.com/gibbed/JC4AnticrashHook

static const char* sg_moduleName = "winmm";

extern "C"
{
	typedef void(*shim)(void);
	void __stdcall wrapper_load(void);
#define MAKE_SHIM(x,y) \
  extern shim shim_p_##x; \
  void shim_l_##x(void); \
  void shim_h_##x(void);
#include "wrapper_shims.inc"
#undef MAKE_SHIM
}

class DllWrapper
{
public:
	DllWrapper(const char* name) :
		m_name(name), m_module(nullptr)
	{
		InitializeCriticalSection(&m_criticalSection);
		reset();
	}

	~DllWrapper()
	{
		EnterCriticalSection(&m_criticalSection);
		if (m_module != nullptr)
		{
			clear();
			FreeLibrary(m_module);
			m_module = nullptr;
		}
		LeaveCriticalSection(&m_criticalSection);
		DeleteCriticalSection(&m_criticalSection);
	}

	void load(void)
	{
		EnterCriticalSection(&m_criticalSection);

		const char* missing_export_name = nullptr;

		if (m_module == nullptr)
		{
			char path[MAX_PATH];
			GetSystemDirectoryA(path, MAX_PATH);
			strcat_s(path, "\\");
			strcat_s(path, m_name.data());

			HMODULE module = LoadLibraryA(path);
			if (module == NULL)
			{
				LeaveCriticalSection(&m_criticalSection);
				crash_brk(L"Failed to load real %S. :(", m_name.data());
				return;
			}

#define MAKE_SHIM(x,y) \
      shim_p_##x = (shim)GetProcAddress(module, #x); \
      if (shim_p_##x == nullptr) \
      { \
        missing_export_name = #x; \
        goto missing_export; \
      }
#include "wrapper_shims.inc"
#undef MAKE_SHIM

			m_module = module;
		}

		LeaveCriticalSection(&m_criticalSection);
		return;

	missing_export:
		LeaveCriticalSection(&m_criticalSection);
		crash_brk(L"Export '%S' is missing from real %S!", missing_export_name, m_name.data());
	}

private:
	std::string m_name;
	HMODULE m_module;
	CRITICAL_SECTION m_criticalSection;

	void reset()
	{
#define MAKE_SHIM(x,y) shim_p_##x = (shim)&shim_l_##x;
#include "wrapper_shims.inc"
#undef MAKE_SHIM
	}

	void clear()
	{
#define MAKE_SHIM(x,y) shim_p_##x = nullptr;
#include "wrapper_shims.inc"
#undef MAKE_SHIM
	}

	void crash_brk(const wchar_t* fmt, ...)
	{
		const wchar_t* caption = L"Critical error";
		wchar_t message[1024] = { 0 };

		va_list args;
		va_start(args, fmt);
		_vsnwprintf_s(message, _countof(message), fmt, args);
		va_end(args);

		MessageBoxW(GetDesktopWindow(), message, caption, MB_OK | MB_ICONERROR);
		ExitProcess(1);
		DebugBreak();
	}
};

DllWrapper wrapper(sg_moduleName);

extern "C"
{
	void __stdcall wrapper_load()
	{
		wrapper.load();
	}
}
