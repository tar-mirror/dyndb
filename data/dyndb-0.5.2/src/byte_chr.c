/* Reimplementation of Daniel J. Bernsteins uint library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.byte_chr.c 1.3 01/04/29 08:11:06+00:00 uwe@fjoras.ohse.de $ */

#include "byte.h"

unsigned int 
byte_chr(const char *s, unsigned int n, int searched)
{
	char ch=searched;
	const char *p=s;

	for (;;) {
		if (!n) break; 
		if (*p == ch) break; ++p; --n;

		if (!n) break; 
		if (*p == ch) break; ++p; --n;

		if (!n) break; 
		if (*p == ch) break; ++p; --n;

		if (!n) break; 
		if (*p == ch) break; ++p; --n;
	}
	return p - s;
}
