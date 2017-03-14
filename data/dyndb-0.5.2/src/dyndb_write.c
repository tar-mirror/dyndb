#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include "dyndb.h"

int32
dyndb_write(int fd, const unsigned char *buf, uint32 l)
{
	int32 ret=0;

	while (l) {
		int32 written;
		written=write(fd,buf,l);
		if (-1==written) {
			if (EINTR==errno) continue;
			return -1;
		}
		l-=written;
		buf+=written;
		ret+=written;
	}
	return ret;
}

