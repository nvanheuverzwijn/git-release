#include <git2.h>
#include "tags.h"
#include "errors.h"

int git_release_tag_get_last(git_repository *repo, char** out)
{
	git_strarray tags = {0};
	if(git_tag_list(&tags, repo))
	{
		die("Could not fetch tag list");
	}
	if(tags.count == 0)
	{
		return E_TAGNOTFOUND;
	}
	*out = tags.strings[tags.count - 1];
	return 0;
}
