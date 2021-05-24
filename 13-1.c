#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>


void create_bind(const char *const path, int path_len, int *const recursion_depth) {
    char *new_path = (char *) malloc((path_len + 2) * sizeof(char));
    strcpy(new_path, path);
    if (new_path[path_len - 1] == 'z' || new_path[path_len - 1] == 'Z') {
        strcat(new_path, "a");
        path_len++;
    } else {
        new_path[path_len - 1]++;
    
    if (symlink(path, new_path) == -1) {
        *recursion_depth = 0;
        return;
    }
    create_bind(new_path, path_len, recursion_depth);
    (*recursion_depth)++;
    unlink(new_path);
    free(new_path);
}

int main() {
    FILE *f;
    int depth;

    if ((f = fopen("a", "w")) == NULL) {
        printf("Cannot create file. ");
        return -1;
    }
    if (fputs("Original file", f) == EOF) {
        printf("Cannot write to file. ");
        return -1;
    }

    create_bind("a", 1, &depth);
    printf("Depth of recursion: %d", depth);

    if (fclose(f) == EOF) {
        printf("Cannot close file ");
        return -1;
    }
    return 0;
}