#include <unistd.h>
#include <stdlib.h> /* malloc */
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "dyndb.h"

static int 
walk_table(struct dyndb *dy, off_t tpos, const int level,
	int (*callback)(uint32 pos,
					void *key, uint32 keylen, 
					void *data, uint32 datalen))
{
	static int ps=0;
	unsigned int i;
	unsigned char conv4[DYNDB_HEAD_SIZE];
	uint32 ui32;
	unsigned char *map;
	void *mp;
	uint32 ml;
	uint32 mpos;
	uint32 epos;
	if (!ps)
		ps=getpagesize();
	mpos=tpos - (tpos%ps); /* ALIGN */
	epos=tpos+sizeof(uint32) * dyndb_slotmod[level];
	for (ml=mpos;ml<epos;)
		ml+=ps;

	mp=mmap(0,ml,PROT_READ,MAP_SHARED, dy->fd, mpos);
	if ((void *)-1==mp) 
		return -1;
	map=((unsigned char *)mp)+tpos%ps;

	for (i=0;i<dyndb_slotmod[level];i++) {
		ui32=dyndb_fromdisk(map+sizeof(uint32)*i);
		while (ui32) {
			off_t pos;
			uint32 nextpos;
			uint32 keylen;
			uint32 datalen;
			uint32 oldhash;
			if (DYNDB_ISTABLE(ui32) 
				&& DYNDB_TABLEPOS(ui32)==(uint32) tpos)
				break;
			if (DYNDB_ISTABLE(ui32)) {
				int ret;
				ret=walk_table(dy,DYNDB_TABLEPOS(ui32),level+1,callback);
				if (ret) return ret;
				break;
			}
			pos=ui32;
			/* found data */
			if (-1==dyndb_seek(dy->fd,pos)) return -1;
			if (-1==dyndb_read(dy->fd, conv4, sizeof(conv4))) return -1;
			nextpos=dyndb_fromdisk(conv4);
			if (nextpos==ui32) abort();
			oldhash=dyndb_fromdisk(conv4+DYNDB_OFF_HASH);
			keylen=dyndb_fromdisk(conv4+DYNDB_OFF_KEYLEN);
			datalen=dyndb_fromdisk(conv4+DYNDB_OFF_DATALEN);
			if (keylen || datalen) {
				unsigned char *key, *data;
				int ret;
				key=(unsigned char *)malloc(keylen+datalen);
				if (!key) {errno=ENOMEM; return -1; }
				data=key+keylen;
				if (-1==dyndb_read(dy->fd, key,keylen+datalen)) 
					{  free(key); return -1;}
				ret=callback(pos,key,keylen,data,datalen);
				if (ret) 
					{ free(key); return ret; }
				free(key);
			}
			ui32=nextpos;
		}	
	}
	munmap(mp,ml);
	return 0;
}

int dyndb_fwalk(struct dyndb *dy, int (*callback)(uint32 pos,
    void *key, uint32 keylen,
    void *data, uint32 datalen))
{
	if (-1==dyndb_seek(dy->fd,0)) return -1;
	return walk_table(dy,0,0,callback);
}

