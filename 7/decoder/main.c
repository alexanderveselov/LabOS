#include <Windows.h>
#include <stdio.h>

// Purpose: Decoder

int main()
{
	HANDLE pipe;
    HANDLE mappedFile;
    HANDLE fileMap;
    HANDLE consumerEvent;
    LPSTR inputData;
    LPSTR mappedData;
    DWORD bytesRead;
	LPCSTR pipename = "\\\\.\\pipe\\coderdecoder";
    int i;
    
    printf("=== I am decoder ===\n");

    printf("Decoder: wait for pipe\n");
    WaitNamedPipe(pipename, INFINITE);

    while (1)
	{
        pipe = CreateFile(pipename, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
		if (pipe != INVALID_HANDLE_VALUE)
		{
			break;
		}
	}
    printf("Decoder: Pipe opened\n");
    
    inputData = (LPSTR)malloc(GetFileSize(pipe, 0));
    ReadFile(pipe, inputData, GetFileSize(pipe, 0), &bytesRead, 0);
    CloseHandle(pipe);

    printf("Before decoding: %s\n", inputData);
    for (i = 0; i < strlen(inputData); ++i)
    {
        inputData[i] -= 1;

    }
    printf("After decoding: %s\n", inputData);

    mappedFile = CreateFile("data/mapped2.txt", GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (mappedFile == INVALID_HANDLE_VALUE)
    {
        printf("Failed to create mapped file!\n");
        printf("\n");
        return -1;
    }

    SetFilePointer(mappedFile, bytesRead, 0, FILE_BEGIN);
    SetEndOfFile(mappedFile);
    fileMap = CreateFileMapping(mappedFile, 0, PAGE_READWRITE, 0, 0, "mapped2");
    if (fileMap == NULL)
    {
        printf("Failed to create file map! (%d)\n", GetLastError());
        free(inputData);
        CloseHandle(mappedFile);
        printf("\n");
        return -1;
    }
    mappedData = (LPSTR)MapViewOfFile(fileMap, FILE_MAP_WRITE, 0, 0, bytesRead);
    memcpy(mappedData, inputData, bytesRead);
    free(inputData);
    UnmapViewOfFile(mappedData);
    CloseHandle(fileMap);
    CloseHandle(mappedFile);
    
    Sleep(1000);

    consumerEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Decoder2Consumer");
    if (consumerEvent == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open event!\n");
        printf("\n");
        return -1;
    }
    printf("Decoder: Set event to consumer\n");
    SetEvent(consumerEvent);
    CloseHandle(consumerEvent);

    printf("\n");


    return 0;
}
