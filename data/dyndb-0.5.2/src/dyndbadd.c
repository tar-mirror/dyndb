#include <unistd.h> /* fsync */
#include "error.h"
#include "buffer.h"
#include "stralloc.h"
#include "dyndb.h"
#include "bailout.h"
#include "open.h"
#include "str.h"
#include "uogetopt.h"
#include "attributes.h"

static uogetopt_t myopts[]={
{0,"",UOGO_MINARGS | UOGO_HIDDEN,0, 3 , 0,0},
{0,"",UOGO_MAXARGS | UOGO_HIDDEN,0, 3 , 0,0},
{0,0,0,0,0,0,0}
};
static struct dyndb dy;

static void die_enter(void) attribute_noreturn;
static void die_enter(void)
{xbailout(111,errno,"failed to add record. dyndb_enter",0,0,0);}

int main(int argc, char **argv)
{
	int fd;

	bailout_progname(argv[0]);
	flag_bailout_fatal_begin=3;
	uogetopt(flag_bailout_log_name ,PACKAGE,VERSION,
		&argc, argv, uogetopt_out,
		"usage: dyndbadd DYNDB KEY <VALUE",
		myopts,
		"  This program adds a record with KEY and VALUE to DYNDB.\n"
		"  Report bugs to dyndb@lists.ohse.de"
	);

	fd=open_readwrite(argv[1]);
	if (fd==-1) xbailout(111,errno,"failed to open ",argv[1],0,0);
	dyndb_init(&dy,fd);
	if (-1==dyndb_enterstart(&dy))
		die_enter();
	if (-1==dyndb_enteraddkey(&dy,(unsigned char *)argv[2],str_len(argv[2])))
		die_enter();

	while (1) {
		int l;
		char *p;

		l = buffer_feed(buffer_0);
		if (l==-1) xbailout(111,errno,"failed to read",0,0,0);
		if (!l) break;
		p = buffer_peek(buffer_0);
		if (-1==dyndb_enteradddata(&dy,(unsigned char *)p,l))
			die_enter();
		buffer_seek(buffer_0,l);
	}

	if (-1==dyndb_enterfinish(&dy)) 
		die_enter();
	if (-1==fsync(fd))
		xbailout(100,errno,"failed to fsync",0,0,0);

	return (0);
}
