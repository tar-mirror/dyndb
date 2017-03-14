#include <stdlib.h> /* abort */
#include "dyndb.h"

int dyndb_walkstart(struct dyndb *dy)
{
	if (-1==dyndb_seek(dy->fd,0)) return -1;
	dy->wlevel=0;
	dy->wi[0]=0;
	dy->tpos[0]=0;
	dy->wchainpos=0;
	return 0;
}

int 
dyndb_walknext(struct dyndb *dy)
{
	unsigned char conv4[DYNDB_HEAD_SIZE];
	unsigned char conv[sizeof(uint32)];
	uint32 ui32;
  newlevel:
	while (1) { /* level */
		uint32 tpos=dy->tpos[dy->wlevel];
		/* table */
		while (dy->wi[dy->wlevel] < dyndb_slotmod[dy->wlevel]) { 
			/* element of table, is either a table or record */
			uint32 pos;
			if (dy->wchainpos) {
				ui32=dy->wchainpos;
				goto founddata;
			}
			pos=tpos+dy->wi[dy->wlevel] * (sizeof(uint32));
			if (-1==dyndb_seek(dy->fd,pos)) return -1;
			if (-1==dyndb_read(dy->fd, conv, sizeof(conv))) return -1;
			ui32=dyndb_fromdisk(conv);
			while (ui32) {
				uint32 nextpos;
				uint32 keylen;
				uint32 datalen;
				if (DYNDB_ISTABLE(ui32) 
					&& DYNDB_TABLEPOS(ui32)==tpos)
					break;
				if (DYNDB_ISTABLE(ui32)) {
					dy->wlevel++;
					dy->wi[dy->wlevel]=0;
					dy->tpos[dy->wlevel]=DYNDB_TABLEPOS(ui32);
					dy->wchainpos=0;
					goto newlevel;
				}
			  founddata:
				pos=ui32;
				/* found data */
				if (-1==dyndb_seek(dy->fd,pos)) return -1;
				if (-1==dyndb_read(dy->fd, conv4, sizeof(conv4))) return -1;
				nextpos=dyndb_fromdisk(conv4);
				if (nextpos==ui32) abort();
				keylen=dyndb_fromdisk(conv4+DYNDB_OFF_KEYLEN);
				datalen=dyndb_fromdisk(conv4+DYNDB_OFF_DATALEN);
				if (keylen || datalen) { /* if not deleted */
					dy->keypos=pos+DYNDB_HEAD_SIZE;
					dy->keylen=keylen;
					dy->datapos=pos+DYNDB_HEAD_SIZE+keylen;
					dy->datalen=datalen;
					if (DYNDB_ISTABLE(nextpos)) {
						/* this is our table */
						dy->wchainpos=0;
						dy->wi[dy->wlevel]++;
					} else
						dy->wchainpos=nextpos;
					return 1;
				}
				ui32=nextpos;
			}	
			dy->wi[dy->wlevel]++;
			dy->wchainpos=0;
		}
		/* table done */
		if (0==dy->wlevel)
			return 0;
		dy->wlevel--;
	}
	return 0;
}
