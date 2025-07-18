#include <windows.h>
#include <iostream>
#include <string>

bool runKdmapper()
{
    std::wstring kdmapperPath(MAX_PATH, L'\0');
    std::wstring driverPath(MAX_PATH, L'\0');

    if (!GetFullPathNameW(L"source\\driver\\kdMapperFiles\\kdmapper.exe", MAX_PATH, &kdmapperPath[0], nullptr))
    {
        std::wcerr << L"[!] Could not get full path of kdmapper.exe\n";
        return false;
    }

    if (!GetFullPathNameW(L"source\\driver\\kdMapperFiles\\RickOwens00.sys", MAX_PATH, &driverPath[0], nullptr))
    {
        std::wcerr << L"[!] Could not get full path of driver sys file\n";
        return false;
    }

    kdmapperPath.resize(wcslen(kdmapperPath.c_str()));
    driverPath.resize(wcslen(driverPath.c_str()));

    std::wstring cmdLine = L"\"" + kdmapperPath + L"\" --free \"" + driverPath + L"\"";

    SECURITY_ATTRIBUTES sa{ sizeof(sa), nullptr, TRUE };
    HANDLE hNull = CreateFileW(L"NUL", GENERIC_WRITE, FILE_SHARE_WRITE, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hNull == INVALID_HANDLE_VALUE)
    {
        std::wcerr << L"[!] Failed to open NUL device\n";
        return false;
    }

    STARTUPINFOW si{ sizeof(si) };
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput = hNull;
    si.hStdError = hNull;

    PROCESS_INFORMATION pi{};

    BOOL success = CreateProcessW(
        nullptr,
        &cmdLine[0],
        nullptr,
        nullptr,
        TRUE,
        CREATE_NO_WINDOW,
        nullptr,
        nullptr,
        &si,
        &pi
    );

    CloseHandle(hNull);

    if (!success)
    {
        std::wcerr << L"[!] Failed to launch kdmapper. Error: " << GetLastError() << L"\n";
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode = 0;
    bool result = false;

    if (GetExitCodeProcess(pi.hProcess, &exitCode))
    {
        if (exitCode == 0)
        {
            result = true;
        }
        else
        {
            std::wcerr << L"[!] kdmapper exited with error code: " << exitCode << L"\n";
        }
    }
    else
    {
        std::wcerr << L"[!] Failed to get kdmapper exit code. Error: " << GetLastError() << L"\n";
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return result;
}
