#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <git2.h>
#include "errors.h"
#include "tags.h"
#include "semvers.h"
#include "memory.h"
#include "branch.h"
#include "remote.h"
#include "ssh.h"

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif


static int progress_cb(const char *str, int len, void *data)
{
	(void)data;
	printf("remote: %.*s", len, str);
	fflush(stdout); /* We don't have the \n to force the flush */
	return 0;
}

static int update_cb(const char *refname, const git_oid *a, const git_oid *b, void *data)
{
	char a_str[GIT_OID_HEXSZ+1], b_str[GIT_OID_HEXSZ+1];
	(void)data;

	git_oid_fmt(b_str, b);
	b_str[GIT_OID_HEXSZ] = '\0';

	if (git_oid_iszero(a)) {
		printf("[new]		 %.20s %s\n", b_str, refname);
	}
	else
	{
		git_oid_fmt(a_str, a);
		a_str[GIT_OID_HEXSZ] = '\0';
		printf("[updated] %.10s..%.10s %s\n", a_str, b_str, refname);
	}

	return 0;
}

static int transfer_progress_cb(const git_transfer_progress *stats, void *payload)
{
	if (stats->received_objects == stats->total_objects) {
		printf("Resolving deltas %d/%d\r",
		       stats->indexed_deltas, stats->total_deltas
		);
	}
	else if (stats->total_objects > 0) {
		printf("Received %d/%d objects (%d) in %zu bytes\r",
		       stats->received_objects, stats->total_objects,
		       stats->indexed_objects, stats->received_bytes
		);
	}
	return 0;
}

static int cred_acquire_from_ssh_agent_cb(git_cred **out,
		const char* UNUSED(url),
		const char* username_from_url,
		unsigned int UNUSED(allowed_types),
		void* UNUSED(payload))
{
	return git_cred_ssh_key_from_agent(out, username_from_url);
}

static int cred_acquire_from_ssh_key_cb(git_cred **out,
		const char* UNUSED(url),
		const char* username_from_url,
		unsigned int UNUSED(allowed_types),
		void* payload)
{
	if(payload == NULL)
	{
		die("cred_acquire_from_ssh_key_cb requires a payload.");
	}
	git_release_ssh_key_pair* ssh_pair = (git_release_ssh_key_pair*)payload;
	printf("Enter passphrase for key '%s' (empty if no passphrase):", ssh_pair->private_key_path);
	char* passphrase = getpass("");
	if(strcmp(passphrase, "") == 0)
	{
		passphrase = NULL;
	}
	return git_cred_ssh_key_new(
		out,
		username_from_url,
		ssh_pair->public_key_path,
		ssh_pair->private_key_path,
		passphrase
	);
}



int main(int argc, char* argv[])
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
	free(new_tag);

	git_release_semvers_increment_minor(tag, &new_tag);
	printf("%s\n", new_tag);
	free(new_tag);

	git_release_semvers_increment_patch(tag, &new_tag);
	printf("%s\n", new_tag);
	free(new_tag);

	if(git_release_branch_exist(repo, "origin/release/v2.10.0"))
	{
		printf("branch does not exist\n");
	}
	else
	{
		printf("branch exist\n");
	}

	git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;
	fetch_opts.callbacks.update_tips = &update_cb;
	fetch_opts.callbacks.sideband_progress = &progress_cb;
	fetch_opts.callbacks.transfer_progress = transfer_progress_cb;
	fetch_opts.callbacks.credentials = cred_acquire_from_ssh_agent_cb;
	if(git_release_remote_fetch(repo, "origin", &fetch_opts))
	{
		fetch_opts.callbacks.credentials = cred_acquire_from_ssh_key_cb;
		git_release_ssh_key_pair_array* ssh_pairs = NULL;
		git_release_ssh_list_file_in_home(&ssh_pairs);
		if(ssh_pairs->count != 0)
		{
			fetch_opts.callbacks.payload = ssh_pairs->pairs[0];
			if(git_release_remote_fetch(repo, "origin", &fetch_opts))
			{
				printf("Could not authenticate against the server. Make sure ssh-agent is running with your key\n");
			}
			else
			{
				printf("fetch success\n");
			}
		}
		else
		{
			printf("Cannot fetch: there is no ssh key in your home .ssh folder.");
		}
		git_release_ssh_free_ssh_key_pair_array(ssh_pairs);
	}
	else
	{
		printf("fetch success\n");
	}
	git_release_ssh_key_pair_array* ssh_pairs = NULL;
	git_release_ssh_list_file_in_home(&ssh_pairs);
	for(int i = 0; i < ssh_pairs->count; i++)
	{
		printf("%s\n", ssh_pairs->pairs[i]->private_key_path);
		printf("%s\n", ssh_pairs->pairs[i]->public_key_path);
	}
	git_release_ssh_free_ssh_key_pair_array(ssh_pairs);
	free(tag);

	return 0;
}
