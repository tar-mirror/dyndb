/* reimplementation of str_rchr by djb@pobox.com */
/* uwe@ohse.de, 20000521. GPL. */

#include "str.h"

unsigned int 
str_rchr(const char *s,int c)
{
  char ch=c;
  unsigned int i=0,f=0;

  for (;;) {
  	if (!s[i]) break;
	if (s[i++]==ch) f=i;

  	if (!s[i]) break;
	if (s[i++]==ch) f=i;

  	if (!s[i]) break;
	if (s[i++]==ch) f=i;

  	if (!s[i]) break;
	if (s[i++]==ch) f=i;
  }
  if (!f) return i;
  return f-1;
}
