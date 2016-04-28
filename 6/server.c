#include "defs.h"

int main()
{
	int mm_fd;
	int fd;
	void* mapped_data;
	sem_t *sem1;
	sem_t *sem2;

	sem1 = sem_open(SEM1_NAME, O_CREAT, 0777, 0);
	if (sem1 == SEM_FAILED)
	{
		printf("(Server) Failed to create semaphore 1!\n");
		return 1;
	}

	sem2 = sem_open(SEM2_NAME, O_CREAT, 0777, 0);
	if (sem2 == SEM_FAILED)
	{
		printf("(Server) Failed to create semaphore 2!\n");
		return 1;
	}

	mm_fd = open(FMAP_NAME, O_RDWR | O_CREAT, 0666);
	if (mm_fd < 0)
	{
		printf("(Server) Failed to create mapped file!\n");
		return 1;
	}
	lseek(mm_fd, MSG_MAX, SEEK_SET);
	write(mm_fd, "", 1);
	mapped_data = mmap(0, MSG_MAX, PROT_READ | PROT_WRITE, MAP_SHARED, mm_fd, 0);
	if (mapped_data == MAP_FAILED)
	{
		printf("Failed to create file mapping!\n");
		return 1;
	}

	printf("(Server) Wait for message from client...\n");
	sem_wait(sem1);

	printf("(Server) Message from client: '%s'\n", mapped_data);

	fd = open(mapped_data, O_RDONLY);
	memset(mapped_data, 0, MSG_MAX);
	if (fd < 0)
	{
		strcpy(mapped_data, "Failed to open file!");
	}
	else
	{
		read(fd, mapped_data, MSG_MAX);
	}

	sem_post(sem2);
	printf("(Server) Send message to client\n");

	munmap(mapped_data, MSG_MAX);
	close(mm_fd);
	return 0;

}
