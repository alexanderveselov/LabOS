#include <windows.h>
#include <stdio.h>

// Purpose: Producer

int main()
{
    HANDLE inputFile;
    HANDLE mappedFile;
    HANDLE fileMap;
    HANDLE coderEvent;
    DWORD bytesToRead;
    DWORD bytesRead;
    LPSTR inputData;
    LPSTR mappedData;
    
    printf("\n");
    printf("=== I am producer ===\n");

    inputFile = CreateFile("data/input.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (inputFile == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open input file!\n");
        printf("\n");
        return -1;
    }

    bytesToRead = GetFileSize(inputFile, 0);
    printf("File size: %d\n", bytesToRead);

    inputData = (LPSTR)malloc(bytesToRead);
    ZeroMemory(inputData, bytesToRead);

    ReadFile(inputFile, inputData, bytesToRead - 1, &bytesRead, 0);
    printf("Input message: %s\n", inputData);
    CloseHandle(inputFile);

    mappedFile = CreateFile("data/mapped1.txt", GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (mappedFile == INVALID_HANDLE_VALUE)
    {
        printf("Failed to create mapped file!\n");
        printf("\n");
        return -1;
    }

    SetFilePointer(mappedFile, bytesRead, 0, FILE_BEGIN);
    SetEndOfFile(mappedFile);

    fileMap = CreateFileMapping(mappedFile, 0, PAGE_READWRITE, 0, 0, "mapped1");
    if (fileMap == NULL)
    {
        printf("Failed to create file map! (%d)\n", GetLastError());
        CloseHandle(mappedFile);
        printf("\n");
        return -1;
    }

    mappedData = (LPSTR)MapViewOfFile(fileMap, FILE_MAP_WRITE, 0, 0, strlen(inputData));
    memcpy(mappedData, inputData, strlen(inputData));

    free(inputData);
    UnmapViewOfFile(mappedData);
    CloseHandle(fileMap);
    CloseHandle(mappedFile);

    coderEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Producer2Coder");
    if (coderEvent == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open event!\n");
        printf("\n");
        return -1;
    }
    Sleep(1000);
    
    printf("Producer: Set event to coder\n");

    SetEvent(coderEvent);
    CloseHandle(coderEvent);

    printf("\n");

    //getchar();
    return 0;

}
