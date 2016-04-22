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
	char buffer[LINE_LEN];

	printf("(Client) Enter filename:\n");
	fgets(buffer, LINE_LEN, stdin);
	buffer[strlen(buffer)-1] = 0;
	printf("(Client) Send message '%s' to the server\n", buffer);
	write(writefd, buffer, strlen(buffer));

	printf("(Client) Message from the server: '");
	while ((n = read(readfd, buffer, LINE_LEN)) > 0)
	{
		buffer[n] = 0;
		printf("%s", buffer);
	}
	printf("'\n");

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