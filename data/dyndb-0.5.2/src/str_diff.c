/* reimplementation of str_diff by djb@pobox.com */
/* uwe@ohse.de, 20000521. GPL. */

#include "str.h"

int 
str_diff(const char *s,const char *t)
{
	unsigned int i=0;
	for (;;) {
		if (s[i]!=t[i]) break;
		if (!s[i]) break;
		i++;
	}
	return ((int)(unsigned int)(unsigned char) s[i])
		- ((int)(unsigned int)(unsigned char) t[i]);
}
