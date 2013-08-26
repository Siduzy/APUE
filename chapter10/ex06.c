#include "../inc/my_apue.h"
#include "../inc/sync.h"

const char * filename = "ex06.rslt";

const char * prntinfo = "Parent: ";
const char * chldinfo = "Child: ";
int towrite = 0;

int init()
{
    umask(0177);
    TELL_WAIT();
}

int writenum(int fd, const char * writer, int num)
{
    char temp[MAXBUFLEN] = {0};
    sprintf(temp, "%s %d\n", writer, num);
    return write(fd, temp, strlen(temp));
}

void chldwrite(int fd)
{
    while (1)
    {
        WAIT_PARENT();
        towrite += 2;
        writenum(fd, chldinfo, towrite);
        TELL_PARENT(getppid());
    }
}   

void prntwrite(int fd, pid_t pid)
{
    while (1)
    {
        writenum(fd, prntinfo, towrite);
        towrite += 2;
        TELL_CHILD(pid);
        WAIT_CHILD();
    }
}


int main(int argc, char ** argv)
{
    init();

    int fd;
    if ((fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, S_IRWXU)) < 0)
    {
        error("open error");
    }

    if (writenum(fd, prntinfo, towrite) < 0)
    {
        error("write error");
    }

    pid_t pid;
    if ((pid = fork()) < 0)
    {
        error("fork error");
    }
    else if (pid == 0) /* Child */
    {
        chldwrite(fd);
    }
    
    towrite += 1;
    prntwrite(fd, pid);
    return 0;
}
