#include <git2.h>
#include <regex.h>
#include <string.h>
#include <stdio.h>
#include "semvers.h"
#include "memory.h"

const char* git_release_semvers_regex_pattern = "v([0-9])+\\.([0-9])+\\.([0-9])+(.*)";
const int major_position = 1;
const int minor_position = 2;
const int patch_position = 3;

regex_t* git_release_semvers_regex = NULL;
static int git_release_semvers_get_semvers_regex(regex_t** out)
{
	if(!git_release_semvers_regex)
	{
		git_release_semvers_regex = xmalloc(sizeof(regex_t));
		if(regcomp(git_release_semvers_regex, git_release_semvers_regex_pattern, REG_EXTENDED)) 
		{
			die("Impossible to compile the regex for semvers");
		}
	}
	*out = git_release_semvers_regex;
	return 0;
}
static int git_release_semvers_regex_match(const char* value, regmatch_t** out)
{
	regex_t* regex = NULL;
	git_release_semvers_get_semvers_regex(&regex);
	size_t nmatch = regex->re_nsub;
	regmatch_t* pmatch = xmalloc (sizeof (*pmatch) * nmatch);
	int match = regexec(regex, value, nmatch, pmatch, 0);
	if(match == REG_NOMATCH)
	{
		return ENOMATCH;
	}
	else if(match)
	{
		die("Could not match");
	}
	*out = pmatch;
	return 0;
}
static int git_release_semvers_extract_int_from_pmatch(const char* value, const regmatch_t* pmatch, int* out)
{
	char *str = NULL;
	int start = pmatch->rm_so;
	int end = pmatch->rm_eo;
	size_t size = end - start;
	   
	str = xmalloc(sizeof(*str) * (size + 1));
	strncpy(str, &value[start], size);
	str[size] = '\0';
	*out = strtol(str, NULL, 10);
	
	return 0;
}
static int git_release_semvers_get_version_number(const char* tag, int version_label, int* out)
{
	int err;
	regmatch_t* pmatch = NULL;
	if((err = git_release_semvers_regex_match(tag, &pmatch)))
	{
		return err;
	}
	if(version_label == MAJOR)
	{
		git_release_semvers_extract_int_from_pmatch(tag, &pmatch[major_position], out);
	}
	else if(version_label == MINOR)
	{
		git_release_semvers_extract_int_from_pmatch(tag, &pmatch[minor_position], out);
	}
	if(version_label == PATCH)
	{
		git_release_semvers_extract_int_from_pmatch(tag, &pmatch[patch_position], out);
	}
	
	return 0;
}
static int git_release_semvers_increment_version_number(const char* tag, int version_label, char** out)
{
	int err;
	int major;
	int minor;
	int patch;
	if((err = git_release_semvers_get_major(tag, &major)))
	{
		return err;
	}
	if((err = git_release_semvers_get_minor(tag, &minor)))
	{
		return err;
	}
	if((err = git_release_semvers_get_patch(tag, &patch)))
	{
		return err;
	}
	if(version_label == MAJOR)
	{
		major++;
		minor = 0;
		patch = 0;
	}
	else if(version_label == MINOR)
	{
		minor++;
		patch = 0;
	}
	else if(version_label == PATCH)
	{
		patch++;
	}
	*out = xmalloc(12 * sizeof(char));
	int len = snprintf(*out, 12, "v%d.%d.%d", major, minor, patch);
	if(len >= 12)
	{
		*out = xmalloc((len + 1) * sizeof(char));
		snprintf(*out, len + 1, "v%d.%d.%d", major, minor, patch);
	}
	return 0;
}

//
// Header function below
//

int git_release_semvers_get_major(const char* tag, int* out)
{
	return git_release_semvers_get_version_number(tag, MAJOR, out);
}
int git_release_semvers_get_minor(const char* tag, int* out)
{
	return git_release_semvers_get_version_number(tag, MINOR, out);
}
int git_release_semvers_get_patch(const char* tag, int* patch)
{
	return git_release_semvers_get_version_number(tag, PATCH, patch);
}
int git_release_semvers_increment_major(const char* tag, char** out)
{
	return git_release_semvers_increment_version_number(tag, MAJOR, out);
}
int git_release_semvers_increment_minor(const char* tag, char** out)
{
	return git_release_semvers_increment_version_number(tag, MINOR, out);
}
int git_release_semvers_increment_patch(const char* tag, char** out)
{
	return git_release_semvers_increment_version_number(tag, PATCH, out);
}
