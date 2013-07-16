/*
 * Use to setvbuf to implement setbuf
 *
 */

#include <stdio.h>
#include <unistd.h>


void my_setbuf(FILE *restrict fp, char * restrict buf)
{
	int fd = fileno(fp);
	
	if (buf == NULL)
	{
		setvbuf(fp, buf, _IONBF);
		return ;
	}

	int mode = 0;
	if (isatty(fd))
	{
		mode = fd==STDERR_FILENO ? _IONBF : _IOLBF;
	}
	else
	{
		mode = _IOFBF;
	}
	
	setvbuf(fp, buf, mode, BUFSIZ);
}

	
