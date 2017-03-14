/* Reimplementation of Daniel J. Bernsteins getln library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
#include "byte.h"
#include "getln.h"

int
getln (buffer * b, stralloc * sa, int *gotit, int termchar)
{
	if (!stralloc_ready (sa, 0))
		return -1;
	sa->len = 0;
	while (1) {
		int r;
		char *p;
		int off;
		r = buffer_feed (b);
		if (r < 0)
			return -1;
		if (r == 0) {
			*gotit = 0;
			return 0;
		}
		p = buffer_PEEK (b);
		off = byte_chr (p, r, termchar);

		if (off!=r)
			r=off+1; /* byte_chr returns index, not length */
		if (!stralloc_catb (sa, p, r))
			return -1;
		buffer_SEEK (b, r);

		if (off!=r) {
			*gotit = 1;
			return 0;
		}
	}
}
