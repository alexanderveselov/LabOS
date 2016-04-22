#include <stdio.h>
#include <unistd.h>

main()
{
   int pid, ppid, status;

   pid = getpid();
   ppid = getppid();
   printf("Father pid: %i, ppid: %i\n", pid, ppid);

   if (fork() == 0)
   {
       execl("son", NULL);
   }
   //system("ps xf | grep 32");
   sleep(1);
   //wait(&status);

   printf("Father: exit\n");

}
