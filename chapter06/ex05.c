#include <time.h>
#include <stdio.h>
#include <sys/time.h>

#define TIME_BUFSIZ 100 

int main(int argc, char ** argv)
{
    time_t t;
    if (time(&t) == -1)
    {
        perror(argv[0]);
    }

    struct tm * ptm = NULL;
    if ((ptm = localtime(&t)) == NULL)
    {
        perror(argv[0]);
        return -1;
    }

    char buf [TIME_BUFSIZ] = {0};

    strftime(buf, TIME_BUFSIZ, "%Y年 %m月 %d日 %A %T %Z", ptm);

    printf("%s\n", buf);
    return 0;
}
