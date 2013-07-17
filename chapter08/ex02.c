#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int error(const char * info)
{
	return fprintf(stderr, info, strerror(errno));
}
	

int fkfunc()
{
	pid_t pid;
	/*
	 * if use vfork(), parent won't be excuted until the child process call exec() or exit(). So the output is:
	 *
	 * $ return from child vVal addr: &vVal
	 * * wait for 2 seconds
	 * $ child returned
	 * $ both excute here ?
	 * * exit main add then the parent begin
	 * * return from parent vVal addr: &vVal
	 *
	 */
	if ((pid = vfork()) < 0)

	/* if use fork(), the order is undetermind. but
	 *
	 * $ both excute here ?
	 *
	 * will be excuted by both parent and child process
	 *
	 */
	//if ((pid = fork()) < 0)
	{
		error("vfork error");
		return -1;
	}
	else if (pid == 0)
	{
		int vVal = 0;
		printf("return from child vVal addr: %d\n", &vVal);
		sleep(2);	
		return 0;
	}
	
	int vVal = -1;		
	printf("return from parent vVal addr: %d\n", &vVal);
	return 1;
}

int main (int argc, char ** argv)
{
	int ret;
	if ((ret = fkfunc()) < 0)
	{
		error("fkfork error");
		return -1;
	}
	else if (ret == 0)
	{
		printf("child returned\n");
	}
	else
	{
		printf("parent returned\n");
	}

	printf("both excute here ?\n");

	return 0;

}
