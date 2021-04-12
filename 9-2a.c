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
    int semid;
    int shmid;
    int n = 1;
    char path[] = "9-2a.c";
    key_t key;
    struct sembuf mybuf;
    if ((key = ftok(path, 0)) < 0) {
        printf("Cannot generate key\n");
        return -1;
    }
    if ((shmid = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Cannot create shared memory\n");
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
            printf("Semaphore does not exist\n");
            return -1;
        }
        else {
            if ((semid = semget(key, 1, 0)) < 0) {
                printf("Unable to get semaphore by key\n");
                return -1;
            }
        }
    }
    else {
        mybuf.sem_num = 0;
        mybuf.sem_flg = 0;
        mybuf.sem_op = 1;
        if (semop(semid, &mybuf, 1) < 0) {
            printf("Cannot set original value of this semaphore to 1\n");
            return -1;
        }
    }
    if ((int*)(-1) == (arr = (int*)shmat(shmid, NULL, 0))) {
        printf("Cannot connect shared memory \n");
        return -1;
    }
    mybuf.sem_num = 0;
    mybuf.sem_flg = 0;
    mybuf.sem_op = -1;
    if (semop(semid, &mybuf, 1) < 0) {
        printf("Cannot enter the critical section \n");
        return -1;
    }
    if (n) {
        arr[0] = 1;
        arr[1] = 0;
        arr[2] = 1;
    }
    else {
        arr[0] += 1;
        for (i = 0; i < 2000000000L; i++);
        arr[2] += 1;
    }
    printf
    ("First program was called %d times, second program - %d times, total - %d times\n",
        arr[0], arr[1], arr[2]);
    mybuf.sem_num = 0;
    mybuf.sem_flg = 0;
    mybuf.sem_op = 1;
    if (semop(semid, &mybuf, 1) < 0) {
        printf("Unable to exit correctlyA\n");
        return -1;
    }
    if (shmdt(arr) < 0) {
        printf("Unable to detach shared memory\n");
        return -1;
    }
    return 0;
}
