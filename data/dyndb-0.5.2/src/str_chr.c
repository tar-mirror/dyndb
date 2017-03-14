/* reimplementation of str_chr by djb@pobox.com */
/* uwe@ohse.de, 20000521. GPL. */

#include "str.h"

unsigned int 
str_chr(const char *s,int c)
{
  char ch;
  unsigned int l=0;

  ch = c;
  for (;;) {
  	if (!s[l]) break;
	if (s[l]==ch) break;
	l++;

  	if (!s[l]) break;
	if (s[l]==ch) break;
	l++;

  	if (!s[l]) break;
	if (s[l]==ch) break;
	l++;

  	if (!s[l]) break;
	if (s[l]==ch) break;
	l++;
  }
  return l;
}
