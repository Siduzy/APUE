#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#ifndef OPEN_MAX
#define OPEN_MAX 64
#endif

int fdimpl(int fd1, int fd2, int lastfd)
{
	int fd_cur = OPEN_MAX;
	if ((fd_cur = dup(fd1)) != fd2)
	{
		lastfd = fd_cur;
		fdimpl(fd1, fd2, lastfd);
	}	
	close(lastfd);
}
//
// My dup2
//
// if success, return the file descriptor
// if failed, return -1
// ...................................................

int mydup2 (int fd1, int fd2)
{
	if (fd2 < 0 || fd2 >= OPEN_MAX)
	{
		return -1;
	}

	if (fd1 == fd2)
	{
		return fd1;
	}

	close(fd2);

	fdimpl(fd1, fd2, OPEN_MAX);
	
	return fd2;

}

int main (int argc, char * argv[]) 
{
	const char * filename = "file.name";
	if (access(filename, F_OK) == 0)
	{
		unlink(filename);
	}
	int fd = OPEN_MAX;
	if ((fd=open(filename, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR)) == -1)
	{
		perror(argv[0]);
		return -1;
	}
	
	int ret = mydup2(fd, 10);
	
	int fd_min= dup(STDIN_FILENO);
	printf("min_fd is %d\n", fd_min);
	printf ("ret %d\n", ret);	
}
