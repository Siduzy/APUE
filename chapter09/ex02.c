#include <unistd.h>
#include <fcntl.h>

#define PS_CMD "ps -o pid,ppid,pgid,sid,comm"
int main(int argc, char ** argv)
{
	pid_t pid;

	if ((pid = fork()) < 0)
	{
		perror("fork");
		return -1;
	}
	else if (pid == 0) /* child */
	{
		sleep(5);
		pid_t pgid;
		if ((pgid = setsid()) == -1)
		{
			perror("pgid");
			return -1;
		}
	
		if (system(PS_CMD) < 0)
		{
			perror("system");
			return -1;
		}
		
		if (printf("from child\n") <= 0)
		{
			perror("printf error");
			exit(-1);
		}
		if (open("/dev/tty", O_RDONLY) < 0)
		{
			perror("can't open the tty from child");
			exit(-1);
		}
		exit(0);
	}
	
	/* parent */
	if (open("/dev/tty", O_RDONLY) < 0)
	{
		perror("can't open the tty from parent");
	}
	else
	{
		printf("parent open /dev/tty succeed\n");
	}	
	exit(0);
}

