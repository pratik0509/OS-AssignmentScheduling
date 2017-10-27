#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int main (int argc,char *argv[])
{
  int pid;
  int status, a, b, c;
  pid = fork();
  if(pid == 0){
    int prer = set_priority(5);
    printf(1, "Previous: %d\n", prer);
    prer = set_priority(6);
    printf(1, "Previous: %d\n", prer);
    exec(argv[1], argv);
    printf(1, "exec %s failed\n", argv[1]);
  }else{
    status = waitx(&a, &b, &c);
    printf(1, "Wait Time = %d\n Run Time = %d Total time = %d with PID = %d \n", a, b, c, status);
  }
  exit();
}
