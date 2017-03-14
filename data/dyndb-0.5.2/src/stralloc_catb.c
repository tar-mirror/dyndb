/* Reimplementation of Daniel J. Bernsteins stralloc library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.stralloc_catb.c 1.4 01/05/01 08:01:19+00:00 uwe@fjoras.ohse.de $ */
#include "stralloc.h"
#include "byte.h"

int
stralloc_catb (stralloc * sa, const char *str, unsigned int len)
{
	if (!stralloc_readyplus (sa, len + 1))
		return 0;
	byte_copy (sa->s + sa->len, len, str);
	sa->len += len;
	sa->s[sa->len] = 'Z'; /* djb */
	return 1;
}
