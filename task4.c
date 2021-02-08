#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[],char* envp[])
{
  pid_t pid = fork();
  int start_pid = getpid();
  fork();

  if(getppid() == start_pid){
    execle("task4_executable","hello executable",0,envp);
    printf("Execl failure!");
}
  

  return 0;
}
