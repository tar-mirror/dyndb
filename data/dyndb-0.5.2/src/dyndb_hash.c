#include "dyndb.h"

unsigned int dyndb_slotmod[]=DYNDB_SLOTMODS;

/* stolen from cdb by daniel bernstein */

uint32 dyndb_hashstart(void) { return 5381; }
uint32 
dyndb_hashadd(uint32 h, const unsigned char *buf,uint32 len) 
{
	while (len) {
		--len;
		h += (h << 5);
		h ^= *buf++;
	}
	return h;
}
uint32
dyndb_hash(const unsigned char *buf,uint32 len)
{
	uint32 h;
	h=5381; /* dyndb_hashstart(); */
	return dyndb_hashadd(h,buf,len);
}

