#include "errno.h"
#include "buffer.h"
#include "stralloc.h"
#include "dyndb.h"
#include <stdio.h> /* rename */
#include <unistd.h> /* fsync */
#include "bailout.h"
#include "fmt.h"
#include "uogetopt.h"
#include "str.h"
#include "attributes.h"

static void die_unchar(char c) attribute_noreturn;
static void die_eof(void) attribute_noreturn;
static void die_read(void) attribute_noreturn;
static void die_enter(void) attribute_noreturn;
static void die_lineno(const char *, const char *) attribute_noreturn;
static void die_bad_length(void) attribute_noreturn;
static int lineno;

static struct dyndb dy;

static void die_lineno (const char *s, const char *t)
{
	char nb[FMT_ULONG];
	nb[fmt_ulong(nb,lineno)]=0;
	xbailout(111,0,s,t," in input line ",nb);
}
static void die_bad_length(void) { die_lineno("","bad length"); }

static void die_unchar(char c)
{
	char nb[FMT_ULONG];
	nb[fmt_xlong(nb,(unsigned long)(unsigned char)c)]=0;
	die_lineno("unexpected character 0x",nb);
}

static void die_eof(void)
{ xbailout(100,0,"unexpected end of file",0,0,0); }
static void die_read(void)
{ xbailout(111,errno,"failed to read",0,0,0); }
static void die_enter(void)
{ xbailout(111,errno,"failed to enter record. dyndb_enter",0,0,0); }

static char 
getcha(void)
{
	int l;
	char c;
	l = buffer_get(buffer_0,&c,1);
	if (l==-1) die_read();
	if (l==0) die_eof();
	return c;
}

static char 
getexpect(const char c1, const char c2, int allow_eof)
{
	int l;
	char c;
	l = buffer_get(buffer_0,&c,1);
	if (l==-1) die_read();
	if (l==0 && allow_eof) return c2;
	if (l==0) die_eof();
	if (c==c1) return c;
	if (c2 && c==c2) return c;
	die_unchar(c);
}
static unsigned long 
getnum(const char terminator)
{
	unsigned long ul=0;
	unsigned int l=0;
	while (1) {
		char c;
		c=getcha();
		if (c==terminator) {
			if (!l) die_unchar(c);
			return ul;
		}
		if (c<'0' || c>'9') die_unchar(c);
		if (ul > 0x7fffffffUL/10)
			die_bad_length();
		ul=10*ul+ ( c - '0');
		l++;
	}
}

static uogetopt_t myopts[]={
{0,"",UOGO_MINARGS | UOGO_HIDDEN,0, 2 , 0,0},
{0,"",UOGO_MAXARGS | UOGO_HIDDEN,0, 3 , 0,0},
{0,0,0,0,0,0,0}
};

int 
main(int argc, char **argv)
{
	int fd;
	stralloc buf=STRALLOC_INIT;
	stralloc buf2=STRALLOC_INIT;

	bailout_progname(argv[0]);
	flag_bailout_fatal_begin=3;

	uogetopt(flag_bailout_log_name ,PACKAGE,VERSION,
		&argc, argv, uogetopt_out,
		"usage: dyndbmake DYNDB [tmpfile] <input",
		myopts,
		"  This program creates DYNDB from input.\n"
		"  Report bugs to dyndb@lists.ohse.de"
	);
	fd=dyndb_create_name(&dy, argv[2]? argv[2] : argv[1],0644);
	if (fd==-1) 
		xbailout(111,errno,"failed to create database. dyndb_create",0,0,0);

	lineno=0;
	while (1) {
		int l;
		int l1,l2;
		char c;
		char *p;
		lineno++;

		c=getexpect('+','\n',0);
		if (c=='\n') break;
		l1=getnum(',');
		l2=getnum(':');
		buf.len=0;
		while (l1) {
			l = buffer_feed(buffer_0);
			if (l==-1) die_read();
			if (l==0) die_eof();
			if (l>l1) l=l1;
			p = buffer_peek(buffer_0);
			if (!stralloc_catb(&buf,p,l)) oom();
			buffer_seek(buffer_0,l);
			l1-=l;
		}

		c=getexpect('-',0,0);
		c=getexpect('>',0,0);
		buf2.len=0;
		while (l2) {
			l = buffer_feed(buffer_0);
			if (l==-1) die_read();
			if (l==0) die_eof();
			if (l>l2) l=l2;
			p = buffer_peek(buffer_0);
			if (!stralloc_catb(&buf2,p,l)) oom();
			buffer_seek(buffer_0,l);
			l2-=l;
		}
		if (-1==dyndb_enter(&dy,(unsigned char *)buf.s,buf.len,
			(unsigned char *)buf2.s,buf2.len)) 
			die_enter();
		c=getexpect('\n',0,0);

	}
	if (-1==fsync(fd)) xbailout(111,errno,"failed to fsync",0,0,0);
	if (argv[2]) {
		if (-1==rename(argv[2],argv[1]))
			xbailout(111,errno,"failed to rename ",argv[2], " to ",
				argv[1]);
	}
	return 0;
}
