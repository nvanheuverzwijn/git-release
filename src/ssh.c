#include <stdlib.h>
#include <stdio.h>	
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "ssh.h"
#include "errors.h"
#include "memory.h"
#include "string_utility.h"

static int git_release_ssh_get_home_directory(char** out)
{
	char* home_directory = getenv("HOME");

	if(!home_directory)
	{
		struct passwd* pwd = getpwuid(getuid());
		if(pwd)
		{
			home_directory = pwd->pw_dir;
		}
		else
		{
			return E_SSHHOMENOTFOUND;
		}
	}
	
	*out = home_directory;
	return 0;
}

int git_release_ssh_get_ssh_folder_from_current_user_home_directory(char** out)
{
	int out_len = 0;
	int return_code = 0;
	char* home_directory = NULL;
	if((return_code = git_release_ssh_get_home_directory(&home_directory)))
	{
		return return_code;
	}
	out_len = strlen(home_directory) + strlen("/.ssh") + sizeof(char);
	*out = xmalloc(out_len);
	snprintf(*out, out_len, "%s%s", home_directory, "/.ssh");
	return return_code;
}

void git_release_ssh_free_ssh_key_pairs(git_release_ssh_key_pair** pairs, int count)
{
	if(pairs == NULL)
	{
		return;
	}
	for(int i = 0; i < count; i++)
	{
		git_release_ssh_free_ssh_key_pair(pairs[i]);
	}
}

void git_release_ssh_free_ssh_key_pair_array(git_release_ssh_key_pair_array* arr)
{
	if(arr == NULL)
	{
		return;
	}
	git_release_ssh_free_ssh_key_pairs(arr->pairs, arr->count);
	free(arr->pairs);
	free(arr);
}

void git_release_ssh_free_ssh_key_pair(git_release_ssh_key_pair* pair)
{
	if(pair == NULL)
	{
		return;
	}
	free(pair->public_key_path);
	free(pair->private_key_path);
	free(pair);
}

int git_release_ssh_list_file_in_home(char* ssh_directory, git_release_ssh_key_pair_array** out)
{
	git_release_ssh_key_pair_array* arr = xmalloc(sizeof(git_release_ssh_key_pair_array));
	DIR *dp = NULL;
	struct dirent *ep = NULL;
	int return_code = 0;
	if(ssh_directory == NULL)
	{
		if((return_code = git_release_ssh_get_ssh_folder_from_current_user_home_directory(&ssh_directory)))
		{
			goto free_and_return;
		}
	}
	dp = opendir(ssh_directory);
	arr->count = 0;
	arr->pairs = NULL;
	if(dp != NULL)
	{
		while((ep = readdir(dp)))
		{
			if(ep->d_type == DT_REG && git_release_string_utility_endswith(ep->d_name, ".pub"))
			{
				int directory_len = strlen(ssh_directory) + sizeof(char) + strlen(ep->d_name) + sizeof(char);

				arr->pairs = xrealloc(arr->pairs, sizeof(arr->pairs) * (arr->count + 1));
				arr->pairs[arr->count] = xmalloc(sizeof(git_release_ssh_key_pair));
				arr->pairs[arr->count]->public_key_path = xmalloc(directory_len);
				snprintf(arr->pairs[arr->count]->public_key_path, directory_len, "%s/%s", ssh_directory, ep->d_name);

				/* remove `.pub` from public key to get the associated private key*/
				git_release_string_utility_substr(arr->pairs[arr->count]->public_key_path, -4, &arr->pairs[arr->count]->private_key_path);

				arr->count += 1;
			}
		}
		closedir(dp);
		*out = arr;
	}
free_and_return:
	free(ssh_directory);
	return return_code;
}
