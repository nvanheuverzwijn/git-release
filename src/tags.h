#ifndef TAGS_H
#define TAGS_H
#include <regex.h>

int git_release_tag_get_last(char** out, git_repository *repo);

#endif 
