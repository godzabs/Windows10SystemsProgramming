/*
	1.Write a GUI or console application called MiniProcExp - a mini Process Explorer, that
	uses either the Toolhelp APIs, WTS APIs or the native API to show information about
	processes. Anything that is not readily available get by opening a proper handle to the
	process - I am going with the Toolhelp API since you can view stuff without elevated command prompt

	2.Extend the previous application by adding operations on processes, such as terminating
	  and changing priority class
*/


#include<windows.h>
#include<tlhelp32.h>
#include<stdio.h>

/*
Purpose of this function is to enumerate processes using the Toolhelp32Snapshot functions
it's a void function, returns nothing
*/
void enumProcesses(void) {
	HANDLE hProc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 proc;
	proc.dwSize = sizeof(proc);
		
	if (hProc == INVALID_HANDLE_VALUE) {
		printf("Failed to get a handle!\n");
	}
	if (!Process32First(hProc,&proc))
	{
		printf("CreateToolhelp32Snapshot() failed!, GetLastError() = %d\n", GetLastError());
	}
	do {
	
		printf("PID:%6d (PPID:%6d): %ws (Threads=%d) (Priority=%d)\n",proc.th32ProcessID, proc.th32ParentProcessID, proc.szExeFile,proc.cntThreads, proc.pcPriClassBase);
	
	} while (Process32Next(hProc,&proc));

	CloseHandle(hProc);
	
}

/*
	void function to close a PID that is passed in, as long as we can get a handle on it
*/
void terminateProcess(DWORD pid) {
	HANDLE hProc = OpenProcess(PROCESS_TERMINATE,FALSE,pid);
	if (hProc == NULL) {
		printf("Failed to get a handle on PID %u, GetLastError() = %d \n", pid, GetLastError());
	}
	DWORD exitCode;
	if (!TerminateProcess(hProc,GetExitCodeProcess(hProc,&exitCode))) {
		printf("Failed to terminate proceson PID %u, GetLastError() = %d \n\n", pid, GetLastError());
	}
	else {
		printf("Successfully closed PID %u !\n",pid);
	}

	CloseHandle(hProc);

}

/*
	void function to change the priority of a PID that is passed in, as long as we can get a handle on it
*/
void changePriority(DWORD pid) {

	HANDLE hProc = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pid);
	if (hProc == NULL) {
		printf("Failed to get a handle on PID %u, GetLastError() = %d \n", pid, GetLastError());
	}
	DWORD exitCode;
	if (!SetPriorityClass(hProc, BELOW_NORMAL_PRIORITY_CLASS)) {
		printf("Failed to change the priority class on process on PID %u, GetLastError() = %d \n\n", pid, GetLastError());
	}
	else {
		printf("Successfully change priority of PID %u to BELOW_NORMAL_PRIORITY_CLASS!\n", pid);
	}

	CloseHandle(hProc);
}

int main(void) {

	int choice = 0;
	DWORD piD;
	printf("[+] Welcome to the dullest process explorer!\nWhat would you like to do?: \n");

	do {

		printf("1.List all processes\n2.Terminate a process with a valid PID\n3.Change priority class\n4.Quit :(\n");
		scanf_s("%d", &choice); 
		switch (choice) {
		case 1:
			enumProcesses();
			break;
		case 2:
			printf("\nGive the PID of the process you want to end: ");
			scanf_s("%u", &piD);
			terminateProcess(piD);
			break;
		case 3: //TODO 3
			printf("\nGive the PID of the process you want to change priroity to BELOW_NORMAL_PRIORITY_CLASS: ");
			scanf_s("%u", &piD);
			changePriority(piD);
			break;

			default:
			break;
		}

	} while (choice != 4);

	return EXIT_SUCCESS;
}

