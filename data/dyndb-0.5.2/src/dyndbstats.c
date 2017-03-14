#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h> /* malloc */
#include <stdio.h> /* rename */
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dyndb.h"
#include "timing.h"
#include "bailout.h"
#include "uogetopt.h"

unsigned long **lvdat;
unsigned long *tables;
unsigned long totaldata;
#define MAXCC 10
unsigned long ccstat[MAXCC+1];
#define MAXPOS 32
unsigned long seekcount[MAXPOS];
unsigned long posord[MAXPOS]={512};
unsigned long seekreadtime[MAXPOS];
int seekreadindex;

static off_t
do_lseek(int fildes, off_t offset, int whence)
{
	static off_t oldpos=0;
	off_t r;
	unsigned long x;
	int i=0;
	unsigned long l=512;

	start();
	r=lseek(fildes,offset,whence);
	if (-1==r) return r;

	if (r>oldpos) x=r-oldpos; else x=oldpos-r;
	x/=512;
	while (x) {
		x/=2;
		l*=2;
		i++;
		posord[i]=l;
	}
	seekcount[i]++;
	seekreadindex=i;

	oldpos=r;
	return r;
}
static int
do_read(int fd, unsigned char *buf, size_t l)
{
	int r=dyndb_read(fd, buf,l);
	seekreadtime[seekreadindex]+=stop();
	return r;
}

static int
do_level(int fd, uint32 tpos, unsigned int lv)
{
	unsigned int i;
	tables[lv]++;

	for (i=0;i<dyndb_slotmod[lv];i++) {
		unsigned char conv[4];
		int cc=0;
		uint32 aktpos;
		uint32 lastpos;
		if (-1==do_lseek(fd,tpos+i*sizeof(uint32),SEEK_SET)) return -1;
		if (-1==do_read(fd, conv, sizeof (conv))) return -1;
		aktpos=dyndb_fromdisk(conv);
		if (aktpos==0) continue;
		if (DYNDB_ISTABLE(aktpos)) {
			int r=do_level(fd,DYNDB_TABLEPOS(aktpos),lv+1);
			lvdat[lv][i]++;
			if (r) return r;
			continue;
		}
		lastpos=tpos+i*sizeof(uint32);
		while (aktpos && !DYNDB_ISTABLE(aktpos)) {
			unsigned char conv2[8];
			uint32 npos;
			if (-1==do_lseek(fd,aktpos,SEEK_SET)) return -1;
			if (-1==do_read(fd, conv2, sizeof (conv2))) return -1;
			npos=dyndb_fromdisk(conv2);
			lvdat[lv][i]++;
			lastpos=aktpos;
			aktpos=npos;
			totaldata++;
			cc++;
		}
		if (cc<=MAXCC) ccstat[cc]++; 
		else ccstat[MAXCC]++;
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
	int i;
	int levels;
	
	bailout_progname(argv[0]);
	flag_bailout_fatal_begin=3;
    uogetopt(flag_bailout_log_name ,PACKAGE,VERSION,
			&argc, argv, uogetopt_out,
			"usage: dyndbstats <DYNDB",
			myopts,
			"  This program shows statistics about DYNDB.\n"
			"  Report bugs to dyndb@lists.ohse.de"
		);


	for (levels=0;dyndb_slotmod[levels];) 
		levels++;
	lvdat=malloc(sizeof(void *)*levels);
	if (!lvdat) oom();
	tables=calloc(sizeof(unsigned long)*levels,1);
	if (!tables) oom();
	for (i=0;i<levels;i++)  {
		lvdat[i]=calloc(dyndb_slotmod[i]*sizeof(unsigned long),1);
		if (!lvdat[i]) oom();
	}	
	if (do_level(0,0,0)) xbailout(111,errno,"failed to read or lseek",0,0,0);
	for (i=0;i<levels;i++) {
		unsigned int j;
		unsigned int mi=UINT_MAX; unsigned int ma=0;
		unsigned long to=0;
		printf("Level %d, %lu tables\n",i,tables[i]);
		for (j=0;j<dyndb_slotmod[i];j++) {
			if (lvdat[i][j]<mi) mi=lvdat[i][j];
			if (lvdat[i][j]>ma) ma=lvdat[i][j];
			to+=lvdat[i][j];
			/* if (lvdat[i][j]) printf("  %d %lu\n",j,lvdat[i][j]); */
		}
		if (to)
			printf("min=%d, max=%d, total=%lu, load=%6.2f\n",
				mi,ma,to,((float)to)/dyndb_slotmod[i]/tables[i]);
	}
	printf("total records: %lu\n",totaldata);
	printf("collision statistic:\n");
	for (i=1;i<=MAXCC;i++)
		printf("  %d %lu\n",i,ccstat[i]);
	printf("seek length statistic:\n");
	for (i=0;i<MAXPOS;i++)
		if (seekcount[i])
			printf("  up to %lu bytes: %lu (%lu, %8.4f ms)\n",posord[i],
				seekcount[i],
				seekreadtime[i],
				((float)seekreadtime[i])/(1000*seekcount[i]));
	exit(0);
}

