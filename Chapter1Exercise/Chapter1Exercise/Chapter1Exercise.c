#define SECURITY_WIN32
#include<stdio.h>
#include<windows.h>
#include<Security.h>
#pragma warning(disable: 4996)
/*
Write a console application that prints more information about the system than the
HelloWin application shown earlier, by calling the following APIs: GetNativeSystem-
Info, GetComputerName, GetWindowsDirectory, QueryPerformanceCounter, GetProductInfo, GetComputerObjectName. Handle errors if they occur.
*/
int main(void)
{
    //GetNativeSystemInfo([out] LPSYSTEM_INFO lpSystemInfo)
    SYSTEM_INFO mySystem;
    GetNativeSystemInfo(&mySystem); //returns nothing, so I can't find out if it has an error or not
    printf("Processor Arch =  %lu , PageSize = %d, and NumberOfProcessors = %d \n", mySystem.wProcessorArchitecture, mySystem.dwPageSize, mySystem.dwNumberOfProcessors);

    //GetComputerNameW(  [out]     LPWSTR  lpBuffer, [in, out] LPDWORD nSize)
    LPWSTR compName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD compNameSize = MAX_COMPUTERNAME_LENGTH + 1;
    if (!GetComputerNameW(compName, &compNameSize)) {
        printf("GetComputerNameW failed!, GetLastError() = %d \n", GetLastError());
    }
    wprintf(L"Computer name : %s\n", compName);

    //GetWindowsDirectoryW([out] LPWSTR lpBuffer,[in] UINT   uSize)
    wchar_t winDirName[MAX_PATH];
    if (!GetWindowsDirectoryW(winDirName, MAX_PATH)) {
        printf("GetWindowsDirectoryW failed!, GetLastError() = %d \n", GetLastError());
    }
    wprintf(L"Windows directory path is = %s\n", winDirName);

    //QueryPerformanceCounter( [out] LARGE_INTEGER* lpPerformanceCount);
    LARGE_INTEGER preCounter;
    if (!QueryPerformanceCounter(&preCounter)) {
        printf("QueryPerformanceCounter() failed!, GetLastError() = %d \n", GetLastError());
    }
    wprintf(L"Current number of ticks returned by QueryPerformanceCounter() = %I64d\n", preCounter.QuadPart);

    //GetProductInfo()
    //I have to use GetVersionExW() which is a deprecated function get get the major,minor,service pack major/minor to pass into 
    //GetProductInfo()
    OSVERSIONINFOEXW myOS = { sizeof(OSVERSIONINFOEX) };
    if (!GetVersionExW(&myOS)) {
        printf("GetVersionExW() failed!, GetLastError() = %d \n", GetLastError());
    }
    /*BOOL GetProductInfo(
  [in]  DWORD  dwOSMajorVersion,
  [in]  DWORD  dwOSMinorVersion,
  [in]  DWORD  dwSpMajorVersion,
  [in]  DWORD  dwSpMinorVersion,
  [out] PDWORD pdwReturnedProductType
);*/
    DWORD osResult = 0;
    if (!GetProductInfo(myOS.dwMajorVersion, myOS.dwMinorVersion, myOS.wServicePackMinor, myOS.wServicePackMinor, &osResult)) {
        printf("GetProductInfo() failed!, GetLastError() = %d \n", GetLastError());
    }
    wprintf(L"GetProductInfo() = %02x\n", osResult);

    /*BOOLEAN SEC_ENTRY GetComputerObjectNameW(
  [in]      EXTENDED_NAME_FORMAT NameFormat,
  [out]     LPWSTR               lpNameBuffer,
  [in, out] PULONG               nSize
);*/
//EXTENDED_NAME_FORMAT compName = { NameDisplay };
    //YOU NEED TO IMPORT Secur32.lib for this function to work!
    wchar_t compN[MAX_PATH];
    ULONG compSize = MAX_PATH;
    if (!GetComputerObjectName(NameCanonical, compN, &compSize)) {
        printf("GetComputerObjectName() failed!, GetLastError() = %d \n", GetLastError()); // This function always returns 1351, since it is not joined to domain, tested it out on a domain joined computer via CRTO labs and it worked
    }
    wprintf(L"GetComputerObjectName() = %s", compN);
    getchar();
    return ERROR_SUCCESS;
}

