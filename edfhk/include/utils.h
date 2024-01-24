namespace utils
{
	class AutoHandle : public CHandle
	{
		// signed handles are invalid, since handles are pointers and
		//  the signed half of the address space is only for the kernel
	public:
		AutoHandle(HANDLE hHandle) noexcept : CHandle(hHandle) { }
		~AutoHandle() noexcept;
	};

	static __forceinline DWORD GetCurrentThreadId()
	{
		return reinterpret_cast<DWORD*>(NtCurrentTeb())[
#ifdef _M_AMD64
			18
#elif _M_IX86
			9
#endif
		];
	}

	std::queue<DWORD> SuspendAllOtherThreads();

	void ResumeThreads(std::queue<DWORD> threads);

	void GetConsole();
}