#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int error(const char * info)
{
	return fprintf(stderr, info, strerror(errno));
}

int zombieprocess()
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		error("fork error");
		return -1;
	}
	else if (pid == 0)
	{
		exit(2);
	}
	
	if (system("ps") < 0)
	{
		error("system error");
	}
	
	sleep(3);
	return 0;
	
}

int main(int argc, char ** argv)
{
	zombieprocess();
	return 0;
}
