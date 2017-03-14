/* uogetopt.c: somewhat GNU compatible reimplementation of getopt(_long) */

/*
 * Copyright (C) 1999 Uwe Ohse
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; either version 2 of 
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * Contact: uwe@ohse.de
 */

/*
 * main differences:
 * + new feature: --version 
 * + new feature: --help  (standard help text, one line per option)
 * + new feature: --help OPTION (maybe somewhat more)
 * + new feature: --longhelp (all --help OPTION)
 * o really no support for i18n.
 * o small code, about 65% of GNU C library stuff.
 * o it doesn't do black magic, like that GNU stuff.
 *
 * Note that the following statement from the GNU getopt.c was the cause
 * for reinventing the wheel:
 *    Bash 2.0 puts a special variable in the environment for each
 *    command it runs, specifying which ARGV elements are the results of
 *    file name wildcard expansion and therefore should not be
 *    considered as options.
 * i decided that this wheel is to broken to be reused. Think of that
 * "-i" trick. As time passed by i calmed down, but now uogetopt is
 * better than GNU getopt ...
 * And in any case: uogetopt is shorter.
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "readwrite.h"
#include "str.h"
#include "env.h"
#include "scan.h"
#include "uogetopt.h"
#include "attributes.h"

#define FLAGS (UOGO_HIDDEN|UOGO_OPTARG)
extern void _exit(int) attribute_noreturn;

static void	uogetopt_printver(uogetopt_env_t *env) 
	attribute_regparm(1);
static void handle_argopt(uogetopt_env_t *env, uogetopt_t *o, char *arg) 
	attribute_regparm(3);
static void outplus(void (*out)(int, const char *), const char *s) 
	attribute_regparm(2);
static unsigned int outandcount(void (*out)(int iserr,const char *), 
	const char *s) attribute_regparm(2);
static void uogetopt_printhelp(uogetopt_env_t *env, int mode)
	attribute_regparm(2);

int uogo_posixmode=0;

static int dummy;
static uogetopt_t hack_version=
    {0,"version",   UOGO_FLAG,&dummy,0,
	     "Show version information.",0
	};
static uogetopt_t hack_help=
    {0,"help",   UOGO_FLAG,&dummy,0,
       /* 12345678901234567890123456789012345678901234567890" */
		 "Show a list of options or the long help on one.\n"
		 "The use with an argument shows the long help text\n"
		 "of that option, without an argument it will list\n"
		 "all options.","[OPTION-NAME]"
	};
static uogetopt_t hack_longhelp=
    {0,"longhelp",   UOGO_FLAG,&dummy,0,
	     "Show longer help texts for all or one option."
       /* 12345678901234567890123456789012345678901234567890" */
		 ,"[OPTION-NAME]"
	};

static char minus_help[]="--help";
static char minus_version[]="--version";


#define L longname
#define S shortname
#define LLEN 80
#define OFFSET 29 /* keep me < LLEN */

void 
uogetopt_out(int iserr,const char *s)
{
	int fd=1;
	if (iserr)
		fd=2;
	write(fd,s,str_len(s));
}

static void
uogetopt_outn (uogetopt_env_t * env,
			   int iserr, const char *s, const char *t, const char *u,
			   const char *v)
{
	env->out (iserr, s);
	if (t) env->out (iserr, t);
	if (u) env->out (iserr, u);
	if (v) env->out (iserr, v);
}
#define O2(s,t) do { uogetopt_outn(env,0,(s),(t),0,0); } while (0)
#define O3(s,t,u) do { uogetopt_outn(env,0,(s),(t),(u),0); } while (0)
#define O4(s,t,u,v) do { uogetopt_outn(env,0,(s),(t),(u),(v)); } while (0)
#define E2(s,t) do { uogetopt_outn(env,1,(s),(t),0,0); } while (0)
#define E4(s,t,u,v) do { uogetopt_outn(env,1,(s),(t),(u),(v)); } while (0)
#define SETEXIT() do { env->flag_err=1; } while(0)
#define SETEXITVOID() do { SETEXIT(); return; } while(0)
#define SETEXITRET(x) do { SETEXIT(); return x; } while(0)

