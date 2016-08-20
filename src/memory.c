#include <stdlib.h>
#include "memory.h"
#include "errors.h"

void* xmalloc(size_t size)
{
	void *ret = malloc(size);
	if(!ret && !size)
	{
		ret = malloc(1);
	}
	if(!ret)
	{
		ret = malloc(size);
		if(!ret && !size)
		{
			ret = malloc(1);
		}
		if(!ret)
		{
			die("Out of memory, malloc failed");
		}
	}
	return ret;
}

void* xrealloc(void* ptr, size_t size)
{
	void* ret = realloc(ptr, size);
    if(!ret && !size)
    {   
        ret = realloc(ptr, 1);
    }   
    if(!ret)
    {   
        ret = realloc(ptr, size);
        if(!ret && !size)
        {
            ret = realloc(ptr, 1);
        }
        if(!ret)
        {
            die("Out of memory, realloc failed");
        }
    }   
    return ret;
}
