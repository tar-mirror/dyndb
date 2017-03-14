#include "dyndb.h"

void
dyndb_todisk (unsigned char *buf, uint32 num)
{
	buf[0]=num&0xff;
	num >>= 8;
	buf[1]=num&0x0ff;
	num >>= 8;
	buf[2]=num&0x0ff;
	num >>= 8;
	buf[3]=num&0x0ff;
}
