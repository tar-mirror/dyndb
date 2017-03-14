#include "dyndb.h"

int
dyndb_init(struct dyndb *d, int fd)
{
	d->fd=fd;
	d->lookflag=0;
	d->lookpos=0;
	d->lookhash=0;
	d->datapos=0;
	d->datalen=0;
	return 0;
}
