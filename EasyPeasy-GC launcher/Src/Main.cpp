#include <windows.h>
#include <iostream>
#include <fstream>
#include "json.hpp"

#define GC_NAME "EasyPeasy-GC.json"
#define GC_LIBRARY "EasyPeasy-GC.dll"

std::string GetModulePath(HMODULE module = NULL) {
	char buffer[MAX_PATH]{};
	GetModuleFileName(module, buffer, MAX_PATH);

	return std::string(buffer).substr(0, std::string(buffer).find_last_of("\\/"));
}

int main() {
	std::fstream file(GC_NAME, std::ios::in);
	nlohmann::json config;
	std::string gamePath;

	try {
		if (!file.is_open()) {
			throw std::string("Couldn't open file");
		}

		config = nlohmann::json::parse(file, NULL, false, false);
		file.close();
		
		if (!config.is_object()) {
			throw std::string("Couldn't find the object");
		}

		if (!config["gamePath"].is_string()) {
			throw std::string("Couldn't get game path");
		}

		gamePath = config["gamePath"].get<std::string>();
		file.open(gamePath, std::ios::in);
		if (!file.is_open()) {
			throw std::string("Couldn't find the game");
		}
		file.close();
	}
	catch (const std::string& exception) {
		std::cout << exception << std::endl;

		if (!config.is_object()) {
			config = {};
		}

		char filePath[MAX_PATH]{};

		OPENFILENAME ofn{};
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = filePath;
		ofn.nMaxFile = sizeof(filePath);
		ofn.lpstrInitialDir = ".\\";
		ofn.lpstrFilter = "Executable\0GenshinImpact.exe\0";
		ofn.lpstrTitle = "Select game";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn)) {
			config["gamePath"] = filePath;
			gamePath = filePath;
		}
		else {
			return 0;
		}

		SetCurrentDirectory(GetModulePath().data());
		file.open(GC_NAME, std::ios::out);
		file << config.dump(4);
		file.close();
	}

	PROCESS_INFORMATION pi{};
	STARTUPINFO si{};
	HANDLE process = NULL;
	LPVOID address = NULL;
	SIZE_T nameSize = strlen(GC_LIBRARY);

	try {
		if (!CreateProcess(gamePath.data(), NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
			throw std::string("Couldn't start game, error: ") + std::to_string(GetLastError());
		}

		process = pi.hProcess;
			
		if (!(address = VirtualAllocEx(process, address, nameSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE))) {
			throw std::string("Couldn't allocate memory, error: ") + std::to_string(GetLastError());
		}

		if (!WriteProcessMemory(process, address, GC_LIBRARY, nameSize, NULL)) {
			throw std::string("Couldn't write memory, error: ") + std::to_string(GetLastError());
		}

		HANDLE thread;
		if (!(thread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibrary, address, NULL, NULL))) {
			throw std::string("Couldn't create remote thread, error: ") + std::to_string(GetLastError());
		}

		WaitForSingleObject(thread, INFINITE);

		DWORD dwExitCode = 0;
		GetExitCodeThread(thread, &dwExitCode);
		CloseHandle(thread);

		if (!VirtualFreeEx(process, address, 0, MEM_RELEASE)) {
			address = NULL;

			throw std::string("Couldn't release memory, error: ") + std::to_string(GetLastError());
		}

		ResumeThread(pi.hThread);
		CloseHandle(process);

		std::cout << "Successfully" << std::endl;

		Sleep(1500);
	}
	catch (const std::string& exception) {
		std::cout << exception << std::endl;

		if (address) {
			VirtualFreeEx(process, address, NULL, MEM_RELEASE);
			CloseHandle(process);
		}
	}

	return 0;
}