#ifndef SEMVERS_H
#define SEMVERS_H
#include <regex.h>

enum VERSION_LABEL
{
	MAJOR, MINOR, PATCH
};

int git_release_semvers_get_major(int* out, const char* tag);
int git_release_semvers_get_minor(int* out, const char* tag);
int git_release_semvers_get_patch(int* out, const char* tag);
int git_release_semvers_increment_major(char** out, const char* tag);
int git_release_semvers_increment_minor(char** out, const char* tag);
int git_release_semvers_increment_patch(char** out, const char* tag);


#endif 
