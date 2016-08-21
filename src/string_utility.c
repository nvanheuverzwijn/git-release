#include <string.h>
#include "string_utility.h"

int git_release_string_utility_endswith(const char *string, const char *suffix)
{
	if(!string || !suffix)
	{
		return 0;
	}
	size_t stringlen = strlen(string);
	size_t suffixlen = strlen(suffix);
    if(suffixlen >  stringlen)
	{
		return 0;
	}
	return strncmp(string + stringlen - suffixlen, suffix, suffixlen) == 0;
}
