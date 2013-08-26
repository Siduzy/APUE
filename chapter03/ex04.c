#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h>

#define MAX_LINE 1024

void lskwrite(int filedes, off_t pos, int whence, const char * buf, int size)
{
    if ((lseek(filedes, pos, whence)) == -1 || write(filedes, buf, size) == -1)
    {
        perror("lskwrite:");
    }
}

int lskread(int filedes, off_t pos, int whence, char * buf, int size)
{
    int n = -1;
    if((lseek(filedes, pos, whence)) == -1 || (n=read(filedes, buf, size)) == -1)
    {
        perror("lskread");
    }
}
int main(int argc, char * argv[])
{
    char * filename = "file.name";
    int fd = -1;
    
    if ((fd=open(filename, O_APPEND)) == -1)
    //if ((fd=open(filename, O_APPEND | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR)) == -1)
    //if ((fd=open(filename, O_RDWR | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR)) == -1)
    //if ((fd=open(filename, O_RDONLY | O_APPEND | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR)) == -1)
    {
        perror(argv[0]);
        return -1;
    }   
    printf("file des: %d\n", fd);
    
//  char * buf = "This is the pos I want to write"; 
//  lskwrite(fd, 100, SEEK_SET, buf, strlen(buf));
//  lskwrite(fd, 100, SEEK_CUR, buf, strlen(buf));
//  lskwrite(fd, 100, SEEK_END, buf, strlen(buf));  
    char buf[MAX_LINE] = {0};
    lskread(fd, 100, SEEK_SET, buf, 10);
    lskread(fd, 100, SEEK_CUR, buf, 10);
    lskread(fd, 100, SEEK_END, buf, 10);
    printf("%s\n", buf);
    return 0;
}
