/* reimplementation of str_len by djb@pobox.com */
/* uwe@ohse.de, 20000521. GPL. */

#include "str.h"

unsigned int 
str_len(const char *s)
{
	unsigned int i=0;
	for (;;) {
		if (!s[i]) return i;
		i++;

		if (!s[i]) return i;
		i++;

		if (!s[i]) return i;
		i++;

		if (!s[i]) return i;
		i++;
	}
}
