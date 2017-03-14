#include "gen_alloci.h"

int 
gen_alloc_readyplus(char **bptr, unsigned int bsize, unsigned int *len, 
	unsigned int *a, unsigned int newlen)
{
	if (*len+newlen < *a)
		return 1;
	return gen_alloc_ready(bptr,bsize,len,a, *len+newlen);
}
