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
#include "str.h"
#include "uogetopt.h"

static int
callback(uint32 pos, void *key, uint32 keylen, void *data, uint32 datalen)
{
	char nb[FMT_ULONG];
	(void) pos;
#define X(y,z) if (-1==(buffer_put(buffer_1,y,z))) \
	xbailout(111,errno,"failed to write",0,0,0);
	X("+",1);
	X(nb, fmt_ulong(nb,keylen));
	X(",",1);
	X(nb, fmt_ulong(nb,datalen));
	X(":",1);
	X(key,keylen);
	X("->",2);
	X(data,datalen);
	X("\n",1);
	return 0;
}

static int opt_n=0;
static uogetopt_t myopts[]={
{'n',"no-newline", UOGO_FLAG ,&opt_n, 1 , 
"Don't add the final newline",0},
{0,"",UOGO_MAXARGS | UOGO_HIDDEN,0, 1 , 0,0},
{0,0,0,0,0,0,0}
};
static struct dyndb dy;

int
main(int argc, char **argv) 
{
	bailout_progname(argv[0]);
	flag_bailout_fatal_begin=3;
	uogetopt(flag_bailout_log_name ,PACKAGE,VERSION,
		&argc, argv, uogetopt_out,
		"usage: dyndbfdump <DYNDB",
		myopts,
		"  This program dumps the content of DYNDB.\n"
		"  Report bugs to dyndb@lists.ohse.de"
	);
	dyndb_init(&dy,0);
	if (-1==dyndb_fwalk(&dy, callback)) 
		xbailout(111,errno,"failed to dump database. dyndb_fwalk",0,0,0);
	if (!opt_n)
		X("\n",1);
	if (-1==buffer_flush(buffer_1)) xbailout(111,errno,"failed to write",0,0,0);
	exit(0);
}

