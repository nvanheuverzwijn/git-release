#ifndef TAGS_H
#define TAGS_H
#include <regex.h>

#define ETAGNOTFOUND 1 // No tag could be found
#define ENOMATCH 2 // Regex could not match

int git_release_tag_get_last(git_repository *repo, char** out);

#endif 