static void outplus(void (*out)(int, const char *),
	const char *s)
{
	unsigned int l;
	out(0,s); 
	l=str_len(s);
	if (s[l-1]!='\n')
		out(0,"\n");
}

static void
uogetopt_num(uogetopt_env_t *env, int issigned, char *s,uogetopt_t *o)
{
	if (issigned ? 
		0==scan_long(s,(long *)o->var) :
		0==scan_ulong(s,(unsigned long *)o->var)) {
		E4(env->program,": ",s,": not a number.\n");
		SETEXITVOID();
	}
}
#define uogetopt_hulong(e,s,o) uogetopt_num(e,0,s,o)
#define uogetopt_hlong(e,s,o) uogetopt_num(e,1,s,o)


static unsigned int 
outandcount(void (*out)(int iserr,const char *), const char *s)
{
	if (!s) return 0;
	out(0,s);
	return str_len(s);
}

static void
uogetopt_describe(void (*out)(int iserr,const char *),uogetopt_t *o, int full)
{
/* 
  -s, --size                 print size of each file, in blocks
  -S                         sort by file size
      --sort=WORD            ctime -c, extension -X, none -U, size -S,
123456789012345678901234567890
I don't really know for what the spaces at the start at the line are
for, but i suppose someone will need them ...
*/
	int l;
	unsigned int off;
	const char *p;
	char buf[LLEN+1];
	char minbuf[3];
	if (o->argtype==UOGO_TEXT) {
		outplus(out,o->help);
		return;
	}

	for (l=0;l<OFFSET;l++)
		buf[l]=' ';
	buf[l]=0;

	out(0,"  ");
	if (o->S) { 
		/* -X */
		minbuf[0]='-'; 
		minbuf[1]=o->S; 
		minbuf[2]=0; 
		out(0,minbuf);
	} else out(0,"  ");

	if (o->S && o->L) out(0,", --");
	else if (o->L)    out(0,"  --");
	else              out(0,"    ");

	l=8;

	if (o->L) l+=outandcount(out,o->L);

	if (o->argtype!=UOGO_FLAG && o->argtype!=UOGO_FLAGOR
		&& o->argtype != UOGO_PRINT) {
		const char *x;
		if (o->L) out(0,"=");
		else      out(0," ");
		l++;
		if (o->paraname) x=o->paraname;
		else if (o->argtype==UOGO_STRING) x="STRING";
		else x="NUMBER";
		l+=outandcount(out,x);
	}

	/* fill up with spaces (at least one) */
	out(0,buf+ ((OFFSET-l) ? l : OFFSET-1));

	/* 1. line of help */
	off=str_chr(o->help,'\n');
	if (!o->help[off] || !o->help[off+1]) { /* no \n or at the end */
		outplus(out,o->help); 
		return;
	}
	minbuf[1]=0;
	p=o->help;
	do {minbuf[0]=*p;out(0,minbuf); } while (*p++!='\n');

	if (!full) return;
	do { 
		out(0,buf);
		do {minbuf[0]=*p++;out(0,minbuf);} while (*p && p[-1]!='\n');
		if (!*p) out(0,"\n");
	} while (EXPECT(*p,1) && EXPECT(p[1],1));
}

static void	
uogetopt_printver(uogetopt_env_t *env)
{
		env->out(0,env->program);
		if (env->package) { O3(" (",env->package,")"); }
		O3(" ",env->version,"\n");
}

