#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <wait.h>

void handler(int nsig) {
    pid_t pid;
    int status;

    while (1) {
        pid = waitpid(-1, &status, WNOHANG);
        if (pid <= 0) {
            return;
        }
        if ((status & 0xff) == 0) {
            printf("Process %d finished with code %d.\n", pid, status >> 8);
        }
        else if ((status & 0xff00) == 0) {
			if ((status & 0x80))
			{
				printf("Process %d killed by a signal %d, core file %s.\n", pid, status & 0x7f, "included");
			}
			else{
				printf("Process %d killed by a signal %d, core file %s.\n", pid, status & 0x7f, "not included");
			}
			
            
        }
    }
}

int main() {
    pid_t pid;
    signal(SIGCHLD, handler);

    for (int i = 0; i < 10; ++i) {
        if ((pid = fork()) < 0) {
            printf("Parent: unable to fork child %d.\n", i);
            exit(-1);
        }
        if (pid == 0) {
            for (int j = 1; j < 10000000; ++j);
            exit(200 + i);
        }
    }

    while (1);
    return 0;
}