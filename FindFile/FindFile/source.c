/*
There was no chapter 11 exercise at the end of the book. However I decided to learn more about the FindFirstFileW() function
This program will find a file that you list, I limited it to c:\\users since I don't want to spend the whole day searching for my file
it uses recursion, therefore it is a bit slower
*/
#include<stdio.h>
#include<Windows.h>
#include<stdbool.h>

BOOL found = 0; //global variable to keep track of if we found the file

void findFileT(const wchar_t* directory, const wchar_t* targetFile) {
    wchar_t query[MAX_PATH];
    WIN32_FIND_DATA data;
    swprintf(query, MAX_PATH, L"%s%s", directory, L"\\*\0");
    //printf("Query should be: %ls\n",query);
    HANDLE findFile = FindFirstFileW(query, &data);
    if (findFile == INVALID_HANDLE_VALUE) {
        //  printf("FindFirstFile failed (%d)\n", GetLastError());
        return;
    }
    do {
        if (wcscmp(data.cFileName, L".") == 0 || wcscmp(data.cFileName, L"..") == 0) { //this will omit invalid paths and safe time
            continue;
        }
        wchar_t subDir[MAX_PATH];
        swprintf(subDir, MAX_PATH, L"%s\\%s", directory, data.cFileName);
        if (wcscmp(targetFile, data.cFileName) == 0) {
            wprintf(L"Found the target file!\nLocation:\t%s\\%s", directory, data.cFileName);
            found = 1;
            return;
        }
        else {
            if (!found) {
                findFileT(subDir, targetFile);
            }
            else {
                return;
            }
        }
    } while (FindNextFile(findFile, &data));
    FindClose(findFile);
}


//used wmain since I want to take in a wchar_t as a command-line arguement
int wmain(int argc, const wchar_t* argv[]) {
    if (argc < 2) {
        printf("Usuage: .\Chapter11FindFile <name of file>\nExample: .\Chapter11FindFile test.txt\n");
        return 1;
    }

    findFileT(L"c:\\users", argv[1]); //limited search to c:\users since I don't want it to take all day
    return EXIT_SUCCESS;
}
