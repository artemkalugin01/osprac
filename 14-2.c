#include <stdio.h>
#include <signal.h>

void handler(int nsig) {
	if (nsig == SIGINT) {
		printf("Signal: %d. Ctrl+C is ignored.\nContinue running\n", nsig);
	}
	else if (nsig == SIGQUIT) {
		printf("Signal: %d. Ctrl+4 ignored.\nContinue running\n", nsig);
	}
}

int main() {
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	while (1);
	return 0;
}