/* Reimplementation of Daniel J. Bernsteins stralloc library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.stralloc_cats.c 1.4 01/05/01 08:01:19+00:00 uwe@fjoras.ohse.de $ */
#include "str.h"
#include "stralloc.h"

int
stralloc_cats (stralloc * sa, const char *str)
{
	return stralloc_catb (sa, str, str_len (str));
}
