#pragma once
#include "memory/memory.h"
#include <vector>
#include <fstream>


bool InjectDLL(const std::string& dllPath) {
    // Open the target process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, memoryData.processId);
    if (!hProcess) {
        std::cerr << "Failed to open target process!" << std::endl;
        return false;
    }

    // Allocate memory in the target process for the DLL path
    LPVOID allocatedMemory = VirtualAllocEx(hProcess, NULL, dllPath.size(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!allocatedMemory) {
        std::cerr << "Failed to allocate memory in target process!" << std::endl;
        CloseHandle(hProcess);
        return false;
    }

    // Write the DLL path into the allocated memory
    if (!WriteProcessMemory(hProcess, allocatedMemory, dllPath.c_str(), dllPath.size(), NULL)) {
        std::cerr << "Failed to write memory in target process!" << std::endl;
        VirtualFreeEx(hProcess, allocatedMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Get the address of LoadLibraryA function in kernel32.dll
    FARPROC loadLibraryAddr = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
    if (!loadLibraryAddr) {
        std::cerr << "Failed to get address of LoadLibraryA!" << std::endl;
        VirtualFreeEx(hProcess, allocatedMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Create a remote thread to call LoadLibraryA in the target process
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, allocatedMemory, 0, NULL);
    if (!hThread) {
        std::cerr << "Failed to create remote thread!" << std::endl;
        VirtualFreeEx(hProcess, allocatedMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Wait for the thread to complete
    WaitForSingleObject(hThread, INFINITE);

    // Clean up
    VirtualFreeEx(hProcess, allocatedMemory, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return true;
}
