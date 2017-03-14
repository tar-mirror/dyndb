#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include "dyndb.h"

int
dyndb_create_fd(struct dyndb *dy, int fd)
{
	unsigned char buf[4096]; /* don't want to use too much stack */
	uint32 l;
	for (l=0;l<sizeof(buf);l++)
		buf[l]=0;
	l=dyndb_slotmod[0]*sizeof(uint32);
	while (l) {
		int32 written;
		written=dyndb_write(fd, buf, l > sizeof(buf) ? sizeof(buf): l);
		if (-1==written)
			return -1;
		l-=written;
	}
	dyndb_init(dy,fd);
	return fd;
}


int
dyndb_create_name(struct dyndb *dy, const char *fname, int mode)
{
	int fd;
	int r;
	fd=open(fname,O_RDWR|O_CREAT|O_TRUNC,mode);
	if (fd==-1) return -1;
	r=dyndb_create_fd(dy,fd);
	if (r!=-1)
		return r;
	r=errno;
	close(fd);
	unlink(fname);
	errno=r;
	return -1;
}
