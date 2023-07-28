/*
This was not part of the chapter10 exercise, but thought it was significatly more intresting since it focuses on enumerating threads
The program just enumerates threads. It will ask for what PID you want to enumerate the threads of, and returns the
TID, PID, Priority #, and exe path name
*/

#include<Windows.h>
#include<stdio.h>
#include<TlHelp32.h>
#include <wchar.h>
int returnProcessName(DWORD p, wchar_t* path) {
    //WCHAR name[MAX_PATH];
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 proc;
    proc.dwSize = sizeof(proc);
    if (!Process32First(hSnap, &proc)) {
        return 0; //failed
    }
    do {
        if (proc.th32ProcessID == p) {
            wcsncpy(path, proc.szExeFile, MAX_PATH);
            break;
        }
    } while (Process32Next(hSnap, &proc));
    CloseHandle(hSnap);
    return 1;
}

int main() {
    DWORD pid;
    printf("Enter PID of process you want to enumerate the thread of: \n");
    scanf("%lu", &pid);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf("Error with hSnapshot\n");
        return 1;

    }
    THREADENTRY32 te;
    te.dwSize = sizeof(THREADENTRY32);
    if (!Thread32First(hSnapshot, &te)) {
        printf("Thread32FIrst failed!\n, GetLastError() = %d", GetLastError());
        return 1;
    }

    do {

        if (te.th32OwnerProcessID == pid) {
            wchar_t exeName[MAX_PATH];
            returnProcessName(pid, exeName);
            printf("TID: %lu , PID: %u , Priority: %d,", te.th32ThreadID, te.th32OwnerProcessID, te.tpBasePri);
            wprintf(L" exeName: %s\n", exeName);
        }
        //te.dwSize = sizeof(THREADENTRY32);

    } while (Thread32Next(hSnapshot, &te));
    CloseHandle(hSnapshot);
    return ERROR_SUCCESS;
}