#include <sys/utsname.h>
#include <stdio.h>

int main(int argc, char ** argv)
{
    struct utsname utsname;
    if (uname(&utsname) == -1)
    {
        perror(argv[0]);
        return -1;
    }
    
    printf("sysname:%s nodename:%s release:%s version:%s machine:%s\n",
            utsname.sysname,
            utsname.nodename,
            utsname.release,
            utsname.version,
            utsname.machine);

    return 0;
}

