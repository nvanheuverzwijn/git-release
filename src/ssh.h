#ifndef SSH_H
#define SSH_H

typedef struct git_release_ssh_key_pair {
	char* public_key_path;
	char* private_key_path;
} git_release_ssh_key_pair;

typedef struct git_release_ssh_key_pair_array {
	git_release_ssh_key_pair** pairs;
	int count;
} git_release_ssh_key_pair_array;

void git_release_ssh_free_ssh_key_pair_array(git_release_ssh_key_pair_array* arr);
void git_release_ssh_free_ssh_key_pair(git_release_ssh_key_pair* pair);
int git_release_ssh_list_file_in_home(git_release_ssh_key_pair_array** out);


#endif