static void 
handle_argopt(uogetopt_env_t *env, uogetopt_t *o, char *arg)
{
	int at=o->argtype;
	if (at==UOGO_STRING)
		{ *((char **)o->var)=arg; return; }
	if (at==UOGO_ULONG)
		{ uogetopt_hulong(env,arg,o); return; }
	if (at==UOGO_LONG)
		{ uogetopt_hlong(env,arg,o); return; }
	if (at==UOGO_CALLBACK) { 
		union disqualify {
			void *v;
			void (*fn)P__((uogetopt_t *,const char *));
		} dq;
		dq.v=o->var;
		(*dq.fn)(o,arg);
		return;
	}
	E2(env->program,": internal problem, unknown option type.\n");
	_exit(1);
}
#define PRINTHELP_MODE_SHORT 0
#define PRINTHELP_MODE_NORM  1
#define PRINTHELP_MODE_LONG  2
static void 
uogetopt_printhelp(uogetopt_env_t *env, int mode)
{
	uogetopt_t *opts=env->opts;
	void (*out)(int iserr,const char *) = env->out;

	/* uogetopt_printver(out,prog,package,version); 
	 * is against the GNU standards */
	if (env->head) outplus(out,env->head); 
	else { O3("usage: ",env->program," [options]\n\n");}
	if (mode) {
		int i;
		for (i=0;opts[i].S || opts[i].L;i++)
			if (!(opts[i].argtype & UOGO_HIDDEN))
				uogetopt_describe(out,&opts[i],mode==PRINTHELP_MODE_LONG);
			uogetopt_describe(out,&hack_version,mode==PRINTHELP_MODE_LONG);
			uogetopt_describe(out,&hack_help,mode==PRINTHELP_MODE_LONG);
			uogetopt_describe(out,&hack_longhelp,mode==PRINTHELP_MODE_LONG);
	}
	if (env->tail) outplus(out,env->tail);
}

