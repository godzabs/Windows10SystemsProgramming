/*
. Create a system that can run multiple work items concurrently, but some of them may
have dependencies on other work items. As a concrete example, think of compiling
projects in Visual Studio. Some projects depend on other projects, so they must be
processed in order - I will create that project 3 depends on project 2, which depends on project 1
I will fire up project one first and go from there.
*/
#include <stdlib.h>
#include <time.h>
#include<Windows.h>
#include<stdio.h>
#include<stdbool.h>
//Made it global for simplicty sake, in real life I would use structs and pass it in locally via the main function
//The struct would mirror a list node with a current and next handle to point
HANDLE projectEvent[2];

//Thread API to mimic a "project" compiling with dependencies
DWORD WINAPI compileProj(PVOID param) {
	//randomize timing
	srand(time(0));
	int sleep_time = rand() % 121;
	int c = (int*)param;
	//switch casing
	switch (c) {
	case 0:
		printf("compiling 1st project....please wait\n");
		Sleep(sleep_time);
		SetEvent(projectEvent[0]);
		break;

	case 1:
		printf("Waiting on 1st project...\n");
		WaitForSingleObject(projectEvent[0], INFINITE);
		printf("compiling 2nd project....please wait\n");
		Sleep(sleep_time);
		SetEvent(projectEvent[1]);
		break;

	case 2:
		printf("Waiting on 2nd project...\n");
		WaitForSingleObject(projectEvent[1], INFINITE);
		printf("[+] Compiling last project, the 3rd one!!!!!!!....\n");
		break;
	default:
		printf("ERROR!\n");
		return 4;
		break;

	}


	return 0;



}

int main(void) {
	HANDLE hThread[3];

	//init all the event & thread handles
	for (int i = 0; i < 2; i++) {
		projectEvent[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	for (int i = 0; i < 3; i++) {
		hThread[i] = CreateThread(NULL, 0, compileProj, i, 0, NULL);
	}

	//Wait for all handles to signal
	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

	//Below is tearing down all the handles to the threads and event's
	for (int i = 0; i < 2; i++) {
		CloseHandle(projectEvent[i]);
	}
	for (int i = 0; i < 3; i++) {
		CloseHandle(hThread[i]);
	}

	return EXIT_SUCCESS;
}