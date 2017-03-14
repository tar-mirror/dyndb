/* Reimplementation of Daniel J. Bernsteins scan library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
/* @(#) $Id: s.scan_sign.c 1.4 01/05/02 08:23:20+00:00 uwe@fjoras.ohse.de $ */
#include "scan.h"

unsigned int
scan_plusminus (const char *str, int *sign)
{
	if (*str == '-') {
		*sign = -1;
		return 1;
	}
	*sign = 1;
	return (*str == '+');
}
