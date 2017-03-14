#include <errno.h>
#include <stdlib.h> /* abort */
#include "dyndb.h"

static int keymatch(struct dyndb *dy, const unsigned char *key, uint32 keylen)
{
	unsigned char buf[128]; /* larger than a typical message-id */
	while (keylen) {
		uint32 l,i;
		l=sizeof(buf);
		if (l>keylen) l=keylen;
		if (-1==dyndb_read(dy->fd,buf,l)) return -1;
		for (i=0;i<l;i++)
			if (key[i]!=buf[i]) return 0;
		keylen-=l;
		key+=l;
	}
	return 1;
}

void 
dyndb_lookupstart(struct dyndb *dy)
{
	dy->lookflag=0;
}

static int 
dyndb_lookupfirst(struct dyndb *dy, const unsigned char *key, uint32 keylen)
{
	uint32 slot;
	unsigned char conv[sizeof(uint32)];
	unsigned char conv4[DYNDB_HEAD_SIZE];
	uint32 tpos=0;
	uint32 rpos;
	uint32 hash;
	uint32 ohash;
	int level=0;

	hash=dyndb_hash(key,keylen);
	dy->lookhash=hash;
	ohash=hash;

	while (1) {
		uint32 pos;
		uint32 ui32;
		slot=hash % dyndb_slotmod[level];
		hash=hash / dyndb_slotmod[level];
		/* table read */
		rpos=sizeof(uint32)*slot+tpos;
		if (-1==dyndb_seek(dy->fd,rpos)) return -1;
		if (-1==dyndb_read(dy->fd, conv, sizeof(uint32))) return -1;
		ui32=dyndb_fromdisk (conv);
		while (1) {
			uint32 thash,tkeylen;
			uint32 nextpos;
			if (!ui32) return 0; /* not found */
			if (DYNDB_ISTABLE(ui32) && DYNDB_TABLEPOS(ui32)==tpos)
				return 0; /* ptr back to table */
			if (DYNDB_ISTABLE(ui32)) {
				tpos=DYNDB_TABLEPOS(ui32);
				level++;
				break;
			}
			pos=ui32;
			/* found data */
			if (-1==dyndb_seek(dy->fd,pos)) return -1;
			if (-1==dyndb_read(dy->fd, conv4, sizeof(conv4))) return -1;
			nextpos=dyndb_fromdisk(conv4);
			if (nextpos==ui32) abort();
			ui32=nextpos;
			thash=dyndb_fromdisk(conv4+DYNDB_OFF_HASH);
			if (thash!=ohash) continue;
			tkeylen=dyndb_fromdisk(conv4+DYNDB_OFF_KEYLEN);
			if (keylen!=tkeylen) continue;
			switch(keymatch(dy,key,keylen)) {
			case 0: break;
			case -1: return -1;
			case 1: 
				dy->lookpos=pos;
				dy->datapos=pos+DYNDB_HEAD_SIZE+keylen;
				dy->datalen=dyndb_fromdisk(conv4+DYNDB_OFF_DATALEN);
				return 1;
			}
		}
	}
}
int 
dyndb_lookupnext(struct dyndb *dy, const unsigned char *key, uint32 keylen)
{
	unsigned char conv4[DYNDB_HEAD_SIZE];
	uint32 nextpos;
	uint32 pos;

	if (!dy->lookflag) {
		int r;
		r=dyndb_lookupfirst(dy,key,keylen);
		if (1!=r)
			return r;
		dy->lookflag=1;
		return r;
	}
	/* follow collision chain */
	/* dy->lookpos point to the record found by the last lookup */
	if (-1==dyndb_seek(dy->fd,dy->lookpos)) return -1;
	if (-1==dyndb_read(dy->fd, conv4, sizeof(uint32))) return -1;
	nextpos=dyndb_fromdisk (conv4+0);
	pos=dy->lookpos;
	while (1) {
		uint32 thash;
		uint32 tkeylen;
		if (!nextpos) return 0; /* not next record */
		if (DYNDB_ISTABLE(nextpos)) return 0; /* ptr back to table */

		/* found data */
		pos=nextpos;
		if (-1==dyndb_seek(dy->fd, pos)) return -1;
		if (-1==dyndb_read(dy->fd, conv4, sizeof(conv4))) return -1;

		nextpos=dyndb_fromdisk(conv4);
		if (pos==nextpos) abort();
		thash=dyndb_fromdisk(conv4+DYNDB_OFF_HASH);
		if (thash!=dy->lookhash) continue;
		tkeylen=dyndb_fromdisk(conv4+DYNDB_OFF_KEYLEN);
		if (keylen!=tkeylen) continue;
		switch(keymatch(dy,key,keylen)) {
		case 0: break;
		case -1: return -1;
		case 1: 
			dy->lookpos=pos;
			dy->datapos=pos+DYNDB_HEAD_SIZE+keylen;
			dy->datalen=dyndb_fromdisk(conv4+DYNDB_OFF_DATALEN);
			return 1;
		}
	}
}
int 
dyndb_lookup(struct dyndb *dy, const unsigned char *key, uint32 keylen)
{
	dyndb_lookupstart(dy);
	return dyndb_lookupnext(dy, key,keylen);
}
