#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define THREAD_COUNT 2

pthread_t threads[THREAD_COUNT];

void signalHandler(int sig)
{
	printf("(Thread 2) Received SIGUSR1 - exit\n");
	pthread_exit(0);
}

void* thread_func1(void* arg)
{
	int i = 0;
	for (i = 0; i < 15; ++i)
	{
		if (i == 10)
		{
			printf("(Thread 1) Send SIGUSR1 to Thread 2\n");
			pthread_kill(threads[1], SIGUSR1);
		}
		else
		{
			printf("(Thread 1) i = %d\n", i);
		}
		usleep(rand() % 100000);
	}

}

void* thread_func2(void* arg)
{
	while (1)
	{
		printf("(Thread 2)\n");
		usleep(rand() % 100000);
	}

}

int main()
{
	int i;
	signal(SIGUSR1, signalHandler);

	pthread_create(&threads[0], 0, thread_func1, 0);
	pthread_create(&threads[1], 0, thread_func2, 0);

	for (i = 0; i < THREAD_COUNT; ++i)
	{
		pthread_join(threads[i], 0);
	}
	
	return 0;
}