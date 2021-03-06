#include "fifo_data.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int openFifo(int* readfd, int* writefd)
{
	if (mknod(fifoName_Read, S_IFIFO | 0666, 0) < 0)
	{
		printf("(Server) Failed to create '%s'!\n", fifoName_Read);
		return 0;
	}

	if (mknod(fifoName_Write, S_IFIFO | 0666, 0) < 0)
	{
		printf("(Server) Failed to create '%s'!\n", fifoName_Read);
		// Delete previous created FIFO
		unlink(fifoName_Read);
		return 0;
	}

	printf("(Server) FIFO Created\n");

	if ((*readfd = open(fifoName_Read, O_RDONLY)) < 0)
	{
		printf("(Server) Failed to open '%s'!\n", fifoName_Read);
		return 0;
	}

	if ((*writefd = open(fifoName_Write, O_WRONLY)) < 0)
	{
		printf("(Server) Failed to open '%s'!\n", fifoName_Write);
		return 0;
	}
	printf("(Server) FIFO Opened\n");

	return 1;

}

void closeFifo(int readfd, int writefd)
{
	close(readfd);
	close(writefd);

	unlink(fifoName_Read);
	unlink(fifoName_Write);
}

void server(int readfd, int writefd)
{
	int fd;
	CustomData data;

	read(readfd, &data, sizeof(long) + MSG_MAX);
	data.msg[data.msg_len] = 0;
	printf("(Server) Received data <Msg: '%s', Len: %d>\n", data.msg, data.msg_len);
	
	fd = open(data.msg, O_RDONLY);
	if (fd < 0)
	{
		strcpy(data.msg, "Failed to open file!");
		data.msg_len = strlen(data.msg);
		data.msg[data.msg_len] = 0;
		write(writefd, &data, sizeof(long) + data.msg_len);
	}
	else
	{
		if ((data.msg_len = read(fd, data.msg, MSG_MAX)) > 0)
		{
			data.msg[data.msg_len] = 0;
			write(writefd, &data, sizeof(long) + data.msg_len);
		}
		close(fd);
	}
}

int main()
{	
	int readfd, writefd;
	if (!openFifo(&readfd, &writefd))
	{
		return 1;
	}

	server(readfd, writefd);

	closeFifo(readfd, writefd);
	return 0;
}