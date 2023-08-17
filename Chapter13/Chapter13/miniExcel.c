#include<stdio.h>
#include<Windows.h>
#include<string.h>
int CellSize = 1024, SizeX = 1024, SizeY = 1024;
size_t TotalSize = 1024 * 1024 * 1024;
void* m_address = NULL; //this will hold a pointer to the memory address of our VirtualAlloc call

void AllocateRegion() {
    m_address = VirtualAlloc(NULL, TotalSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!m_address) {
        printf("[-]ERROR, Failed to allocate memory!\n");
        return;
    }
    printf("[+]Memory allocated, address is 0x%p \n", m_address);
}

void* getCell(int x, int y) {

    if (x < 0 || x >= SizeX || y < 0 || y >= SizeY) {
        printf("[-]Oi the indices you input are out of range!\n");
        return NULL;
    }
    return (BYTE*)m_address + CellSize * ((size_t)x + SizeX * y);

}

void writeCell(int x, int y) {
    void* p = getCell(x, y);
    if (!p) {
        printf("[-]Call to getCell failed!\n");
        return;
    }
    WCHAR text[512];
    printf("Enter text you want to store in cell: ");
    while (getchar() != '\n');
    fgetws(text, 512, stdin);
    wcscpy_s((WCHAR*)p, CellSize / sizeof(WCHAR), text);
    printf("[+]Wrote text to cell");
}

void onRead(int x, int y) {
    void* p = getCell(x, y);
    if (!p) {
        printf("[-]Call to getCell failed!\n");
        return;
    }
    WCHAR text[512];
    wcscpy_s(text, CellSize / sizeof(WCHAR), (PCWSTR)p);
    printf("\nData retrieved from cells %d, %d : %ls\n", x, y, text);
}



int main(void) {

    AllocateRegion();
    int value = 0;
    int c, d;
    for (;;) {
        printf("\nWhat would you like to do:\n1.Enter text\n2.Read text\n3.Quit\n");
        scanf_s("  %d", &value);
        switch (value) {
        case 1:
            printf("Enter x: \n");
            scanf_s("  %d", &c);
            printf("Enter y: \n");
            scanf_s("  %d", &d);
            writeCell(c, d);
            break;
        case 2:
            printf("Enter x: \n");
            scanf_s("%d", &c);
            printf("Enter y: \n");
            scanf_s("%d", &d);
            onRead(c, d);
            break;
        case 3:
            exit(0);
            break;

        }
    }

    return EXIT_SUCCESS;
}