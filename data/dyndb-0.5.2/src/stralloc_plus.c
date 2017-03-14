/* Reimplementation of Daniel J. Bernsteins stralloc library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.stralloc_plus.c 1.4 01/05/01 08:01:22+00:00 uwe@fjoras.ohse.de $ */

#include "stralloc.h"
#include "gen_alloci.h"
int
stralloc_readyplus(stralloc *x,unsigned int n)
{
	return  gen_alloc_readyplus(
		&x->s,
		sizeof(*x->s),
		&x->len,
		&x->a,
		n);
}
