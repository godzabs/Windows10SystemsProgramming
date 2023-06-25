#include<windows.h>
#include<stdio.h>
/*
etNativeSystem-
Chapter 1: Foundations 41
GetNativeSystemInfo, GetWindowsDirectory, QueryPerformanceCounter, GetProductInfo, GetComputerObjectName*/
int main(void) {

	LPWSTR compName = (LPWSTR)malloc(sizeof(LPWSTR) * 20);
	DWORD compNameSize = 0;
	//The first call is meant to fail, since the function will return the true size of the 
	if (!GetComputerNameW(compName, &compNameSize)) {
		compName = (LPWSTR)realloc(compName,sizeof(LPWSTR)*compNameSize);
		if (!GetComputerNameW(compName, &compNameSize)) {
			printf("Second call failed therefore it's not a size error, GetLastError() = %d", GetLastError());
			return 1;

		}
	}
	wprintf(L"Computer name = %ls\n", compName);
	free(compName);


	UINT winDirSize = MAX_PATH;
	LPWSTR winDir = (LPWSTR)malloc(sizeof(LPWSTR) * MAX_PATH);
	if (!(GetWindowsDirectoryW(winDir,winDirSize))) {
		printf("GetWindowsDirectoryW() Failed, GetLastError() = %d", GetLastError());
		return 1;
	}
	wprintf(L"Windows directory = %ls\n", winDir);
	free(winDir);





	getchar();
	return ERROR_SUCCESS;
}