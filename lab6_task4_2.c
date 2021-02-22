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
	char name[] = "test.fifo";
	(void)umask(0);
	if(mknod(name,S_IFIFO | 0666,0) < 0){
		printf("cannot create\n");
		return -1;
}
	
	if((fd = open(name,O_WRONLY)) < 0){
		printf("cannot open");
		return -1;
}

	size = write(fd, "Hello, world!", 14);

	if (size != 14){
		printf("cannot write");
		return -1;
}

	if(close(fd) < 0){
		printf("parent: cannot close");
		return -1;
}

    return 0;
}