int 
uogetopt2(uogetopt_env_t *env, int *argc, char **argv)
{
	int i;
	int posix;
	int newargc;
	int ocount;
	int is_longhelp;
	int h_used=0;
	int v_used=0;
	int ques_used=0;
	int V_used=0;
	int minargs=-1;
	int maxargs=-1;
	uogetopt_t *copyright=0;

	uogetopt_t *opts=env->opts;
	void (*out)(int iserr,const char *) = env->out;

	env->flag_err=0;
	for (i=0;opts[i].S || opts[i].L;i++) {
		if ((opts[i].argtype & ~(FLAGS)) == UOGO_MINARGS)
			minargs=opts[i].val;
		if ((opts[i].argtype & ~(FLAGS)) == UOGO_MAXARGS)
			maxargs=opts[i].val;
		if ((opts[i].argtype & ~(FLAGS)) == UOGO_INCLUDE) {
			out(1,"UOGO_INCLUDE set in uogetopt().\n");
			_exit(1); /* programmers fault, terminate program */
		}
		if (!opts[i].var && !(opts[i].argtype&UOGO_HIDDEN)) {
			int at;
			at=opts[i].argtype & ~(FLAGS);
			/* no use to waste code for detailed error messages, this only
			 * happens during development.
			 */
			if (at!=UOGO_PRINT && at != UOGO_TEXT) {
				out(1,"NULL variable address in uogetopt().\n");
				_exit(1); /* programmers fault, terminate program */
			}
		}
		if (opts[i].L && str_equal(opts[i].L,"copyright"))
			copyright=&opts[i];
		switch (opts[i].S) {
		case 'v': v_used=1; break;
		case 'h': h_used=1; break;
		case 'V': V_used=1; break;
		case '?': ques_used=1; break;
		}
	}
	if (!argv[1]) {
		newargc=1;
		goto checkrest;
	}

	/* try to map -?, -h to --help, -V, -v to --version */
	if (!argv[2] && argv[1][0]=='-') {
		if (argv[1][1]=='h' && !h_used) argv[1]=minus_help;
		if (argv[1][1]=='?' && !ques_used) argv[1]=minus_help;
		if (argv[1][1]=='v' && !v_used) argv[1]=minus_version;
		if (argv[1][1]=='V' && !V_used) argv[1]=minus_version;
	}
	ocount=i;
	if (argv[1] && str_equal(argv[1],minus_version)) {
		uogetopt_printver(env);
		if (copyright && copyright->help) {
			out(0,"\n");
			outplus(out,copyright->help);
		}
		goto exithelpversion;
	}
	is_longhelp=(str_equal(argv[1],"--longhelp"));
	if (argv[1] && (is_longhelp || str_equal(argv[1],minus_help))) {
		if (argv[2]) { 
			uogetopt_t *u=0;
			if (argv[2][0]=='-') argv[2]++;
			if (argv[2][0]=='-' && argv[2][1]) argv[2]++;
			for (i=0;i<ocount;i++) {
				if (opts[i].L && str_equal(opts[i].L,argv[2])) break;
				if (opts[i].S && !argv[2][1] && argv[2][0]==opts[i].S) break;
			}
			if (i!=ocount) u=&opts[i]; 
			else if (str_equal(argv[2],"longhelp")) u=&hack_longhelp;
			else if (str_equal(argv[2],"help")) u=&hack_help;
			else if (str_equal(argv[2],"version")) u=&hack_version;
			if (!u) { (out)(0,"no such option\n"); goto exiterr; }
			if (!u->help) { (out)(0,"no help available\n"); goto exiterr; }
			uogetopt_describe(out,u,1); /* long help every time */
			goto exithelpversion;
		}
		uogetopt_printhelp(env,PRINTHELP_MODE_NORM+is_longhelp);
		goto exithelpversion;
	}

	if (uogo_posixmode)
		posix=1;
	else
		posix=!!env_get("POSIXLY_CORRECT");
	newargc=1;
	for (i=1;i<*argc;i++) {
		if (*argv[i]!='-' || !argv[i][1]) {
			if (posix) { 
		  copyrest:
				while (argv[i]) argv[newargc++]=argv[i++];
				argv[newargc]=0;
				*argc=newargc;
				goto checkrest;
			}
			argv[newargc++]=argv[i];
			continue;
		}
		if (argv[i][1]=='-') { 
			int j;
			int ioff;
			char *para;
			uogetopt_t *o;
			int at;
			unsigned int paraoff;

			if (!argv[i][2]) { i++; goto copyrest; } /* -- */

			o=opts;

			/* --x=y */
			paraoff=str_chr(argv[i],'=');
			if (argv[i][paraoff]) { 
				para=argv[i]+paraoff; 
				*para++=0;
				ioff=0;
			} else {
				ioff=1;
				para=0;
			}

			for (j=0;j<ocount;o++,j++)
				if (o->L && str_equal(o->L,argv[i]+2)) 
					break;
			at=o->argtype & (~(FLAGS));
			if (j==ocount || at==UOGO_NOOPT) {
				E4(env->program,": illegal option -- ",argv[i],"\n");
				goto exiterr;
			}
			if (ioff==1) {
				if (o->argtype & UOGO_OPTARG) {
					if (argv[i+1][0]!='-') 
						para=argv[i+1];
					if (!para) {
						char one[]="1";
						para=one;
						ioff=0;
					}
				} else
					para=argv[i+1];
			}

			if ((at==UOGO_FLAG || at==UOGO_FLAGOR || at==UOGO_PRINT) 
				&& ioff==0) { 
				E4(env->program,": option doesn't allow an argument -- ",
					argv[i], "\n");
				goto exiterr;
			}
			if (at==UOGO_FLAG) { *((int*)o->var)=o->val; continue;}
			if (at==UOGO_FLAGOR) { *((int*)o->var)|=o->val; continue;}
			if (at==UOGO_PRINT) { 
				outplus(out,o->help); 
				goto exithelpversion;
			}
			if (!para) {
				E4(env->program,": option requires an argument -- ", 
					argv[i]+2,"\n");goto exiterr;
			}
				
			handle_argopt(env,o,para);
			i+=ioff;
			continue;
		} else { 
			int j;
			for (j=1;argv[i][j];j++) { /* all chars */
				char c=argv[i][j];
				int k;
				int nexti;
				char *p;
				char optstr[2];
				uogetopt_t *o;
				int at;
				optstr[0]=c;optstr[1]=0;
				o=opts;
				for (k=0;k<ocount;k++,o++) {
					if (o->S && o->S==c) 
						break;
				}
				at=o->argtype & (~(FLAGS));
				if (k==ocount || at==UOGO_NOOPT) {
					E4(env->program,": illegal option -- ",optstr,"\n");
					goto exiterr;
				}
				if (at==UOGO_FLAG) { *((int*)o->var)=o->val; continue;}
				if (at==UOGO_FLAGOR) { *((int*)o->var)|=o->val; continue;}
				if (at==UOGO_PRINT) { 
					outplus(out,o->help);
					goto exithelpversion;
				}
				/* options with arguments, first get arg */
				nexti=i; p=argv[i]+j+1; 
				if (!*p) { 
					p=argv[i+1];
					if (o->argtype & UOGO_OPTARG) {
						if (*p=='-')
							p=0;
						if (!p) {
							char one[]="1";
							p=one;
						}
					}
					if (!p) {
						E4(env->program,": option requires an argument -- ",
							optstr,"\n");
						goto exiterr;
					}
					nexti=i+1;
				}

				handle_argopt(env,o,p);
				i=nexti;
				break;
			}
		}
	}
	*argc=newargc;
	argv[newargc]=0;
  checkrest:
    if (minargs!=-1 && newargc <minargs) {
	  E2(env->program,": need more");
	  goto finish_argcount;
	}
    if (maxargs!=-1 && newargc >maxargs) {
	  E2(env->program,": too many");
   finish_argcount:
      out(1," arguments. Use the --help option for more information.\n");
	  uogetopt_printhelp(env,0);
	  goto exiterr;
	}
	return 1;
  exithelpversion:
    if (env->doexit) _exit(0);
	return 2;
  exiterr:
    if (env->doexit) _exit(2);
	return 0;
}
void uogetopt (
    const char *prog, const char *package, const char *version,
	int *argc, char **argv, /* note: "int *" */
	void (*out)(int iserr,const char *),
	const char *head,
	uogetopt_t *opts,
	const char *tail)
{
	uogetopt_env_t e;
	e.program=prog;
	e.package=package;
	e.version=version;
	e.head=head;
	e.tail=tail;
	e.out=out;
	e.opts=opts;
	e.doexit=1;
	e.flag_err=0;
	uogetopt2(&e,argc,argv);
}


