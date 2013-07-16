#include<pwd.h>
#include<stdio.h>
#include <errno.h>

int main(int argc, char **argv)
{
	setpwent();
	struct passwd * pswd;
	
	while ((pswd=getpwent()) != NULL)
	{
		printf("Name:%s Uid:%u Gid:%u Dir:%s SHL: %s",
				pswd->pw_name,
				pswd->pw_uid,
				pswd->pw_gid,
				pswd->pw_dir,
				pswd->pw_shell);
	}

	if (errno!=0)
	{
		perror(argv[0]);
	}

	endpwent();
}
		
