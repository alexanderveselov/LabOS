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
	ssize_t n;
	char buffer[LINE_LEN];

	n = read(readfd, buffer, LINE_LEN);
	buffer[n] = 0; 
	printf("(Server) Trying to open file '%s'...\n", buffer);
	fd = open(buffer, O_RDONLY);
	if (fd < 0)
	{
		const char* errorMsg = "Failed to open file!";
		write(writefd, errorMsg, strlen(errorMsg));
	}
	else
	{
		while ((n = read(fd, buffer, LINE_LEN)) > 0)
		{
			buffer[n] = 0;
			write(writefd, buffer, n);
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