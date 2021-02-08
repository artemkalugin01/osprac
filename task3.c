#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[],char *envp[])
{
  while(envp*){
		printf("%s\n",*envp);
		envp++;
}
  return 0;
}
