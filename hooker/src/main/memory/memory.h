#include "engine/includes.h"
bool FindProcessName(const wchar_t* ProcessName)
{
	HANDLE ProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (ProcessSnapshot == INVALID_HANDLE_VALUE) {
		std::cout << "Sorry, but we cant open a handle" << std::endl;
	}
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	while (Process32Next(ProcessSnapshot, &pe32)) {
		if (!_wcsicmp(pe32.szExeFile, ProcessName)) {
			memoryData.processId = pe32.th32ProcessID;
			memoryData.ProcessName = ProcessName;
			memoryData.processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, memoryData.processId);
			if (memoryData.processHandle == NULL)
			{
				DWORD error = GetLastError();
				std::cout << "Total fail blyaha muha, code:" << error << std::endl;
			}
			else {
				std::wcout << pe32.szExeFile << std::endl;
				std::cout << "Process ID: " << memoryData.processId << std::endl;
				std::cout << "Process Handle: " << memoryData.processHandle << std::endl;
			}
			break;

		}
	}
	CloseHandle(ProcessSnapshot);
	return true;
}

bool GetAllModules() {
	HANDLE moduleHandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, memoryData.processId);
	if (moduleHandleSnapshot == INVALID_HANDLE_VALUE) {
		std::cout << "Error snapshoting modules" << std::endl;
	}
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);

	while (Module32Next(moduleHandleSnapshot, &me32)) {
		wchar_t* currentModule = me32.szModule;
		std::wcout << currentModule << std::endl;
	}
	CloseHandle(moduleHandleSnapshot);
	return true;
}

bool FindExactModule(const wchar_t* moduleName) {
	HANDLE moduleHandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, memoryData.processId);
	if (moduleHandleSnapshot == INVALID_HANDLE_VALUE) {
		std::cout << "Error creating snapshot" << std::endl;
		return false; 
	}

	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);

	
	while (Module32Next(moduleHandleSnapshot, &me32)) {
		if (_wcsicmp(me32.szModule, moduleName) == 0) {
			std::wcout << "Found module, name: " << me32.szModule << std::endl;
			CloseHandle(moduleHandleSnapshot);
			return true;
		}
	}

	std::cout << "Module not found" << std::endl;
	CloseHandle(moduleHandleSnapshot);
	return false;
}

uintptr_t GetDllBaseAddress(const char* dllName) {
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, memoryData.processId);
	if (hProcess == NULL) {
		std::cerr << "Could not open process!" << std::endl;
		return 0;
	}

	HMODULE hMods[1024];
	DWORD cbNeeded;
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
		for (size_t i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
			char szModName[MAX_PATH];
			if (GetModuleFileNameExA(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(char))) {
				std::cout << "Module: " << szModName << std::endl;
				if (strstr(szModName, dllName) != NULL) {
					std::cout << "Found DLL! Base Address: " << hMods[i] << std::endl;
					CloseHandle(hProcess);
					return reinterpret_cast<uintptr_t>(hMods[i]);
				}
			}
		}
	}

	std::cerr << "DLL not found!" << std::endl;
	CloseHandle(hProcess);
	return 0;
}




template <typename T>
T ReadMemory(uintptr_t address) {
	T buffer;
	ReadProcessMemory(memoryData.processHandle, (LPCVOID)address, &buffer, sizeof(T), NULL);
	return buffer;
}

template<typename T>
void WriteMemory(uintptr_t address, T value)
{
	return WriteProcessMemory(memoryData.processHandle, (LPVOID)address, &value, sizeof(T), NULL);
}

template <typename T>
void ReadMemory(uintptr_t address, T* buffer, size_t size) {
	return ReadProcessMemory(memoryData.processHandle, (LPCVOID)address, buffer, size * sizeof(T), NULL);
}

bool ReadMemory(HANDLE hProcess, uintptr_t address, void* buffer, SIZE_T size) {
	SIZE_T bytesRead;
	return ReadProcessMemory(hProcess, (LPCVOID)address, buffer, size, &bytesRead) && bytesRead == size;
}

int TestOffset(uintptr_t offset) {
	const size_t MAX_LENGTH = 256;
	char moduleName[MAX_LENGTH];
	std::cout << "Enter module name: ";
	std::cin.getline(moduleName, MAX_LENGTH);

	uintptr_t baseAddress = GetDllBaseAddress(moduleName);
	if (baseAddress == 0) {
		std::cout << "You entered dll not correctrly or it dont exist" << std::endl;
	}
	std::cout << "Module name entered: " << moduleName << std::endl;

	uintptr_t value = ReadMemory<uintptr_t>(baseAddress + offset);
	if (value != 0) { 
		return value;
	}
	else {
		std::cerr << "Failed to read memory at address " << std::hex << (baseAddress + offset) << std::endl;
		return 0;
	}
}
