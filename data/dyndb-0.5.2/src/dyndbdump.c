#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h> /* malloc */
#include <stdio.h> /* rename */
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dyndb.h"
#include "fmt.h"
#include "buffer.h"
#include "bailout.h"
#include "uogetopt.h"
#include "str.h"
static struct dyndb dy;

static int
prt(void)
{
	char nb[FMT_ULONG];
	uint32 kp,kl;
	uint32 dp,dl;
	kp=dyndb_keypos(&dy);
	kl=dyndb_keylen(&dy);
	dp=dyndb_datapos(&dy);
	dl=dyndb_datalen(&dy);
	/* XXX assumes data follows key */
	if (-1==dyndb_seek(dy.fd,kp))
		xbailout(111,errno,"failed to seek",0,0,0);
#define X(y,z) if (-1==(buffer_put(buffer_1,(const char *)y,z))) \
	xbailout(111,errno,"failed to write",0,0,0);
	X("+",1);
	X(nb, fmt_ulong(nb,kl));
	X(",",1);
	X(nb, fmt_ulong(nb,dl));
	X(":",1);
	while (kl) {
		unsigned char buf[1024];
		int32 r;
		r=dyndb_read(dy.fd,buf,kl>sizeof(buf) ? sizeof(buf) : kl);
		if (r==-1)
			xbailout(111,errno,"failed to read",0,0,0);
		X(buf,r);
		kl-=r;
	}
	X("->",2);
	while (dl) {
		unsigned char buf[1024];
		int32 r;
		r=dyndb_read(dy.fd,buf,dl>sizeof(buf) ? sizeof(buf) : dl);
		if (r==-1)
			xbailout(111,errno,"failed to read",0,0,0);
		X(buf,r);
		dl-=r;
	}
	X("\n",1);
	buffer_flush(buffer_1);
	return 0;
}

static int opt_n=0;
static uogetopt_t myopts[]={
{'n',"no-newline",UOGO_FLAG , &opt_n, 1 , 
"Don't write the final newline.\n",0},
{0,"",UOGO_MAXARGS | UOGO_HIDDEN,0, 1 , 0,0},
{0,0,0,0,0,0,0}
};


int
main(int argc, char **argv) 
{
	bailout_progname(argv[0]);
	flag_bailout_fatal_begin=3;
    uogetopt(flag_bailout_log_name ,PACKAGE,VERSION,
	        &argc, argv, uogetopt_out,
			"usage: dyndbdump [options] <DYNDB",
			myopts,
			"  This program dumps the content of DYNDB.\n"
			"  Report bugs to dyndb@lists.ohse.de"
		);
	dyndb_init(&dy,0);
	dyndb_walkstart(&dy);
	while (1) {
		int r;
		r=dyndb_walknext(&dy);
		if (-1==r)
			xbailout(111,errno,"failed to dump database. dyndb_walknext",0,0,0);
		if (!r)
			break;
		prt();
	}

	if (!opt_n)
		X("\n",1);
	
	if (-1==buffer_flush(buffer_1)) xbailout(111,errno,"failed to write",0,0,0);
	exit(0);
}

