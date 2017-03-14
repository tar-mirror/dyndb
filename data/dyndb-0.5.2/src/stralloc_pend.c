/* Reimplementation of Daniel J. Bernsteins stralloc library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.stralloc_pend.c 1.4 01/05/01 08:01:21+00:00 uwe@fjoras.ohse.de $ */
#include "stralloc.h"
#include "gen_alloci.h"

int
stralloc_append(stralloc *s, const char *t)
{
	return gen_alloc_append(&s->s,sizeof(*t), &s->len, &s->a, t);
}
