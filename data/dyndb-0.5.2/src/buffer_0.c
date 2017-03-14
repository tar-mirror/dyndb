/* Reimplementation of Daniel J. Bernsteins buffer library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
#include "readwrite.h"
#include "buffer.h"

static int
buffer_0_read (int fd, char *buf, int len)
{
	if (buffer_flush (buffer_1) == -1)
		return -1;
	return read (fd, buf, len);
}

static char buffer_0_space[BUFFER_INSIZE];

static buffer buffer_0_buf =
       BUFFER_INIT ((buffer_op_t) buffer_0_read, 0, buffer_0_space, 
	                                                sizeof buffer_0_space);

buffer *buffer_0 = &buffer_0_buf;
