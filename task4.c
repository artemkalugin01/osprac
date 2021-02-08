#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[])
{
    pid_t pid, ppid;

    int start_pid = getpid();
    fork();

    if(getppid() == start_pid){
        // Print current datetime
	execle("/bin/help", "help", 0, envp);

        printf("Execl failure!");
        exit(-1);
    }

    return 0;
}
