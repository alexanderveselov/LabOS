#include <stdio.h>
#include <unistd.h>

main()
{
	printf("Son pid: %i, ppid: %i\n", getpid(), getppid());
	sleep(2);
	printf("New son ppid: %i\n", getppid());
	printf("Son: exit\n");
}
