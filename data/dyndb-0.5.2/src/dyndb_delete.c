#include <errno.h>
#include "dyndb.h"

/* set key and data length to zero for the record found by 
 * latest call to dyndb_lookupnext */

int 
dyndb_delete(struct dyndb *dy)
{
	unsigned char conv2[DYNDB_HEAD_SIZE-DYNDB_OFF_KEYLEN];
	if (0==dy->lookpos) {
		errno=EINVAL;
		return -1;
	}
	if (-1==dyndb_seek(dy->fd,dy->lookpos+DYNDB_OFF_KEYLEN)) return -1;
	if (-1==dyndb_read(dy->fd, conv2, sizeof(conv2))) return -1;
	if (-1==dyndb_seek(dy->fd,dy->lookpos+DYNDB_OFF_KEYLEN)) return -1;
	dyndb_todisk(conv2,0);
	dyndb_todisk(conv2+DYNDB_OFF_DATALEN-DYNDB_OFF_KEYLEN,0);
	if (-1==dyndb_write(dy->fd, conv2, sizeof(conv2))) return -1;
	return 0;
}
