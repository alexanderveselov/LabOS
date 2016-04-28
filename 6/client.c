#include "defs.h"

int main()
{
	int fd;
	void* mapped_data;
	sem_t *sem1;
	sem_t *sem2;

	sem1 = sem_open(SEM1_NAME, 0);
	if (sem1 == SEM_FAILED)
	{
		printf("(Client) Failed to open semaphore 1!\n");
		return 1;
	}

	sem2 = sem_open(SEM2_NAME, 0);
	if (sem2 == SEM_FAILED)
	{
		printf("(Client) Failed to open semaphore 2!\n");
		return 1;
	}

	fd = open(FMAP_NAME, O_RDWR);
	if (fd < 0)
	{
		printf("(Client) Failed to open mapped file!\n");
		return 1;
	}
	mapped_data = mmap(0, MSG_MAX, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (mapped_data == MAP_FAILED)
	{
		printf("Failed to create file mapping!\n");
		return 1;
	}

	printf("(Client) Enter filename:\n");
	scanf("%s", mapped_data);

	sem_post(sem1);
	printf("(Client) Send message to server\n");

	sem_wait(sem2);
	printf("(Client) Message from server: '%s'\n", mapped_data);

	munmap(mapped_data, MSG_MAX);
	close(fd);

	return 0;
}
