/* Reimplementation of Daniel J. Bernsteins stralloc library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.stralloc_opyb.c 1.3 01/05/01 08:01:21+00:00 uwe@fjoras.ohse.de $ */
#include "stralloc.h"
#include "byte.h"

int
stralloc_copyb (stralloc * sa, const char *src, unsigned int n)
{
	if (!stralloc_ready (sa, n + 1))
		return 0;
	byte_copy (sa->s, n, src);
	sa->len = n;
	sa->s[n] = 'Z';	/* ``offensive programming'', indeed */
	return 1;
}
