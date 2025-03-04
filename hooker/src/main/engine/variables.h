#pragma once
#include <string>
#include "Windows.h"

wchar_t processName[256];
wchar_t moduleName[256];

namespace Engine
{
	int userInput;
	std::string userInputString;
	std::string processName;

	std::string directoryPath;
};

struct MemoryData
{
	int processId;
	const wchar_t* ProcessName;
	HANDLE processHandle;
}; MemoryData memoryData;