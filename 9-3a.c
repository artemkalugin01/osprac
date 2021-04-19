#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Описание.
 * В программе: 2 процесса, 1 общий pipe, 1 семафор
 * В программе в цикле выполняются N (где N >=2 вводится с клавиатуры в начале программы) раз
 * следующие действия: 1) чтение сообщения из pipe, 2) Запись сообщения в pipe
 *
 * Всё это работает следующим образом в несколько этапов:
 * 1. изначально sem_id равен 2
 * 2. Родитель вызывает функцию D как в начале, так и в конце итерации цикла
 * 3. Ребёнок вызывает функцию Z в начале итерации цикла, и функцию A в конце итерации цикла
 * Благодаря этому цикл корректно отрабатывает N раз
 * */

int try_semop(int sem_id, int sem_op) {
    struct sembuf mybuf;
    mybuf.sem_num = 0;
    mybuf.sem_op  = sem_op;
    mybuf.sem_flg = 0;

    if (semop(sem_id, &mybuf, 1) < 0) {
        printf("No condition\n");
        return -1;
    }
    return 0;
}

int A(int sem_id, int value) {
    return try_semop(sem_id, value);
}

int D(int sem_id, int value) {
    return try_semop(sem_id, -value);
}

int Z(int sem_id) {
    return try_semop(sem_id, 0);
}


int main()
{
    int     fd[2], result;
    size_t  size;

    char    message[14];
    int     n;

    printf("Enter loop lenght\n");
    scanf("%d",&n);

    if(n<2){
        printf("Not valid integer! Must be positive and not less than 2\n");
        return -1;
    }

    if (pipe(fd) < 0) {
        printf("Cannot open pipe\n");
        return -1;
    }

    result = fork();

    // Creating sem_id
    int    sem_id;
    char   pathname[]="9-3a.c";
    key_t  key;

    if ((key = ftok(pathname,0)) < 0) {
        printf("Can\'t generate key\n");
        return -1;
    }

    if ((sem_id = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Cannot create semaphore set\n");
            return -1;
        } else if ((sem_id = semget(key, 1, 0)) < 0) {
            printf("Cannot find semaphore\n");
            return -1;
        }
    } else {
        // Increase value of new semaphore by 2.
        A(sem_id, 2);
    }
    // sem_id created

    if (result < 0) {
        printf("Cannot fork child\n");
        return -1;
    } else if (result > 0) {


        for (int i = 1; i <= n; ++i) {
            D(sem_id, 1);

            if (i != 1) {
                size = read(fd[0], message, 14);
                if (size < 0) {
                    printf("Cannot read from pipe\n");
                    return -1;
                }
                printf("Parent read:%s\n", message);
            }

            // Write message to child.
            size = write(fd[1], "Hello, world! ", 14);
            if (size != 14) {
                printf("Cannot write pipe\n");
                return -1;
            }

            D(sem_id, 1);
        }

        printf("Parent exit\n");
    } else {
        /* Child process */

        for (int i = 0; i < n; ++i) {
            Z(sem_id);

            // Read message from parent.
            size = read(fd[0], message, 14);
            if (size < 0) {
                printf("Cannot read from pipe\n");
                return -1;
            }
            printf("Child read:%s\n",message);

            // Write message to parent.
            size = write(fd[1], "Hello, world! ", 14);
            if (size != 14) {
                printf("Cannot write to pipe\n");
                return -1;
            }

            A(sem_id, 2);
        }

        if (close(fd[0]) < 0) {
            printf("child: Cannot close reading pipe\n"); exit(-1);
        }
        if (close(fd[1]) < 0) {
            printf("child: Cannot close writing pipe\n"); exit(-1);
        }
        printf("Child exit\n");
    }

    return 0;
}

