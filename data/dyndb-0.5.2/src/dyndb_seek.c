#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "dyndb.h"


int32 dyndb_seek(int fd, uint32 pos)
{
	return lseek(fd,pos,SEEK_SET);
}
int32 dyndb_seekend(int fd)
{
	return lseek(fd,0,SEEK_END);
}
