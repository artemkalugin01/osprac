#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_NAME_SIZE 32

void count_max_link_depth(int recursive_depth) {
	char previous_file_name[BUFFER_NAME_SIZE];
	char current_file_name[BUFFER_NAME_SIZE];

	sprintf(previous_file_name, "rec_count_test%d", recursive_depth);
	sprintf(current_file_name, "rec_count_test%d", recursive_depth + 1);

	if (symlink(previous_file_name, current_file_name) != 0) {
		printf("Cannot create symlink. \n");
		exit(-1);
	}
	recursive_depth++;

	int file_descriptor;
	if ((file_descriptor = open(current_file_name, O_RDWR | O_CREAT, 0666)) < 0) {
		printf("Max recursive link depth was reached.\n");
		printf("Recursive depth is %d\n", recursive_depth - 1);
		if (remove(current_file_name) < 0) {
			printf("Unable to remove a link.\n");
			exit(-1);
		}
		return;
	}
	if (close(file_descriptor) < 0) {
		printf("Cannot close the file.\n");
		exit(-1);
	}

	count_max_link_depth(recursive_depth);
	if (remove(current_file_name) < 0) {
		printf("Cannot remove a link.\n");
		exit(-1);
	}
}

int main() {
	char file_name[] = "rec_count_test0";
	int file_descriptor;

	if ((file_descriptor = open(file_name, O_RDWR | O_CREAT, 0666)) < 0) {
		printf("Cannot open the file.\n");
		return -1;
	}
	if (close(file_descriptor) < 0) {
		printf("Cannot close file.\n");
		return -1;
	}

	count_max_link_depth(0);
	if (remove(file_name) < 0) {
		printf("Cannot remove initial file.\n");
		return -1;
	}

	return 0;
}