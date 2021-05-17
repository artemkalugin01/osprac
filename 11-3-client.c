#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main() {

    int msqid;
    char pathname[] = "11-3-server.c";
    key_t key;

    struct requestDataBuf {
        long mtype;
        struct {
            pid_t pid;
            double message;
        } info;
    } clientBuf;
    clientBuf.mtype = 1;
    clientBuf.info.pid = getpid();

    struct responseDataBuf {
        long mtype;
        struct {
            double message;
        } info;
    } serverBuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Cannot generate key\n");
        return -1;
    }
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Cannot get msqid\n");
        return -1;
    }

    double num;
    printf("Input number:\n> ");
    scanf("%lf", &num);

    int msgSz = sizeof(clientBuf.info);
    clientBuf.info.message = num;
    printf("PID: %d\nMessage: %lf;\n", clientBuf.info.pid, clientBuf.info.message);
    if (msgsnd(msqid, (struct requestDataBuf *) &clientBuf, msgSz, 0) < 0) {
        printf("Cannot send message\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        return -1;
    }

    printf("Awaiting response...\n");

    int maxMsgSz = sizeof(serverBuf.info);
    if (msgrcv(msqid, &serverBuf, maxMsgSz, getpid(), 0) < 0) {
        printf("Cannot receive message\n");
        return -1;
    }
    printf("Received message :  %lf;\n", serverBuf.info.message);

    return 0;
}