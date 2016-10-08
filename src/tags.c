#include <git2.h>
#include <string.h>
#include "tags.h"
#include "errors.h"
#include "memory.h"

int git_release_tag_get_last(char** out, git_repository *repo)
{
	char* last_tag = NULL;
	git_strarray tags = {0};
	if(git_tag_list(&tags, repo))
	{
		die("Could not fetch tag list");
	}
	if(tags.count == 0)
	{
		git_strarray_free(&tags);
		return E_TAGNOTFOUND;
	}
	last_tag = tags.strings[tags.count - 1];
	size_t last_tag_memlen = strlen(last_tag) + sizeof(char);

	*out = xmalloc(last_tag_memlen);
	strncpy(*out, last_tag, last_tag_memlen);

	git_strarray_free(&tags);

	return 0;
}
