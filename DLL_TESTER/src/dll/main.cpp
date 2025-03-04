#include <Windows.h>
#include <iostream>

extern "C" __declspec(dllexport) void InjectedFunction() {
    MessageBoxA(NULL, "DLL Injected Successfully!", "Injection", MB_OK);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        // When the DLL is loaded, show a message box.
        MessageBoxA(NULL, "DLL Loaded Successfully!", "Injection", MB_OK);

        // Call the function that we want to execute
        InjectedFunction();
    }
    return TRUE;
}
