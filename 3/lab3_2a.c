#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define LINE_LEN 80

void client(int readfd, int writefd)
{
	ssize_t n;
	char buffer[LINE_LEN];

	printf("(Client) Enter filename:\n");
	fgets(buffer, LINE_LEN, stdin);
	buffer[strlen(buffer)-1] = 0;
	printf("(Client) Send message '%s' to server\n", buffer);
	write(writefd, buffer, strlen(buffer));

	printf("(Client) Message from server: '");
	while ((n = read(readfd, buffer, LINE_LEN)) > 0)
	{
		buffer[n] = 0;
		printf("%s", buffer);
	}
	printf("'\n");

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
	int pipe1[2], pipe2[2];
	pid_t childpid;

	pipe(pipe1);
	pipe(pipe2);

	childpid = fork();
	if (childpid == 0)
	{
		close(pipe1[1]);
		close(pipe2[0]);
		server(pipe1[0], pipe2[1]);

	}
	else
	{
		close(pipe1[0]);
		close(pipe2[1]);
		client(pipe2[0], pipe1[1]);
		waitpid(childpid, 0, 0);
	}

	return 0;
}