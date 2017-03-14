/* Reimplementation of Daniel J. Bernsteins scan library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.scan.h 1.7 01/06/07 12:49:54+00:00 uwe@fjoras.ohse.de $ */
#ifndef SCAN_H
#define SCAN_H

extern unsigned int scan_plusminus(const char *,int *);

extern unsigned int scan_ushort(const char *,unsigned short *);

extern unsigned int scan_uint(const char *,unsigned int *);

extern unsigned int scan_long(const char *,long *);
extern unsigned int scan_ulong(const char *,unsigned long *);
extern unsigned int scan_xlong(const char *,unsigned long *);
extern unsigned int scan_8long(const char *,unsigned long *);

/* unimplemented: */
extern unsigned int scan_0x(const char *,unsigned int *);

extern unsigned int scan_xshort(const char *,unsigned short *);
extern unsigned int scan_nbbshort(const char *,unsigned int,unsigned int,unsigned int,unsigned short *);

extern unsigned int scan_xint(const char *,unsigned int *);
extern unsigned int scan_nbbint(const char *,unsigned int,unsigned int,unsigned int,unsigned int *);

extern unsigned int scan_nbblong(const char *,unsigned int,unsigned int,unsigned int,unsigned long *);

extern unsigned int scan_whitenskip(const char *,unsigned int);
extern unsigned int scan_nonwhitenskip(const char *,unsigned int);
extern unsigned int scan_charsetnskip(const char *,const char *,unsigned int);
extern unsigned int scan_noncharsetnskip(const char *,const char *,unsigned int);

extern unsigned int scan_strncmp(const char *,const char *,unsigned int);
extern unsigned int scan_memcmp(const char *,const char *,unsigned int);


#endif
