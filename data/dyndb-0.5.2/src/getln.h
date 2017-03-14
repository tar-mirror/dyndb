/* Reimplementation of Daniel J. Bernsteins getln library.
 * (C) 2001 Uwe Ohse, <uwe@ohse.de>.
 *   Report any bugs to <uwe@ohse.de>.
 * Placed in the public domain.
 */
#ifndef GETLN_H
#define GETLN_H

#include "buffer.h"
#include "stralloc.h"

extern int getln(buffer *,stralloc *,int *got_termchar,int termchar);

#endif
