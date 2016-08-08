#ifndef TAGS_H
#define TAGS_H
#include <regex.h>

#define ETAGNOTFOUND 1 // No tag could be found
#define ENOMATCH 2 // Regex could not match

enum VERSION_LABEL
{
	MAJOR, MINOR, PATCH
};

int git_release_tag_get_last(git_repository *repo, char** out);
int git_release_tag_get_major(const char* tag, int* out);
int git_release_tag_get_minor(const char* tag, int* out);
int git_release_tag_get_patch(const char* tag, int* out);
int git_release_tag_increment_major(const char* tag, char** out);
int git_release_tag_increment_minor(const char* tag, char** out);
int git_release_tag_increment_patch(const char* tag, char** out);


#endif 
