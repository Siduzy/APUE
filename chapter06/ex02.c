#include <stdio.h>
#include <shadow.h>
#include <errno.h>

int main(int argc, char **argv)
{
    struct spwd * pwd;
    setspent(); 

    while ((pwd=getspent()) != NULL)
    {
        printf("Login Name: %s Password: %s\n", 
                pwd->sp_namp,
                pwd->sp_pwdp);
    }
    if (errno != 0)
    {
        perror(argv[0]);
    }

    endspent();
    return 0;
}
