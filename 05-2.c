#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
int par[2], child[2], result;
size_t size;
char resstring[14];
char string[] = "Hello, World!";

if (pipe(par) < 0) {
printf("Can\'t open pipe of the parent!\n");
exit(-1);
}

if (pipe(child) < 0) {
printf("Can\'t open pipe of the child!\n");
exit(-1);
}

result = fork();

if (result < 0) {
printf("Can\'t fork!\n");
exit(-1);
} else if (result > 0) {
// Parent

if (close(par[0]) < 0) {
printf("Can\'t close reading side of parent pipe\n");
exit(-1);
}

if (close(child[1]) < 0) {
printf("Can\'t close writing side of child pipe\n");
exit(-1);
}

size = write(par[1], string, 14);

if (size != 14) {
printf("Can\'t write all string to pipe\n");
exit(-1);
}

if (close(par[1]) < 0) {
printf("Can\'t close writing of parent\n");
exit(-1);
}

size = read(child[0], resstring, 14);

if (size != 14) {
printf("Can\'t read child!\n");
exit(-1);
}

if (close(child[0]) < 0) {
printf("Can\'t close reading from child\n");
exit(-1);
}

printf("Parent reads child, output: %s \n", resstring);
printf("Parent exit\n");

} else {
// Child

if (close(par[1]) < 0) {
printf("Can\'t close writing from parent\n");
exit(-1);
}

if (close(child[0]) < 0) {
printf("Can\'t close reading from child\n");
exit(-1);
}

size = read(par[0], resstring, 14);

if (size < 0) {
printf("Can\'t read from parent\n");
exit(-1);
}

printf("Child: %s \n", resstring);

size = write(child[1], resstring, 14);

if (close(child[1]) < 0) {
printf("Can\'t close writing of child\n");
exit(-1);
}

if (close(par[0]) < 0) {
printf("Can\'t close reading from parent\n");
exit(-1);
}

printf("Child exit\n");
}
return 0;
}
