/*
 * Copyright (C) 1999,2000 Uwe Ohse
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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
#ifndef UOGETOPT_H
#define UOGETOPT_H

#define UOGO_FLAG   0   /* (*var) shall be "val": this means _no_ argument */
#define UOGO_FLAGOR 1   /* (*var) shall be "(*arg)|val": this means _no_ argument */
#define UOGO_STRING 2   /* (*var) shall be pointer to argument */
#define UOGO_ULONG  4   /* (*var)=strtoul(argument) */
#define UOGO_LONG   5   /* (*var)=strtol(argument) */
#define UOGO_CALLBACK 6 /* var is function ptr: int (*fn)(uogetopt_t *, const char *) */
#define UOGO_NOOPT  7   /* this is not really an option */
#define UOGO_PRINT  8   /* just print the help text and exit */
#define UOGO_TEXT   9   /* just print the help text without padding */
#define UOGO_INCLUDE 10 /* include another table - only understood by .._join */
#define UOGO_MINARGS 11 /* val contains minimal number of remaining args. */
#define UOGO_MAXARGS 12 /* val contains maximum number of remaining args. */

/* to OR into the argtype */
#define UOGO_HIDDEN  0x10000 /* don't show this in --help or --longhelp */
#define UOGO_OPTARG  0x20000 /* option may or may not have an argument */

extern int uogo_posixmode; /* defaults to off */
typedef struct {
	char shortname;
	const char *longname;
	int argtype; /* 0 or 1 no argument, set (*arg) from val. */
	void *var; /* mandatory */
	int val; /* val for *var in case of type 0 or 1 */
	const char *help;
	const char *paraname;
} uogetopt_t;

#ifndef P__
#if defined (__GNUC__) || (defined (__STDC__) && __STDC__)
#define P__(args) args
#else
#define P__(args) ()
#endif  /* GCC.  */
#endif  /* Not P__.  */
void uogetopt P__((
	const char *prog, const char *package, const char *version,
	int *argc, char **argv, /* note: "int *" */
	void (*out)(int iserr,const char *), 
	const char *head,
	uogetopt_t *,
	const char *tail));

typedef struct  {
	const char *program;
	const char *package;
	const char *version;
	const char *head;
	const char *tail;
	void (*out)(int iserr,const char *);
	uogetopt_t *opts;
	int doexit;
	int flag_err;
} uogetopt_env_t;
int uogetopt2(uogetopt_env_t *,int *argc, char **argv); /* 1 ok, 0 error */

void uogetopt_out P__((int iserr,const char *s));
uogetopt_t *uogetopt_join(uogetopt_t *);
void uogetopt_free(uogetopt_t *); /* free what join() allocated */
#endif
