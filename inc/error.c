#include "error.h"
#include <stdio.h>
#include <errno.h>

void error(const char * info)
{
	fprintf(stderr, info, strerror(errno));
	_exit(0);
}
