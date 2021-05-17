#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <sys/file.h>
#include <errno.h>


int main() {
    int pid_file = open("11-3-server.c", O_CREAT | O_RDWR, 0666);
    int rc = flock(pid_file, LOCK_EX | LOCK_NB);

    if (!rc){
    int msqid;
    char pathname[] = "11-3-server.c";
    key_t key;
    int msgSz, maxMsgSz;

    struct requestDataBuf {
        long mtype;
        struct {
            pid_t pid;
            double message;
        } info;
    } clientBuf;

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

    while (1) {
        int msgMaxLenght = sizeof(clientBuf.info);
        if (msgrcv(msqid, (struct clientmsgbuf *) &clientBuf, msgMaxLenght, -2, 0) < 0) {
            printf("Cannot receive message\n");
            return(-1);
        }
    if(clientBuf.mtype ==2){
            printf("Killing server!\n\n");
    return(-1);
    }
        printf("Received:\nPID = %d\nData = %lf\n", clientBuf.info.pid, clientBuf.info.message);
        double num = clientBuf.info.message * clientBuf.info.message;
        printf("Response: %lf\n", num);

        serverBuf.mtype = clientBuf.info.pid;
        serverBuf.info.message = num;
        int len = sizeof(serverBuf.info);
        if (msgsnd(msqid, (struct sermsgbuf *) &serverBuf, len, 0) < 0) {
            printf("Cannot send message\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            return(-1);
        }
        printf("Response sent\n");
    }
    return 0;
    }
    else{
        printf("Anouther instance of server is running!\n");
        return -1;
    }
}