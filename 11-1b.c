#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>


int main() {
    long typ = 255;
	int msqid;

	char pathname[] = "11-1a.c";

	key_t key;

	int len, maxlen;

	struct mymsgbuf {
		long mtype;
		struct {
			short sinfo;
			float finfo;
		} info;
	} mybuf;

	if ((key = ftok(pathname, 0)) < 0) {
		printf("Cannot generate key\n");
		return -1;
	}

	if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
		printf("Cannot get msqid\n");
		return -1;
	}

	while (2) {
		maxlen = sizeof(mybuf.info);
		if (msgrcv(msqid, &mybuf, maxlen, 0, 0) < 0) {
			printf("Can't receive message from queue\n");
			return -1;
		}

		if (mybuf.mtype == typ) {
			msgctl(msqid, IPC_RMID, NULL);
			return -1;
		}

		printf("message type = %ld, sInfo = %i, fInfo = %f\n", mybuf.mtype, mybuf.info.sinfo, mybuf.info.finfo);
	}

	return 0;
}