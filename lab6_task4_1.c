#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[], char* envp[])
{
	int fd;
	size_t size;
	char resstring[14];
	char name[] = "test.fifo";

	(void)umask(0);

	if((fd = open(name,O_RDONLY)) < 0){
		printf("Can\'t open fifo\n");
		return -1;
}

	size = read(fd,resstring,14);
	if (size < 0){
		printf("Can\'t read\n");
		return -1;
}

	printf("fifo resstring: %s\n",resstring);

	if (close(fd) < 0){
		printf("child: Can\'t close");
		return -1;
}

    return 0;
}
