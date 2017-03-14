/* Reimplementation of Daniel J. Bernsteins stralloc library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.stralloc_free.c 1.8 01/05/02 08:44:19+00:00 uwe@fjoras.ohse.de $ */
#include "stralloc.h"
#include "alloc.h"

void stralloc_free(stralloc *sa)
{
	alloc_free(sa->s);
	sa->s = 0;
	sa->len = sa->a = 0;
}

