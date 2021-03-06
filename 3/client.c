#include "fifo_data.h"
#include <stdio.h>
#include <fcntl.h>

int openFifo(int* readfd, int* writefd)
{
	if ((*writefd = open(fifoName_Read, O_WRONLY)) < 0)
	{
		printf("(Client) Failed to open '%s'!\n", fifoName_Read);
		return 0;
	}

	if ((*readfd = open(fifoName_Write, O_RDONLY)) < 0)
	{
		printf("(Client) Failed to open '%s'!\n", fifoName_Write);
		return 0;
	}
	printf("(Client) FIFO Opened\n");

	return 1;
}

void closeFifo(int readfd, int writefd)
{
	close(readfd);
	close(writefd);
}

void client(int readfd, int writefd)
{
	ssize_t n;
	CustomData data;

	printf("(Client) Enter filename:\n");
	fgets(data.msg, MSG_MAX, stdin);
	data.msg[strlen(data.msg)-1] = 0;
	data.msg_len = strlen(data.msg);

	printf("(Client) Send data <Msg: '%s', Len: %d>\n", data.msg, data.msg_len);
	write(writefd, &data, sizeof(long) + data.msg_len);

	if ((n = read(readfd, &data, sizeof(long) + MSG_MAX)) > 0)
	{
		data.msg[data.msg_len] = 0;
		printf("(Client) Data from server <Msg: '%s', Len: %d>\n", data.msg, data.msg_len);
	}

}

int main()
{
	int readfd, writefd;
	if (!openFifo(&readfd, &writefd))
	{
		return 1;
	}

	client(readfd, writefd);

	closeFifo(readfd, writefd);

	return 0;
}