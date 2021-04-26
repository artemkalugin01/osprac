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

    int len;

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

    for (int i = 0; i < 5; i++) {
        mybuf.mtype = 1;
        mybuf.info.sinfo = 1111 + i;
        mybuf.info.finfo = 11.11 + i;
        len = sizeof(mybuf.info);

        if (msgsnd(msqid, &mybuf, len, 0) < 0) {
            printf("Cannot send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            return -1;
        }
    }

    mybuf.mtype = typ;
    len = 0;
    if (msgsnd(msqid, &mybuf, len, 0) < 0) {
        printf("Cannot send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        return -1;
    }

    return 0;
}