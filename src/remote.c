#include <git2.h>
#include "remote.h"
#include "errors.h"

int git_release_remote_fetch(git_repository* repo, const char* name, git_fetch_options* fetch_options)
{
	int return_code = 0;
	git_remote* remote_reference = NULL;
	if(git_remote_lookup(&remote_reference, repo, name))
	{
		return_code = E_REMOTENOTFOUND;
		goto free_and_return;
	}
	if(git_remote_fetch(remote_reference, NULL, fetch_options, NULL))
	{
		return_code = E_REMOTENOTFETCH;
		goto free_and_return;
	}
free_and_return:
	git_remote_free(remote_reference);
	return return_code;
}
