#include <utils.h>

// code taken from SpecialK, credits to Kaldaien

namespace utils
{
    AutoHandle::~AutoHandle() noexcept
    {
        // we cannot close these handles because technically they
        //  were never opened (by usermode code).
        if ((intptr_t)m_h < (intptr_t)nullptr)
            m_h = nullptr;

        // signed handles are often special cases
        //  such as -2 = Current Thread, -1 = Current Process
    }

    std::queue <DWORD> SuspendAllOtherThreads()
    {
        std::queue <DWORD> threads;
        AutoHandle hSnap(CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0));

        if (hSnap != INVALID_HANDLE_VALUE)
        {
            THREADENTRY32 tent = { };
            tent.dwSize = sizeof(THREADENTRY32);

            if (Thread32First(hSnap, &tent))
            {
                do
                {
                    if (tent.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof tent.th32OwnerProcessID)
                    {
                        if (tent.th32ThreadID != GetCurrentThreadId() && tent.th32OwnerProcessID == GetCurrentProcessId())
                        {
                            AutoHandle hThread(OpenThread(THREAD_SUSPEND_RESUME, FALSE, tent.th32ThreadID));

                            if (hThread != nullptr)
                            {
                                threads.push(tent.th32ThreadID);
                                SuspendThread(hThread);
                            }
                        }
                    }

                    tent.dwSize = sizeof(tent);
                } while (Thread32Next(hSnap, &tent));
            }
        }

        return threads;
    }

    void ResumeThreads(std::queue <DWORD> threads)
    {
        while (!threads.empty())
        {
            DWORD tid = threads.front();

            AutoHandle hThread(OpenThread(THREAD_SUSPEND_RESUME, FALSE, tid));

            if (hThread != nullptr)
            {
                ResumeThread(hThread);
            }

            threads.pop();
        }
    }

    void GetConsole()
    {
        if (AttachConsole(-1) == FALSE)
        {
            AllocConsole();
        }

        FILE* file;
        freopen_s(&file, "CONIN$", "rb", stdin);
        freopen_s(&file, "CONOUT$", "wb", stdout);
        freopen_s(&file, "CONOUT$", "wb", stderr);
    }
}
