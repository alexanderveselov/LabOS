#include <windows.h>
#include <stdio.h>

// Purpose: Consumer

int main()
{
    HANDLE outputFile;
    HANDLE mappedFile;
    HANDLE fileMap;
    HANDLE decoderEvent;
    DWORD inputFileSize;
    DWORD bytesWritten;
    LPSTR mappedData;

    decoderEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Decoder2Consumer");
    if (decoderEvent == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open event!\n");
        printf("\n");
        return -1;
    }

    printf("Consumer: Wait for event from decoder...\n");
    WaitForSingleObject(decoderEvent, INFINITE);

    
    mappedFile = CreateFile("data/mapped1.txt", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (mappedFile == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open mapped file!\n");
        return -1;
    }

    fileMap = CreateFileMapping(mappedFile, 0, PAGE_READWRITE, 0, 0, "mapped1");
    if (fileMap == NULL)
    {
        printf("Failed to create file map! (%d)\n", GetLastError());
        CloseHandle(mappedFile);
        return -1;
    }
    
    inputFileSize = GetFileSize(mappedFile, 0);
    mappedData = (LPSTR)MapViewOfFile(fileMap, FILE_MAP_WRITE, 0, 0, inputFileSize);
    printf("Consumer: Mapped data: %s\n", mappedData);

    outputFile = CreateFile("data/output.txt", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    WriteFile(outputFile, mappedData, inputFileSize, &bytesWritten, 0);
    
    printf("Consumer: written to file\n");

    UnmapViewOfFile(mappedData);
    CloseHandle(fileMap);
    CloseHandle(mappedFile);
    CloseHandle(outputFile);

    return 0;
}
