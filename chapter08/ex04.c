#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int error(const char * info)
{
    return fprintf(stderr, info, strerror(errno));
}

static const char * filename = "/home/sid/testinterp";
int main(int argc, char ** argv)
{
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        error("fork error");
        return -1;
    }
    else if (pid == 0)
    {
        //if (execl(filename, "testinterp", "myarg1", "MY arg2", (char *)0)<0)
        if (execlp(filename, "testinterp", "myarg1", "MY arg2", (char *)0)<0)
        {
            error("execlp error");
            return -1;
        }

    }
    if (waitpid(pid, NULL, 0) < 0)
    {
        error("waitpid error");
    }

    return 0;
}
    

