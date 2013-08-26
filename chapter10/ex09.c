#include "../inc/my_apue.h"

struct
{
    int signumber; 
    char signame[8];
}sig_id[] = {

    {SIGINT, "SIGINT"},
    {SIGQUIT, "SIGQUIT"},
    {SIGUSR1, "SIGUSR1"},
    {SIGALRM, "SIGALARM"}

};

void pr_mask(const char *str)
{
    sigset_t sigset;
    if (sigprocmask(0, NULL, &sigset) < 0)
    {
        error("sigprocmask");
    }
    
    int sig, nsigs = sizeof(sig_id) / sizeof(sig_id[0]);

    for (sig = 0; sig < nsigs; ++sig)
        if (sigismember(&sigset, sig_id[sig].signumber))
            printf("%s ", sig_id[sig].signame);
}

int main(int argc, char * argv[0])
{
    sigset_t sigset;
    if (sigprocmask(0, NULL, &sigset) < 0)
    {
        error("sigpromask");
    }

    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);

    sigprocmask(SIG_BLOCK, &sigset, NULL);
    pr_mask(NULL);
    return 0;
}
