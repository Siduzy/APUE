// ex05.c
//
// try to creat a symbol link contains nothing. result is FAILED
//
//
#include <unistd.h>
#include <stdio.h>

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
#define MAX_PATH 1024

int main(int argc, char *argv[])
{
	char * linkname = "link.name";
	if (symlink("", linkname) == -1)
	{
		perror("line 12");
		return -1;
	}

	char buf[MAX_PATH] = {0};
	int ret = -1;
	if ((ret=readlink(linkname, buf, MAX_PATH-1)) == -1)
	{
		perror("line 20");
		return -1;
	}
	
	buf[ret+1] = 0;	
	printf("the link is %s", buf);
}
