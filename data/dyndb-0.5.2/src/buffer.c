/* Reimplementation of Daniel J. Bernsteins buffer library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
#include "buffer.h"

void
buffer_init (buffer * b, buffer_op_t op, int fd, char *buf,
			 unsigned int len)
{
	b->buf = buf;
	b->fd = fd;
	b->op = op;
	b->pos = 0;
	b->len = len;
}
