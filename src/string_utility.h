#ifndef STRING_UTILITY_H
#define STRING_UTILITY_H

int git_release_string_utility_startswith(const char* string, const char* prefix);
int git_release_string_utility_endswith(const char* string, const char* suffix);
int git_release_string_utility_substr(const char* source, int len, char** out);

#endif
