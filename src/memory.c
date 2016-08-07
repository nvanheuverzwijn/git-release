#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

void die(const char* message)
{
	fprintf(stderr, message);
	exit(1);
}

void *xmalloc(size_t size)
{
	void *ret = malloc(size);
	if (!ret && !size)
	{
		ret = malloc(1);
	}
	if (!ret)
	{
		ret = malloc(size);
		if (!ret && !size)
		{
			ret = malloc(1);
		}
		if (!ret)
		{
			die("Out of memory, malloc failed");
		}
	}
	return ret;
}
