#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_handler(int sig)
{
	printf("Received signal %i\n",sig);
	sleep(5);

}

void setSignal(int sig,struct sigaction* new_action, struct sigaction* old_action)
{
	new_action->sa_handler=sig_handler;
	sigemptyset(&(new_action->sa_mask));
	// Block SIGINT while other signals are handling
	sigaddset(&(new_action->sa_mask),SIGINT);
	new_action->sa_flags = 0;

	if(sigaction(sig,new_action,old_action)<0)
	{
		printf("Set signal error!\n");
		exit(0);
	}
	printf("Set signal success!\n");

}

void restoreSignal(int sig, struct sigaction* old_action)
{
	if(sigaction(sig,old_action,NULL)<0)
	{
		printf("Restore signal error!\n");
		exit(0);
	}
	printf("Restore signal success!\n");

}

int main()
{
	struct sigaction new_usr1_action, old_usr1_action, new_usr2_action, old_usr2_action,
	new_usr3_action, old_usr3_action;

	printf("Process pid = %i\n", getpid());

	setSignal(SIGUSR1, &new_usr1_action, &old_usr1_action);
	setSignal(SIGUSR2, &new_usr2_action, &old_usr2_action);
	setSignal(SIGINT,  &new_usr3_action, &old_usr3_action);

	if ((kill(getpid(), SIGUSR1)) == -1)
	{
		printf("Signal send with error!\n");
	}
	else
	{
		printf("Signal send OK!\n");
	}

	sleep(5);

	restoreSignal(SIGUSR1,&old_usr1_action);
	restoreSignal(SIGUSR2,&old_usr2_action);
	restoreSignal(SIGINT, &old_usr3_action);

	return 0;
}