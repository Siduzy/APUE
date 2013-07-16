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


int
fiemap_copy (int src_fd, int dest_fd)
{
  int last = 0;
  unsigned int i;
  int return_val = 1;
  char fiemap_buf[4096] = "";
  size_t optimal_buf_size = 1024;
  unsigned fiemap_flags = 0;
  struct fiemap *fiemap = (struct fiemap *)fiemap_buf;
  struct fiemap_extent *fm_ext = &fiemap->fm_extents[0];
  unsigned count = (sizeof(fiemap_buf) - sizeof(*fiemap)) /
                    sizeof(struct fiemap_extent);

  memset (fiemap, 0, sizeof (*fiemap));

  do
    {
      fiemap->fm_start = 0ULL;
      fiemap->fm_length = FIEMAP_MAX_OFFSET;
      fiemap->fm_flags |= fiemap_flags;
      fiemap->fm_extent_count = count;

      if (ioctl (src_fd, FS_IOC_FIEMAP, (unsigned long) fiemap) < 0)
          return 0;

      if (fiemap->fm_mapped_extents == 0)
        {
          return 1;
       }
      for (i = 0; i < fiemap->fm_mapped_extents; i++)
        {
          __u64 ext_logical = fm_ext[i].fe_logical;
          __u64 ext_len = fm_ext[i].fe_length;

          if (lseek (src_fd, ext_logical, SEEK_CUR) < 0LL)
            {
              
              return_val = 0;
            }

          __u64 tot_read;
          char buf[optimal_buf_size];
          while (tot_read < ext_len)
            {
             memset (buf, 0, sizeof(buf));
             ssize_t n_read = read (src_fd, buf, optimal_buf_size);
              if (n_read < 0)
                {
#ifdef EINTR
                  if (errno == EINTR)
                    continue;
#endif
                  
                  return_val = 0;
                }

              if (n_read == 0)
                break;

              if (write (dest_fd, buf, n_read) != n_read)
                {
                  return_val = 0;
                }

              tot_read += n_read;
            }

          if (fm_ext[i].fe_flags & FIEMAP_EXTENT_LAST)
            last = 1;

          fiemap->fm_start = (fm_ext[i-1].fe_logical + fm_ext[i-1].fe_length);
        }
	
    } while (last == 0);

  return return_val;
}

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
