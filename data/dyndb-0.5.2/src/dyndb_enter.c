#include <errno.h>
#include "dyndb.h"

/* avoid memset incompatibilities inside library */
static void 
init_mem(void *v,uint32 l)
{
	char *s=v;
	while (l--)
		s[l]=0;
}


static int
resolve_collisions(struct dyndb *dy, const int level, uint32 startofchain, 
	uint32 endofchain, uint32 newtabpos)
{
	uint32 newtab[DYNDB_MAXLLTABSIZE];
	unsigned char conv[sizeof(uint32)];
	uint32 h;
	uint32 aktpos,firstpos;
	int count=0;

	/* terminate the end of the chain */
	if (-1==dyndb_seek(dy->fd,endofchain)) return -1;
	dyndb_todisk(conv,DYNDB_TOTABLE(newtabpos));
	if (-1==dyndb_write(dy->fd, conv, sizeof (conv))) return -1;

	/* get first list element */
	if (-1==dyndb_seek(dy->fd,startofchain)) return -1;
	if (-1==dyndb_read(dy->fd, conv, sizeof (conv))) return -1;
	aktpos=dyndb_fromdisk(conv);
	init_mem(newtab,sizeof(newtab));
	firstpos=aktpos;

	/* pass 1: populate newtab */
	while (1) {
		int i;
		unsigned char conv2[DYNDB_OFF_KEYLEN];
		uint32 nextpos;
		if (count++==DYNDB_MAXLLTABSIZE) {errno=EINVAL; return -1; }
		if (DYNDB_ISTABLE(aktpos)) break;
		/* read next positions and hash */
		if (-1==dyndb_seek(dy->fd,aktpos)) return -1;
		if (-1==dyndb_read(dy->fd,conv2,sizeof(conv2))) return -1;
		nextpos=dyndb_fromdisk(conv2);
		h=dyndb_fromdisk(conv2+DYNDB_OFF_HASH);
		for (i=0;i<level;i++)
			h/=dyndb_slotmod[i];
		h%=dyndb_slotmod[level];
		/* update new table, with the first record with this hash:
		 * we can detect collisions this way 
		 */
		 if (0==newtab[h]) {
			if (-1==dyndb_seek(dy->fd,newtabpos+h*sizeof(uint32))) 
				return -1;
			dyndb_todisk(conv,aktpos);
			if (-1==dyndb_write(dy->fd, conv, sizeof (conv))) return -1;
			newtab[h]=aktpos;
		}
		aktpos=nextpos;
	}

	/* update upper level table */
	if (-1==dyndb_seek(dy->fd,startofchain)) return -1;
	dyndb_todisk(conv,DYNDB_TOTABLE(newtabpos));
	if (-1==dyndb_write(dy->fd, conv, sizeof (conv))) return -1;

	/* second pass: create collision chains */
	aktpos=firstpos;
	count=0;

	while (1) {
		int i;
		unsigned char conv2[DYNDB_OFF_KEYLEN];
		uint32 nextpos;
		if (DYNDB_MAXLLTABSIZE==count++) {errno=EINVAL; return -1; }
		if (DYNDB_ISTABLE(aktpos)) break;
		/* read next positions and hash */
		if (-1==dyndb_seek(dy->fd,aktpos)) return -1;
		if (-1==dyndb_read(dy->fd,conv2,sizeof(conv2))) return -1;
		nextpos=dyndb_fromdisk(conv2);
		h=dyndb_fromdisk(conv2+DYNDB_OFF_HASH);
		for (i=0;i<level;i++)
			h/=dyndb_slotmod[i];
		h%=dyndb_slotmod[level];
		if (newtab[h]==aktpos) {
			aktpos=nextpos;
			continue;
		} else {
			uint32 cpos=newtab[h];
			uint32 save=cpos; /* keep compiler quiet */
			while (cpos) {
				unsigned char convb[DYNDB_OFF_KEYLEN];
				uint32 h2;
				if (cpos==aktpos) break;
				if (-1==dyndb_seek(dy->fd,cpos)) return -1;
				if (-1==dyndb_read(dy->fd,convb,sizeof(convb))) return -1;
		 		h2=dyndb_fromdisk(convb+DYNDB_OFF_HASH);
				for (i=0;i<level;i++)
					h2/=dyndb_slotmod[i];
				h2%=dyndb_slotmod[level];
				if (h!=h2) break;
				save=cpos;
		 		cpos=dyndb_fromdisk(convb);
				if (DYNDB_ISTABLE(cpos)) break;
			}
			if (-1==dyndb_seek(dy->fd,save)) return -1;
			dyndb_todisk(conv,aktpos);
			if (-1==dyndb_write(dy->fd, conv, sizeof (conv))) return -1;
		}
		aktpos=nextpos;
	}

	/* third pass: set the "next" pointer of last collision chain element to 
	 * the new table */
	aktpos=firstpos;
	{
		for (h=0;h<dyndb_slotmod[level];h++) {
			uint32 cpos=newtab[h];
			uint32 save=0; /* keep compiler quiet */
			if (!cpos) continue;
			while (cpos) {
				int i;
				unsigned char convb[DYNDB_OFF_KEYLEN];
				uint32 h2;
				if (-1==dyndb_seek(dy->fd,cpos)) return -1;
				if (-1==dyndb_read(dy->fd,convb,sizeof(convb))) return -1;
		 		h2=dyndb_fromdisk(convb+DYNDB_OFF_HASH);
				for (i=0;i<level;i++)
					h2/=dyndb_slotmod[i];
				h2%=dyndb_slotmod[level];
				if (h!=h2) break;
				save=cpos;
		 		cpos=dyndb_fromdisk(convb);
				if (DYNDB_ISTABLE(cpos)) break;
			}
			if (-1==dyndb_seek(dy->fd,save)) return -1;
			dyndb_todisk(conv,DYNDB_TOTABLE(newtabpos));
			if (-1==dyndb_write(dy->fd, conv, sizeof (conv))) return -1;
		}
	}
	return 0;
}

