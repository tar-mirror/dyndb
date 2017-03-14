/* Reimplementation of Daniel J. Bernsteins fmt library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.gen_fmt.h 1.4 01/05/02 08:26:11+00:00 uwe@fjoras.ohse.de $ */
#include "fmt.h"

#define FMT_UFUNC(name,type,base,charset) \
unsigned int \
fmt_##name(char *t, type num) \
{ type num2; unsigned int len; \
  for (len=1,num2=num; num2>=base; num2/=base) len++; \
  if (t) { \
    unsigned int len2=len; \
  	do { t[--len2]=charset[num%base]; num/=base; } while(num); \
  } \
  return len; \
}

#define FMT_UFUNC_PAD(name,type,base,charset,padchar) \
unsigned int \
fmt_##name(char *t, type num, unsigned int minlen) \
{ type num2; unsigned int len; \
  for (len=1,num2=num; num2>=base; num2/=base) len++; \
  if (t) { \
    unsigned int len2=minlen > len ? minlen : len ; \
  	do { t[--len2]=charset[num%base]; num/=base; } while(num); \
	while (len2) { t[--len2]=padchar; } \
  } \
  return len > minlen ? len : minlen ; \
}

