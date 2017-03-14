/* Reimplementation of Daniel J. Bernsteins stralloc library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.stralloc_eady.c 1.4 01/05/01 08:01:20+00:00 uwe@fjoras.ohse.de $ */
#include "stralloc.h"
#include "gen_alloci.h"

int
stralloc_ready(stralloc *x,unsigned int n)
{
	return  gen_alloc_ready(
		&x->s,
		sizeof(*x->s),
		&x->len,
		&x->a,
		n);
}
