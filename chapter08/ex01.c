#include <unistd.h>

int glob = 6;

int main(int argc, char ** argv)
{
	int var;
	pid_t pid;

	var = 88;
	printf("before vfork\n");
	if ((pid = vfork()) < 0) 
	{
		perror(argv[0]);
	}
	else if (pid ==0)
	{
		glob++;
		var++;
		//fclose(stdout);
		_exit(0);
	}

	/*
	 * Parent continues here
	 */
	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
	return 0;
}
