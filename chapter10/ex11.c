#include "../inc/my_apue.h"

#define BUFFSIZE 100 

static void signal_intr(int signo)
{
    printf("%s", "Catch SIGXFSZ\n");
    return;
}

int copy(int fdsrc, int fddes)
{   
    int nread = 0;
    char buf[BUFFSIZE] = {0};

    while ((nread = read(fdsrc, buf, BUFFSIZE)) >= 0)
    {
        if (write(fddes, buf, nread) < 0)
        {
            error("write error");
            return -1;
        }
    }
    if (nread < 0)
    {
        error("read error");
        return -1;
    }

    return 0;
}
int main(int argc, char *argv[0])
{
    int n;
    char buf[BUFFSIZE];
    if (signal(SIGXFSZ, signal_intr) == SIG_ERR)
    {
        error("signal error");
    }

    struct rlimit limit;
    limit.rlim_cur = 1024;
    limit.rlim_max = 1024;
    if (setrlimit(RLIMIT_FSIZE, &limit) != 0)
    {
        error("setrlimit error");
    }
    
    const char * copyfrom = "copy.from";
    const char * copyto = "copy.to";
    int fdfrom = -1;
    int fdto = -1;  

    if ((fdfrom = open(copyfrom, O_RDONLY)) < 0 )
    {
        error("open copy.from error");
    }
    if (access(copyto, F_OK) ==== 0)
    {
        unlink(copyto);
    }

    if ((fdto = open(copyto, O_RDWR | O_CREAT | O_EXCL, S_IRWXU)) < 0)
    {
        error("open copy.to error");
    }

    copy(fdfrom, fdto);

    exit(0);

copy(fdfrom, fdto);}
            
