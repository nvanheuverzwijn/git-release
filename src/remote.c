#include <git2.h>
#include "remote.h"
#include "errors.h"
#include <stdio.h>

int git_release_remote_fetch(git_repository* repo, const char* name)
{
	int return_code;
	git_remote* remote_reference = NULL;
	if(git_remote_lookup(&remote_reference, repo, name))
	{
		return_code = E_REMOTENOTFOUND;
	}
	int ret = 0;
	if((ret = git_remote_fetch(remote_reference, NULL, NULL, NULL)))
	{
		printf("\n%i\n", ret);
		return_code = E_REMOTENOTFETCH;
	}
	git_remote_free(remote_reference);
	return return_code;
}
