#include <stdlib.h>
#include <stdio.h>	
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include "ssh.h"
#include "errors.h"
#include "memory.h"
#include "string_utility.h"

static int git_release_ssh_public_key_type(char** out, char* public_key_path)
{
	FILE *file;
	file = fopen(public_key_path, "r");
	size_t len = 0;
	ssize_t bytes_read = getdelim(out, &len, ' ', file);
	if(bytes_read == -1)
	{
		fclose(file);
		free(out);
		return 1;
	}
	fclose(file);
	return 0;
}

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
			return 1;
		}
	}
	
	*out = home_directory;
	return 0;
}

static int git_release_ssh_is_folder(const char* folder)
{
	struct stat path_stat;
	if(stat(folder, &path_stat) || !S_ISDIR(path_stat.st_mode))
	{
		return 1;
	}
	return 0;
}

int git_release_ssh_get_ssh_folder_from_current_user_home_directory(char** out)
{
	int out_len = 0;
	char* home_directory = NULL; /* Don't bother to free this pointer because it will point to a protected memory segment which contains the home directory string*/
	if(git_release_ssh_get_home_directory(&home_directory))
	{
		return E_SSHHOMENOTFOUND;
	}
	out_len = strlen(home_directory) + strlen("/.ssh") + sizeof(char);
	*out = xmalloc(out_len);
	snprintf(*out, out_len, "%s%s", home_directory, "/.ssh");

	if(git_release_ssh_is_folder(*out))
	{
		return E_SSHFOLDERDOESNOTEXIST;
	}
	return 0;
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
	free(pairs);
}

void git_release_ssh_free_ssh_key_pair_array(git_release_ssh_key_pair_array* arr)
{
	if(arr == NULL)
	{
		return;
	}
	git_release_ssh_free_ssh_key_pairs(arr->pairs, arr->count);
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

int git_release_ssh_list_keys_in_folder(git_release_ssh_key_pair_array** out, const char* ssh_directory)
{
	git_release_ssh_key_pair_array* arr = xmalloc(sizeof(git_release_ssh_key_pair_array));
	DIR *dp = NULL;
	struct dirent *ep = NULL;

	dp = opendir(ssh_directory);
	if(dp != NULL)
	{
		arr->count = 0;
		arr->pairs = NULL;
		while((ep = readdir(dp)))
		{
			if(ep->d_type == DT_REG && git_release_string_utility_endswith(ep->d_name, ".pub"))
			{
				int directory_len = strlen(ssh_directory) + sizeof(char) + strlen(ep->d_name) + sizeof(char);

				arr->pairs = xrealloc(arr->pairs, sizeof(arr->pairs) * (arr->count + 1));
				arr->pairs[arr->count] = xmalloc(sizeof(git_release_ssh_key_pair));
				/* finding public key */
				arr->pairs[arr->count]->public_key_path = xmalloc(directory_len);
				snprintf(arr->pairs[arr->count]->public_key_path, directory_len, "%s/%s", ssh_directory, ep->d_name);
				/* finding private key */
				git_release_string_utility_substr(&arr->pairs[arr->count]->private_key_path, arr->pairs[arr->count]->public_key_path, -4);
				/* finding pairs type */
				git_release_ssh_public_key_type(&arr->pairs[arr->count]->type, arr->pairs[arr->count]->public_key_path);

				arr->count += 1;
			}
		}
		closedir(dp);
		*out = arr;
	}
	else
	{
		git_release_ssh_free_ssh_key_pair_array(arr);
		return E_SSHFOLDERDOESNOTEXIST;
	}
	return 0;
}
