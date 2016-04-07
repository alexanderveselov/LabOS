#include <Windows.h>
#include <stdio.h>

// Purpose: Coder

int main()
{
    HANDLE mappedFile;
    HANDLE fileMap;
    HANDLE producerEvent;
	HANDLE pipe;
    DWORD inputFileSize;
    DWORD pipemode;
    DWORD bytesWritten;
    LPSTR mappedData;
    LPSTR codedData;
	LPCSTR pipename = "\\\\.\\pipe\\coderdecoder";
    int i;
    
    printf("=== I am coder ===\n");

    producerEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Producer2Coder");
    if (producerEvent == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open event!\n");
        printf("\n");
        return -1;
    }

    printf("Coder: Wait for event from producer...\n");
    WaitForSingleObject(producerEvent, INFINITE);

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
    printf("Mapped data: %s\n", mappedData);

    codedData = (LPSTR)malloc(inputFileSize);
    ZeroMemory(codedData, inputFileSize);
    memcpy(codedData, mappedData, inputFileSize - 1);

    UnmapViewOfFile(mappedData);
    CloseHandle(fileMap);
    CloseHandle(mappedFile);

    printf("Before coding: %s\n", codedData);

    for (i = 0; i < strlen(codedData); ++i)
    {
        codedData[i] += 1;
    }
    

    printf("After coding: %s\n", codedData);
    Sleep(1000);

    pipe = CreateNamedPipe(pipename, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE, 1, inputFileSize, inputFileSize, INFINITE, 0);
    if (pipe == INVALID_HANDLE_VALUE)
    {
        printf("Failed to create pipe!\n");
        free(codedData);
        return -1;
    }

	pipemode = PIPE_READMODE_MESSAGE;
	if (!SetNamedPipeHandleState(pipe, &pipemode, 0, 0))
    {
		printf("SetNamedPipeHandleState failed\n");
        free(codedData);
        return -1;
    }
    
	if (!WriteFile(pipe, codedData, inputFileSize, &bytesWritten, 0))
    {
        printf("Failed to write to pipe! (%d)\n", GetLastError());
        free(codedData);
        return -1;
    }

    printf("\n");
    printf("Coder: Written to pipe\n");
    CloseHandle(pipe);

    free(codedData);

    return 0;
}
