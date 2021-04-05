#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
int main()
{
    int *array;
    int id;
    char path[] = "7-2.c";
    key_t key;
    char *source_code;
    
    int file_size = 0;
    int *file_size_ptr;
    
    if ((key = ftok(path,0)) < 0) {
        printf("Cannot generate key\n");
        return -1;
    }
    
    if ((id = shmget(key, sizeof(int) + file_size * sizeof(char), 0)) < 0) {
        printf("Cannot create memory\n");
        return -1;
    }
    if ((file_size_ptr = (int *)shmat(id, NULL, 0)) == (int *)(-1)) {
        printf("Cannot attach memory\n");
        return -1;
    }
    file_size = *file_size_ptr;
    source_code = (char*)(file_size_ptr + 1);
    
    for (int i = 0; i < file_size; i++)
        putchar(source_code[i]);
    
    if (shmdt(file_size_ptr) < 0) {
        printf("Cannot detach memory\n");
        return -1;
    }
    
    if (shmctl(id, IPC_RMID, NULL) < 0) {
        printf("Cannot delete memory\n");
        return -1;
    }
    return 0;
}
