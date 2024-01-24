#include <dx11/dx11hk.h>
#include <utils.h>

static const wchar_t* sg_processName = L"EDF5.exe";
static const wchar_t* sg_windowTitle = L"EarthDefenceForce 5 for PC";
// static const wchar_t* sg_windowClass = L"xgs::Framework";

static DWORD thread_main(LPVOID lpParam)
{
	utils::GetConsole();

	std::cout << "EDF! EDF!" << std::endl;

	auto game_module = GetModuleHandleW(sg_processName);

	// Wait a bit for the window to be created
	//	TODO: find a better solution
	Sleep(4000);

	// Could just use GetForegroundWindow() I think or idk GetActiveWindow()
	auto hWnd = FindWindowW(nullptr, sg_windowTitle);

	if (game_module == nullptr || hWnd == nullptr)
	{
		std::cout << "Could not find game module or window handle." << std::endl;
		return 0;
	}

	DX11Hk::GetInstance().Run(game_module, hWnd);

	return 1;
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD fdwReason, LPVOID lpReserved)
{
	auto game_module = GetModuleHandle(sg_processName);
	if (game_module == nullptr)
	{
		return TRUE;
	}

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(nullptr);
		CreateThread(nullptr, 0, &thread_main, nullptr, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}

	return TRUE;
}
