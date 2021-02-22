#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[], char* envp[])
{
    int fd[2];
    int pipe_size = 0;

	if (pipe(fd) < 0){
		printf("Can\'t create pipe\n");
		return -1;
}
	fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFD | O_NONBLOCK));

	while ( write(fd[1], "1", 1) == 1){pipe_size++;}

	printf("size of pipe: %db \n",pipe_size);

    return 0;
}
