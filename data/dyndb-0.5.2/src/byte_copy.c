/* Reimplementation of Daniel J. Bernsteins uint library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.byte_copy.c 1.3 01/04/29 08:11:07+00:00 uwe@fjoras.ohse.de $ */

#include "byte.h"

void 
byte_copy(char *to, unsigned int n,const char *from)
{
	for (;;) {
		if (!n) return; 
		*to++ = *from++; --n;

		if (!n) return; 
		*to++ = *from++; --n;

		if (!n) return; 
		*to++ = *from++; --n;

		if (!n) return; 
		*to++ = *from++; --n;
	}
}
