/* Reimplementation of Daniel J. Bernsteins stralloc library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.stralloc_opys.c 1.3 01/05/01 08:01:21+00:00 uwe@fjoras.ohse.de $ */
#include "str.h"
#include "stralloc.h"

int
stralloc_copys (stralloc * sa, const char *src)
{
	return stralloc_copyb (sa, src, str_len (src));
}
