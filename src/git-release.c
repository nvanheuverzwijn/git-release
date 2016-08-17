#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <git2.h>
#include "tags.h"
#include "semvers.h"
#include "memory.h"
#include "branch.h"
#include "remote.h"

int main(int argc, char *argv[])
{
    git_libgit2_init();

	char* tag = NULL;
	git_repository *repo = NULL;

	if(git_repository_open_ext(&repo, ".", GIT_REPOSITORY_OPEN_NO_SEARCH, NULL))
	{
		die("Current directory not a git directory");
	}
	if(git_release_tag_get_last(repo, &tag))
	{
		die("Could not get last tag");
	}
	printf("%s\n", tag);

	int major;
	if(git_release_semvers_get_major(tag, &major))
	{
		die("Could not get major number of tag");
	}
	printf("%d\n", major);

	int minor;
	if(git_release_semvers_get_minor(tag, &minor))
	{
		die("Could not get minor number of tag");
	}
	printf("%d\n", minor);

	int patch;
	if(git_release_semvers_get_patch(tag, &patch))
	{
		die("Could not get patch number of tag");
	}
	printf("%d\n", patch);

	char* new_tag = NULL;
	git_release_semvers_increment_major(tag, &new_tag);
	printf("%s\n", new_tag);

	git_release_semvers_increment_minor(tag, &new_tag);
	printf("%s\n", new_tag);

	git_release_semvers_increment_patch(tag, &new_tag);
	printf("%s\n", new_tag);

	if(git_release_branch_exist(repo, "origin/release/v2.10.0"))
	{
		printf("branch does not exist\n");
	}
	else
	{
		printf("branch exist\n");
	}

	int ret = 0;
	if((ret = git_release_remote_fetch(repo, "origin")))
	{
		printf("could not fetch: %i\n", ret);
	}
	else
	{
		printf("fetch success\n");
	}

	return 0;
}
