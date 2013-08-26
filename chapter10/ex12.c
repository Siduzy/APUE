#include "../inc/my_apue.h"


#define SUPERBUFSIZE 50000000

static void sig_alarm(int signo)
{
    printf("receiving signal %d", signo);
    system("ls -l");
    sleep(3);
}

int main(int argc, char * argv[])
{
    const char * towrite = "write.to";
    FILE * fw;
    if((fw = fopen(towrite, "w+")) == NULL)
    {
        error("fopen error");
    }

    char * pBuf = (char *)malloc(SUPERBUFSIZE);
    setbuf(fw, pBuf);

    if (signal(SIGALRM, sig_alarm) < 0)
    {
        error("signal error");
    }

    alarm(1);
    
    char buf[] = "abcdefjhijklmnopqrstuvwxyz";
    fwrite(buf, sizeof(char), sizeof buf, fw);  
    
    sleep(1);   
    return 0;
}


