/* Reimplementation of Daniel J. Bernsteins stralloc library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.stralloc_copy.c 1.4 01/05/01 08:01:19+00:00 uwe@fjoras.ohse.de $ */
#include "stralloc.h"

int
stralloc_copy (stralloc * to, stralloc * from)
{
	return stralloc_copyb (to, from->s, from->len);
}
