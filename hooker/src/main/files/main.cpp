#include "memory/inject.h"
int main() {
    std::cout << "Hello, it is hook system" << std::endl;
    std::cout << "Type INFO for information" << std::endl;

    std::cout << "Type COMMAND for command list" << std::endl;

    std::cout << "Sorry for a lot of bugs..." << std::endl;

    

    std::cin >> Engine::userInputString;

    if (Engine::userInputString == "INFO") {
        std::cout << "Information table:" << std::endl;
        std::cout << "It is hook system, you can type a process and get information about it." << std::endl;
        std::cout << "You can hook a function of this process." << std::endl;
        std::cout << "Btw, it's not really hooks, it's more like Cheat Engine. It's useful for getting offsets, addresses, patterns, and testing your own offsets using TEST_OFFSET(type HOOK before and attach to process), and also injecting dll using INJECT_DLL(type HOOK before and attach to process)." << std::endl;
        
    }

    if (Engine::userInputString == "COMMAND") {
        std::cout << "DUMP" << std::endl;
        std::cout << "HOOK" << std::endl;
    }

    if (Engine::userInputString == "HOOK") {
       

        std::wcout << L"Enter process name (e.g., process.exe): ";
        std::wcin >> processName;

        
        if (!FindProcessName(processName)) {
            std::wcout << L"Process not found!" << std::endl;
        }

        std::cout << "Type FIND_ALL_DLLS to get list of dlls" << std::endl;
        std::cout << "Type FIND_EXACT_DLL to find dll" << std::endl;
        std::cout << "Type EXTRACT_OFFSETS_FROM_DLL to get offsets from dll" << std::endl;
        std::cout << "Type GET_DLL_BASE_ADRESS to get dll base address" << std::endl;
        std::cout << "Type TEST_OFFSET to read a value from dll" << std::endl;
        std::cout << "Type INJECT_DLL_UNSAFE (not safe, cause anticheats can detect" << std::endl;
        std::cout << "Type INJECT_DLL_SAFE to inject(safe, but may not work)" << std::endl;


        std::cin >> Engine::userInputString;
        if (Engine::userInputString == "FIND_ALL_DLLS") {
            GetAllModules();
        }
        if (Engine::userInputString == "FIND_EXACT_DLL") {
            std::cout << "Enter module name:";
            std::wcin >> moduleName;
            if (FindExactModule(moduleName)) {
                std::wcout << "Found module, name: " << moduleName << std::endl;
            }
        }
        if (Engine::userInputString == "GET_DLL_BASE_ADRESS") {
            std::cout << "enter dll:";
            std::string dllName;
            std::cin >> dllName;
            GetDllBaseAddress(dllName.c_str());

        }
        if (Engine::userInputString == "TEST_OFFSET") {
            uintptr_t offset;
            std::cout << "Enter an offset or idk(0x12 like this or 0x9123AKDS)";
            std::cin >> offset;

            TestOffset(offset);
        }

        if (Engine::userInputString == "INJECT_DLL_SAFE") {
            std::cout << "IT MAY NOT INJECT IN ALL APPS, REASON CAN BE ALSO IN YOUR DLL" << std::endl;
            std::cout << "injector use not createRemoteThread() method" << std::endl;
            std::cout << "Currently in develop, already was tried two methods, none of them works, (crashing)" << std::endl;
        }

        if (Engine::userInputString == "INJECT_DLL_UNSAFE") {
            std::string dllPath;
            std::cout << "Path: " << std::endl;
            std::cin >> dllPath;
            if (InjectDLL(dllPath)) {
                std::cout << "DLL injected successfully!" << std::endl;
            }
            else {
                std::cerr << "DLL injection failed!" << std::endl;
            }
        }

        if (Engine::userInputString == "DUMP"){
            std::cout << "Sorry not working rn" << std::endl;
        }
        

    }


    system("pause");
}