/*
 * these functions are somewhat like those of the lmbench suite
 * by Larry McVoy.   
 *
 * Uwe Ohse, 1999. PD.
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include "timing.h"

static void
tvsub(struct timeval *tdiff, struct timeval *t1,struct timeval * t0)
{
	tdiff->tv_sec = t1->tv_sec - t0->tv_sec;
	tdiff->tv_usec = t1->tv_usec - t0->tv_usec;
	if (tdiff->tv_usec < 0)
		tdiff->tv_sec--, tdiff->tv_usec += 1000000;
}

static struct timeval start_tv, stop_tv;

void
start(void)
{
	(void) gettimeofday(&start_tv, (struct timezone *) 0);
}

/* Stop timing and return real time in microseconds.  */
unsigned long
stop(void)
{
	struct timeval tdiff;

	(void) gettimeofday(&stop_tv, (struct timezone *) 0);

	tvsub(&tdiff, &stop_tv, &start_tv);
	return (tdiff.tv_sec * 1000000 + tdiff.tv_usec);
}

