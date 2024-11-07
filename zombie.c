#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  //array of process ids
  pid_t pids[10];
  int i;
  for (i=9;i>=0;i--){
    pids[i]=fork();
    if(pids[i]==0){
      sleep(i+1);
      //graceless termination of the process
      //no cleanup effect no buffered output flushing
      _exit(0);
    }
  }

  for(i=9;i>=0;--i){
    printf("parent%d\n",i);
    waitpid(pids[i],NULL ,0);
  }
}
