#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string_utility.h"
#include "memory.h"

int git_release_string_utility_endswith(const char *string, const char *suffix)
{
	if(!string || !suffix)
	{
		return 0;
	}
	size_t stringlen = strlen(string);
	size_t suffixlen = strlen(suffix);
	if(suffixlen > stringlen)
	{
		return 0;
	}
	return strncmp(string + stringlen - suffixlen, suffix, suffixlen) == 0;
}

int git_release_string_utility_substr(const char* source, int len, char** out)
{
	size_t source_len = strlen(source);
	if(source_len < abs(len))
	{
		return 1;
	}
	size_t out_len = source_len - abs(len);
	*out = xmalloc(out_len);
	if(len > 0)
	{
		memcpy(*out, source + len, out_len);
	}
	else if(len <= 0)
	{
		memcpy(*out, source, out_len);
	}
	(*out)[out_len] = '\0';
	return 0;
}