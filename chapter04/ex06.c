/*
 * Copy sparse file
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/fiemap.h>
#include <linux/fs.h>
#include <linux/types.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int error(const char * info)
{
    return fprintf(stderr, info, strerror(errno));
}
//-------------------------------------------------------------------

void print_fe(struct fiemap_extent * pfmpex)
{
    char *pstr = NULL;
    switch (pfmpex->fe_flags)
    {/*
    case FIEMAP_EXTENT_HOLE:
        pstr = "hole";
        break;*/
    case FIEMAP_EXTENT_UNWRITTEN:
        pstr = "unwritten";
        break;
    default:
        pstr = "data";
    }
    printf("#\tLogical        Physical        Length        Flags\n");
    printf("%-16.16llx %-16.16llx %-16.16llx %-4.4x\n",
            pfmpex->fe_logical,
            pfmpex->fe_physical,
            pfmpex->fe_length,
            pfmpex->fe_flags);
}
//------------------------------------------------------------------

int fiemap_copy(int src_fd, int dst_fd)
{
    struct fiemap * pfm;
    if ((pfm = (struct fiemap *)malloc(sizeof(struct fiemap))) == NULL)
    {
        error("malloc:");
        return errno;
    }

    pfm->fm_start = 0;
    pfm->fm_length = FIEMAP_MAX_OFFSET;
    pfm->fm_flags = 0;
    pfm->fm_extent_count = 0;

    if (ioctl(src_fd, FS_IOC_FIEMAP, pfm) == -1)
    {
        error("ioctl:");
        return -1;
    }
    
    unsigned int extent_size = sizeof(struct fiemap_extent) * pfm->fm_mapped_extents;
    
    if ((pfm = (struct fiemap *)realloc(pfm, sizeof(struct fiemap) + extent_size)) == NULL)
    {
        error("realloc");
        return -1;
    }
        
    memset(pfm->fm_extents, 0, extent_size);
    pfm->fm_extent_count = pfm->fm_mapped_extents;
    pfm->fm_mapped_extents = 0;
    
    if (ioctl(src_fd, FS_IOC_FIEMAP, pfm) == -1)
    {
        error("ioctl(2)");
        return  -1;
    }
    
    struct fiemap_extent * pfe = pfm->fm_extents;
    int i;
    for (i = 0; i < pfm->fm_mapped_extents; ++i)
    {
        print_fe(&pfe[i]);
        if (lseek(src_fd, pfe[i].fe_logical, SEEK_CUR) < 0LL)
        {
            error("leek");
            return -1;
        }

        char buf[BUF_SIZE] = {0};
        __u64 tot_read = 0;
        ssize_t nread = 0;
        
        while (tot_read < pfe[i].fe_length)
        {
            if ((nread = read(src_fd, buf,  sizeof(buf))) < 0)
            {
                if (errno == EINTR)
                    continue;

                error("read :");
                return -1;
            }
                            
            if (nread == 0)
            {   
                break;
            }

            if (write(dst_fd, buf, nread) != nread)
            {
                error("write");
                return -1;
            }

            tot_read += nread;
            nread = 0;
        }

        if (pfe[i].fe_flags & FIEMAP_EXTENT_LAST)
        {
            break;
        }

    }

            
    return 0;
}
//---------------------------------------------------------------------

int main(int argc, char ** argv)
{

    int fd1 = -1;
    if ((fd1 = open("sparse.file", O_RDONLY)) == -1)
    {
        error("open");
        return -1;
    }

    int fd2 = -1;   
    if ((fd2 = open("cp.file", O_RDWR | O_CREAT | O_EXCL, S_IRWXU)) < 0)
    {
        error("creat");
        return -1;
    }

    fiemap_copy(fd1, fd2);

    return 0;
}
//-----------------------------------------------------------------------
