#include <unistd.h> /* fsync */
#include "error.h"
#include "buffer.h"
#include "stralloc.h"
#include "dyndb.h"
#include "bailout.h"
#include "open.h"
#include "str.h"
#include "uogetopt.h"

static uogetopt_t myopts[]={
{0,"",UOGO_MINARGS | UOGO_HIDDEN,0, 3 , 0,0},
{0,"",UOGO_MAXARGS | UOGO_HIDDEN,0, 3 , 0,0},
{0,0,0,0,0,0,0}
};
static struct dyndb dy;

int main(int argc, char **argv)
{
	int fd;
	bailout_progname(argv[0]);
	flag_bailout_fatal_begin=3;

	uogetopt(flag_bailout_log_name ,PACKAGE,VERSION,
		&argc, argv, uogetopt_out,
		"usage: dyndbdel DYNDB KEY",
		myopts,
		"  This program deletes the record matching KEY from DYNDB\n"
		"  by making it inaccessible.\n"
		"  Report bugs to dyndb@lists.ohse.de"
	);

	fd=open_readwrite(argv[1]);
	if (fd==-1) xbailout(100,errno,"failed to open ",argv[1],0,0);
	dyndb_init(&dy,fd);

	switch(dyndb_lookup(&dy,(unsigned char *)argv[2],str_len(argv[2]))) {
	case -1: xbailout(100,errno,"failed to find record. dyndb_lookup",0,0,0);
	case  0: xbailout(100,0,"failed to find record.",0,0,0);
	}

	if (dyndb_delete(&dy))
		xbailout(100,errno,"failed to delete record. dyndb_delete", 0,0,0);
	if (-1==fsync(fd))
		xbailout(100,errno,"failed to fsync",0,0,0);

	return (0);
}
