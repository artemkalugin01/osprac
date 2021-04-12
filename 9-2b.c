#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

int main()
{
    int* arr;
    long i;
    int shmid;
    int semid;
    int n = 1;
    key_t key;
    struct sembuf buf;
    char path[] = "9-2a.c";
    if ((key = ftok(path, 0)) < 0) {
        printf("Cannot generate key\n");
        return -1;
    }
    if ((shmid = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("cannot create shared memory\n");
            return -1;
        }
        else {
            if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0) {
                printf("Cannot find shared memory\n");
                return -1;
            }
            n = 0;
        }
    }
    if ((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Semaphore does not exist.\n");
            return -1;
        }
        else {
            if ((semid = semget(key, 1, 0)) < 0) {
                printf("cannot get semaphore by key\n");
                return -1;
            }
        }
    }
    else {
        buf.sem_num = 0;
        buf.sem_flg = 0;
        buf.sem_op = 1;
        if (semop(semid, &buf, 1) < 0) {
            printf("Cannot initialize this semaphore to 1\n");
            return -1;
        }
    }
    if ((int*)(-1) == (arr = (int*)shmat(shmid, NULL, 0))) {
        printf("Cannot connect to shared memory in program B\n");
        return -1;
    }
    buf.sem_num = 0;
    buf.sem_flg = 0;
    buf.sem_op = -1;
    if (semop(semid, &buf, 1) < 0) {
        printf("Cannot enter the critical section\n");
        return -1;
    }
    if (n) {
        arr[0] = 0;
        arr[1] = 1;
        arr[2] = 1;
    }
    else {
        arr[1] += 1;
        for (i = 0; i < 2000000000L; i++);
        arr[2] += 1;
    }
    printf
    ("First program was called %d times, second program - %d times, total - %d times\n",
        arr[0], arr[1], arr[2]);
    buf.sem_num = 0;
    buf.sem_flg = 0;
    buf.sem_op = 1;
    if (semop(semid, &buf, 1) < 0) {
        printf("Unable to exit the critical section correctly\n");
        return -1;
    }
    if (shmdt(arr) < 0) {
        printf("Unable to detach shared memory\n");
        return -1;
    }
    return 0;
}
