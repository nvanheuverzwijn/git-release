#ifndef SSH_H
#define SSH_H

typedef struct git_release_ssh_key_pair {
	char* public_key_path;
	char* private_key_path;
	char* type;
	int encrypted;
} git_release_ssh_key_pair;

typedef struct git_release_ssh_key_pair_array {
	git_release_ssh_key_pair** pairs;
	int current;
	int count;
} git_release_ssh_key_pair_array;

void git_release_ssh_free_ssh_key_pair_array(git_release_ssh_key_pair_array* arr);
void git_release_ssh_free_ssh_key_pair(git_release_ssh_key_pair* pair);
int git_release_ssh_get_ssh_folder_from_current_user_home_directory(char** out);
int git_release_ssh_key_pair_array_current(git_release_ssh_key_pair** out, git_release_ssh_key_pair_array* ssh_key_pair_array);
int git_release_ssh_key_pair_array_next(git_release_ssh_key_pair** out, git_release_ssh_key_pair_array* ssh_key_pair_array);
int git_release_ssh_list_keys_in_folder(git_release_ssh_key_pair_array** out, const char* ssh_directory);


#endif
