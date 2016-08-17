#include <git2.h>
#include "branch.h"
#include "errors.h"

int git_release_branch_exist(git_repository* repo, const char* name)
{
	int return_code = 0;
	git_reference* reference;
	if(git_branch_lookup(&reference, repo, name, GIT_BRANCH_REMOTE))
	{
		return_code = E_BRANCHNOTFOUND;
	}
	git_reference_free(reference);
	return return_code;
}

