#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include "dyndb.h"

int32 
dyndb_read(int fd,unsigned char *buf,const uint32 len)
{
	uint32 got=0;
	while (len!=got) {
		int r;
		r=read(fd,buf+got,len-got);
		if (-1==r && EINTR==errno)
			continue;
		if (-1==r) return -1;
		if (0==r) {errno=EIO; return -1; }
		buf+=r;
		got+=r;
	}
	return got;
}