#ifdef TEST

int flag=0;
char *string;
unsigned long ul=17;
signed long lo=18;
uogetopt_t myopts[]={
	{'t',"test",UOGO_FLAG,&flag,1,"test option\n",0},
	{'l',"longtest",UOGO_FLAGOR,&flag,2,"test option with some characters\nand more than one line, of which one is really long, of course, just to test some things, like crashs or not crash, life or 42\n",0},
	{'L',0,UOGO_FLAG,&flag,2,"test option with some characters\nand more than two lines\nwith not much information\nbut a missing newline at the end: 43",0},
	{'M',0,UOGO_FLAG,&flag,2,"test option with some characters\nand two lines: 44\n",0},
	{'N',0,UOGO_FLAG,&flag,2,"test option with some characters\nand two lines, no nl at end: 45",0},
	{'-',"del",UOGO_TEXT,0,0,"delimiting text",0},
	{'p',"print",UOGO_PRINT,0,0,"UOGO_PRINT demonstration",0},
	{'q',"print2",UOGO_PRINT,0,0,"UOGO_PRINT demonstration\nwith some more text\nand still more\neven with a very, very long line with quite some characters in it",0},
	{'u',"ulong",UOGO_ULONG,&ul,2,"set a ulong variable\n","Ulong"},
	{'n',"nulong",UOGO_LONG,&lo,2,"set a long variable\n","long"},
	{'s',"string",UOGO_STRING,&string,2,"set a string variable\n","String"},
	{0,"copyright",UOGO_PRINT|UOGO_HIDDEN,0,0,"copyright notice",0},
	{0,0,0,0,0,0,0}
};
int 
main(int argc, char **argv)
{

	printf("old argc=%d\n",argc);
	uogetopt("uogetopt","misc","0.1",
		&argc, argv,
		uogetopt_out,
		"head\n",myopts,"tail\n");
	printf("flag=%d\n",flag);
	printf("string=%s\n",string ? string : "NULL");
	printf("ulong=%lu\n",ul);
	printf("long=%ld\n",lo);
	printf("new argc=%d\n",argc);
	{ int i; for (i=0;argv[i];i++) printf("argv%d: %s\n",i,argv[i]);}
	exit(0);
}
#endif
