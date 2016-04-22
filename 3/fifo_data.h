#ifndef FIFO_DATA_H
#define FIFO_DATA_H

#include <string.h>

#define MSG_MAX 1024

typedef struct
{
	long msg_len;
	char msg[MSG_MAX];

} CustomData;

const char* fifoName_Read = "/tmp/fifo_read"; 
const char* fifoName_Write = "/tmp/fifo_write";


#endif // FIFO_DATA_H
