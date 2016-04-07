#include <Windows.h>
#include <stdio.h>

void createProcess(const char* filename)
{
    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInformation;

    ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
    printf("Creating %s...\n", filename);
    CreateProcess(filename, 0, 0, 0, FALSE, 0/*CREATE_NEW_CONSOLE*/, 0, 0, &startupInfo, &processInformation);

}


int main()
{
    HANDLE producerToCoderEvent;
    HANDLE decoderToConsumerEvent;

    DeleteFile("data/mapped1.txt");

    // Create sync objects
    producerToCoderEvent = CreateEvent(0, 0, 0, "Producer2Coder");
    if (producerToCoderEvent == INVALID_HANDLE_VALUE)
    {
        printf("Failed to create producer to coder event!\n");
        return -1;
    }

    decoderToConsumerEvent = CreateEvent(0, 0, 0, "Decoder2Consumer");
    if (decoderToConsumerEvent == INVALID_HANDLE_VALUE)
    {
        printf("Failed to create decoder to consumer event!\n");
        return -1;
    }

    createProcess("Debug/producer.exe");
    createProcess("Debug/coder.exe");
    createProcess("Debug/decoder.exe");
    createProcess("Debug/consumer.exe");

    printf("\n");
    
    getchar();
    return 0;

}