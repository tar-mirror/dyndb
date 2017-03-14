#include "dyndb.h"

uint32
dyndb_fromdisk (unsigned char *buf)
{
	return buf[0] 
	     + buf[1]*0x100UL
		 + buf[2]*0x10000UL
		 + buf[3]*0x1000000UL;
}

