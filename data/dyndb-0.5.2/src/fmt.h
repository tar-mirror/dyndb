/* Reimplementation of Daniel J. Bernsteins fmt library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.fmt.h 1.4 01/05/01 07:03:21+00:00 uwe@fjoras.ohse.de $ */
#ifndef FMT_H
#define FMT_H

#define FMT_ULONG 40 /* enough to hold 2^128 - 1 in decimal, plus \0 */
#define FMT_LEN ((char *) 0)

extern unsigned int fmt_ushort(char *target,unsigned short val);
extern unsigned int fmt_ushort0(char *target,unsigned short val, 
	unsigned int len);

extern unsigned int fmt_uint(char *target,unsigned int val);
extern unsigned int fmt_uint0(char *target,unsigned int val,unsigned int len);

extern unsigned int fmt_ulong(char *target,unsigned long val);
extern unsigned int fmt_ulong0(char *target,unsigned long val,
	unsigned int len);
extern unsigned int fmt_xlong(char *target,unsigned long val);
extern unsigned int fmt_xlong0(char *target,unsigned long val,
	unsigned int len);

#if 0
/* bernstein also has these: */
extern unsigned int fmt_xint(char *target,unsigned int);
extern unsigned int fmt_nbbint(char *target,unsigned int,unsigned int,unsigned int,unsigned int);
extern unsigned int fmt_xshort(char *target,unsigned short);
extern unsigned int fmt_nbbshort(char *target,unsigned int,unsigned int,unsigned int,unsigned short);
extern unsigned int fmt_nbblong(char *target,unsigned int,unsigned int,unsigned int,unsigned long);

extern unsigned int fmt_plusminus(char *,int);
extern unsigned int fmt_minus(char *,int);
extern unsigned int fmt_0x(char *,int);

extern unsigned int fmt_str(char *,char *);
extern unsigned int fmt_strn(char *,char *,unsigned int);
#endif

#endif
