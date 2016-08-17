#ifndef SEMVERS_H
#define SEMVERS_H
#include <regex.h>

#define ETAGNOTFOUND 1 // No tag could be found
#define ENOMATCH 2 // Regex could not match

enum VERSION_LABEL
{
	MAJOR, MINOR, PATCH
};

int git_release_semvers_get_major(const char* tag, int* out);
int git_release_semvers_get_minor(const char* tag, int* out);
int git_release_semvers_get_patch(const char* tag, int* out);
int git_release_semvers_increment_major(const char* tag, char** out);
int git_release_semvers_increment_minor(const char* tag, char** out);
int git_release_semvers_increment_patch(const char* tag, char** out);


#endif 
