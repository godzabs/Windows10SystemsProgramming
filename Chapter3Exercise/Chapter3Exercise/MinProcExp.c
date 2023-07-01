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
int main(void) {

	int choice = 0;
	printf("[+] Welcome to the dullest process enumerator!\nWhat would you like to do?: \n");

	do {

		printf("1.List all processes\n2.Terminate a process with a valid PID\n3.Change priority class\n4.Quit :(\n");
		scanf_s("%d", &choice); 
		switch (choice) {
		case 1:
			enumProcesses();
			break;
		case 2: //TODO 2
			printf("You choose terminate proc\n");
			break;
		case 3: //TODO 3
			printf("You choose change priority class\n");
			break;

			default:
			break;
		}

	} while (choice != 4);

	return EXIT_SUCCESS;
}

