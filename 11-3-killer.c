#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
 
struct server
{
    long mtype;
    struct{
    	float msg;
    } info;
} serverbuf;
 
struct client
{
    long mtype;
    struct
    {
        pid_t pid;
        float msg;
    } info;
} clientBuf;
 
int main() {
    char pathname[] = "11-3-server.c";
    int msqid;
    key_t key;
 
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }
 
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get msqid\n");
        exit(-1);
    }
 
    clientBuf.mtype = 2;
    clientBuf.info.pid = getpid();
    clientBuf.info.msg = 0;
 
    printf("This program will kill server!");
 
    if (msgsnd(msqid, (struct clientBuf *) &clientBuf, sizeof(clientBuf.info), 0) < 0) {
        printf("Can't send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        return -1;
    }
 
    printf("Killed! \n");
 
    return 0;
}