int 
dyndb_enterstart(struct dyndb *dy)
{
	unsigned char conv[DYNDB_HEAD_SIZE];
	int32 ret;
	ret=dyndb_seekend(dy->fd);
	if (-1==ret) return -1;
	dy->epos=ret;
	dy->ehash=dyndb_hashstart();
	dy->ekeylen=0;
	dy->edatalen=0;
	dyndb_todisk(conv+DYNDB_OFF_NEXT,0); /* next pointer */
	dyndb_todisk(conv+DYNDB_OFF_HASH,0); /* hash */
	dyndb_todisk(conv+DYNDB_OFF_KEYLEN,0);
	dyndb_todisk(conv+DYNDB_OFF_DATALEN,0);
	if (-1==dyndb_write(dy->fd, conv,sizeof(conv))) return -1;
	return 0;
}
int dyndb_enteraddkey(struct dyndb *dy,const unsigned char *k,uint32 kl)
{
	if (-1==dyndb_write(dy->fd, k,kl)) return -1;
	dy->ehash=dyndb_hashadd(dy->ehash,k,kl);
	dy->ekeylen+=kl;
	return 0;
}
int dyndb_enteradddata(struct dyndb *dy,const unsigned char *d,uint32 dl)
{
	if (-1==dyndb_write(dy->fd, d,dl)) return -1;
	dy->edatalen+=dl;
	return 0;
}
int dyndb_enterfinish(struct dyndb *dy)
{
	uint32 hash;
	uint32 slot;
	uint32 tpos;
	uint32 lastpos;
	int level=0;
	int32 ipos;
	unsigned char conv4[DYNDB_HEAD_SIZE];
	unsigned char conv[sizeof(uint32)];

	ipos=dyndb_seek(dy->fd,dy->epos);
	if (-1 == ipos) return -1;
	dyndb_todisk(conv4+DYNDB_OFF_NEXT,0);
	dyndb_todisk(conv4+DYNDB_OFF_HASH,dy->ehash);
	dyndb_todisk(conv4+DYNDB_OFF_KEYLEN,dy->ekeylen);
	dyndb_todisk(conv4+DYNDB_OFF_DATALEN,dy->edatalen);
	if (-1==dyndb_write(dy->fd,conv4,sizeof(conv4))) return -1;

	tpos=0;
	hash=dy->ehash;
	/* find the hash slot */
	while (1) {
		uint32 ui32;
		uint32 startofchain;
		slot=hash % dyndb_slotmod[level];
		hash=hash / dyndb_slotmod[level];
		/* table read */
		startofchain=sizeof(uint32)*slot+tpos;
		if (-1==dyndb_seek(dy->fd,startofchain)) return -1;
		if (-1==dyndb_read(dy->fd, conv, sizeof(uint32))) return -1;
		ui32=dyndb_fromdisk (conv);
		if (!ui32) {lastpos=startofchain; break;} /* empty slot in the table */
		if (DYNDB_ISTABLE(ui32) && DYNDB_TABLEPOS(ui32)==tpos) {
			lastpos=startofchain; 
			break;
		}
		if (!DYNDB_ISTABLE(ui32)) {	/* data entry */
			int collisioncount=0;
			int32 newtab;
			uint32 newtablen;
			unsigned char buf[4096];
			while (1) {
				collisioncount++;
				if (-1==dyndb_seek(dy->fd,ui32)) return -1;
				if (-1==dyndb_read(dy->fd, conv, sizeof(conv))) return -1;
				lastpos=ui32;
				ui32=dyndb_fromdisk (conv);
				if (0==ui32) break;
				if (DYNDB_ISTABLE(ui32) && DYNDB_TABLEPOS(ui32)==tpos) break;
				/* we can never find another table here - but the 
				 * seek function can (during an update).
				 */
			} 
			if (!dyndb_slotmod[level+1] 
				|| collisioncount < DYNDB_MAXCOLLISIONS) break;
			level++;
			/* need to create a next level table */
			newtab=dyndb_seekend(dy->fd);
			if (-1 == newtab) return -1;

			init_mem(buf,sizeof(buf));
			newtablen=dyndb_slotmod[level]*sizeof(uint32);
			while (newtablen) {
				int32 written;
				written=dyndb_write(dy->fd, buf, 
					newtablen > sizeof(buf) ? sizeof(buf): newtablen);
				if (-1==written) return -1;
				newtablen-=written;
			}
			if (-1==resolve_collisions(dy,level,startofchain,
					lastpos,newtab))
				return -1;
			tpos=newtab;
			continue;
		} 
		/* next level table */
		level++;
		tpos=DYNDB_TABLEPOS(ui32);
	}

	/* fix next pointer */
	if (-1==dyndb_seek(dy->fd,lastpos)) return -1;
	dyndb_todisk(conv,dy->epos);
	if (-1==dyndb_write(dy->fd, conv,sizeof(uint32))) return -1;
	if (-1==dyndb_seek(dy->fd,dy->epos)) return -1;
	dyndb_todisk(conv,DYNDB_TOTABLE(tpos));
	if (-1==dyndb_write(dy->fd, conv,sizeof(uint32))) return -1;
	return 0;
}

int 
dyndb_enter(struct dyndb *dy, const unsigned char *key, uint32 keylen,
	const unsigned char *data, uint32 datalen)
{
	if (-1==dyndb_enterstart(dy)) return -1;
	if (-1==dyndb_enteraddkey(dy,key,keylen)) return -1;
	if (-1==dyndb_enteradddata(dy,data,datalen)) return -1;
	return dyndb_enterfinish(dy);
}
