#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

DWORD getProcIDW(const wchar_t* procName) {
	HANDLE processList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	DWORD procID = 0;

	if (processList == INVALID_HANDLE_VALUE) return procID;

	PROCESSENTRY32W procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32W);

	if (Process32FirstW(processList, &procEntry)) {
		do {
			if (_wcsicmp(procEntry.szExeFile, procName) == 0) {
				procID = procEntry.th32ProcessID;
				break;

			}

		} while (Process32NextW(processList, &procEntry));

	}

	CloseHandle(processList);
	return procID;

}

int main() {
	wchar_t* exeName{0};
	char* dllPath{0};

	std::cout << "#=====-Injector started-=====#\n";
	std::cout << "Enter process-name:";
	std::wcin >> exeName;                             //This line crashes, remaining program functions as prev tests have shown
	std::cout << "Enter dll-path:";
	std::cin >> dllPath;

	std::cout << "Searching...\n";
	DWORD procID = getProcIDW(exeName);
	std::cout << "Process ID: [" << procID << "]\n";

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);
	if (hProcess && hProcess != INVALID_HANDLE_VALUE) {
		std::cout << "Succesfully opened process-handle\n";
		void* loc = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); 
		WriteProcessMemory(hProcess, loc, dllPath, strlen(dllPath)+1, 0); 
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, NULL, NULL); 

		if (hThread) CloseHandle(hThread);

	}
	else {
		std::cout << "Failed to open process-handle, terminating programm\n";

	}

	if (hProcess) CloseHandle(hProcess);
	return 0;

}