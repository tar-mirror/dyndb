#include <unistd.h>

int fsync(int fd)
{
	sync();
	(void) fd;
	return 0;
}
