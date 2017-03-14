#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h> /* malloc */
#include <stdio.h> /* rename */
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dyndb.h"
#include "bailout.h"
#include "uogetopt.h"
static struct dyndb dy;

static int
do_level(uint32 tpos, unsigned int lv)
{
	char spc[40];
	unsigned int i;
	for (i=0;i<lv*2;i++) 
		spc[i]=' ';
	spc[i]=0;

	for (i=0;i<dyndb_slotmod[lv];i++) {
		int cc=0;
		unsigned char conv[4];
		uint32 aktpos;
		if (-1==dyndb_seek(dy.fd,tpos+i*sizeof(uint32))) return -1;
		if (-1==dyndb_read(dy.fd, conv, sizeof (conv))) return -1;
		aktpos=dyndb_fromdisk(conv);
		if (aktpos==0) continue;
		if (DYNDB_ISTABLE(aktpos)) {
			int r=do_level(DYNDB_TABLEPOS(aktpos),lv+1);
			if (r) return r;
			continue;
		}
		while (aktpos && !DYNDB_ISTABLE(aktpos)) {
			unsigned char conv2[8];
			uint32 npos;
			uint32 hash;
			if (-1==lseek(dy.fd,aktpos,SEEK_SET)) return -1;
			if (-1==dyndb_read(dy.fd, conv2, sizeof (conv2))) return -1;
			npos=dyndb_fromdisk(conv2);
			hash=dyndb_fromdisk(conv2+4);
			printf("%s%sT%lx:%u %lx npos %lx, hash %lx",
					cc ? "\n" : "",
					spc,(unsigned long)tpos,i, (unsigned long)aktpos,
							(unsigned long)npos,(unsigned long)hash);
			aktpos=npos;
			cc++;
		}
		printf(", %d collisions\n",cc);
	}
	return 0;
}
static uogetopt_t myopts[]={
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
		"usage: dyndbtree <DYNDB",
		myopts,
		"  This program shows statistics about DYNDB.\n"
		"  Report bugs to dyndb@lists.ohse.de"
	);
	dyndb_init(&dy,0);

	if (do_level(0,0)) xbailout(111,errno,"failed to read or lseek",0,0,0);
	exit(0);
}

