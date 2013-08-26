#include <unistd.h>
#include <fcntl.h>

#define RWX_ALL (S_IRWXU | S_IRWXG | S_IRWXO)
int main(int argc, char *argv[])
{
    char *filename = "sparse.file";
    int fd = -1;
    umask(0111);
    if ((fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, RWX_ALL)) == -1)
    {
        if ((fd = open(filename, O_WRONLY | O_TRUNC)) == -1)
        {
            perror(argv[0]);
            return -1;
        }
    }
        
    char *str = "Simply write something";

    write (fd, str, strlen(str));

    if (lseek(fd, 100, SEEK_SET) == -1)
    {
        perror(argv[0]);
        return -2;
    }
    
    if (write(fd, str, strlen(str)) < 0)
    {
        perror(argv[0]);
        return -3;
    }

    if (lseek(fd, 1000, SEEK_CUR) == -1)
    {
        perror(argv[0]);
        return -4;
    }
    int i = 10;
    while (--i > 0)
    {
        write(fd, str, strlen(str));
    }
    return 0;
}
