#ifndef TAGS_H
#define TAGS_H
#include <regex.h>

int git_release_tag_get_last(git_repository *repo, char** out);

#endif 
