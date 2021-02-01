#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid = fork();
  if (pid == -1){
  printf("Ошибка! Создать процесс не удалось.");
  }
  else if (pid == 0){
  pid = getpid();
  printf("It is a child. pid = %d",(int)pid);
  }
  else{
  pid = getpid();
  printf("It is a Parent. pid = %d",(int)pid);
  }
  

  return 0;
}
