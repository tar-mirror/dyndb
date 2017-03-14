/* Reimplementation of Daniel J. Bernsteins buffer library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
#include "readwrite.h"
#include "buffer.h"

static char buffer_1_space[BUFFER_OUTSIZE];

static buffer buffer_1_buf =
       BUFFER_INIT ((buffer_op_t) write, 1, buffer_1_space, 
	                                        sizeof buffer_1_space);

buffer *buffer_1 = &buffer_1_buf;
