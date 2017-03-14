#include "error.h"
#include "buffer.h"
#include "stralloc.h"
#include "str.h"
#include "dyndb.h"
#include "dyndb.h"
#include "bailout.h"
#include "uogetopt.h"

static uogetopt_t myopts[]={
{0,"",UOGO_MINARGS | UOGO_HIDDEN,0, 2 , 0,0},
{0,"",UOGO_MAXARGS | UOGO_HIDDEN,0, 2 , 0,0},
{0,0,0,0,0,0,0}
};
static struct dyndb dy;

int main(int argc, char **argv)
{
	uint32 len;
	uint32 pos;
	bailout_progname(argv[0]);
	flag_bailout_fatal_begin=3;

    uogetopt(flag_bailout_log_name ,PACKAGE,VERSION,
		&argc, argv, uogetopt_out,
		"usage: dyndbget KEY <DYNDB",
		myopts,
		"  This program prints the first record in DYNDB with the key KEY.\n"
		"  Report bugs to dyndb@lists.ohse.de"
	);
	dyndb_init(&dy,0);

	switch(dyndb_lookup(&dy,(unsigned char *)argv[1],str_len(argv[1]))) {
	case -1: xbailout(111,errno,"failed to find record. dyndb_seek",0,0,0);
	case 0: return (100);
	case 1: break;
	}
	len=dyndb_datalen(&dy);
	pos=dyndb_datapos(&dy);
	if (-1==dyndb_seek(0,pos))
		xbailout(111,errno,"failed to seek",0,0,0);

	while (len) {
		int l;
		char *p;

		l = buffer_feed(buffer_0);
		if (l==-1) xbailout(111,errno,"failed to read",0,0,0);
		if (l==0) xbailout(100,0,"unexpected end of file",0,0,0);
		if (l>(int) len) l=len;
		p = buffer_peek(buffer_0);
		if (-1==buffer_put(buffer_1,p,l)) 
			xbailout(111,errno,"failed to write",0,0,0);
		buffer_seek(buffer_0,l);
		len-=l;
	}
	if (-1==buffer_flush(buffer_1)) 
		xbailout(111,errno,"failed to write",0,0,0);
	return 0;
}
