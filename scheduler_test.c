#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int main (int argc,char *argv[])
{
  if(fork() == 0){
    int pid1, pid2;
    int status1, a1, b1, c1;
    int status2, a2, b2, c2;
    pid1 = fork();
    if(pid1 == 0){
      set_priority(7);
      sleep(20);
      exec(argv[1], argv);
      printf(1, "exec %s failed\n", argv[1]);
    }else{
      pid2 = fork();
      if(pid2 == 0){
        set_priority(17);
        sleep(20);
        exec(argv[1], argv);
        printf(1, "exec %s failed\n", argv[1]);
      }else{
        status1 = waitx(&a1, &b1, &c1);
        status2 = waitx(&a2, &b2, &c2);
        printf(1, "Wait Time = %d\n Run Time = %d Total time = %d with PID = %d \n", a1, b1, c1, status1);
        printf(1, "Wait Time = %d\n Run Time = %d Total time = %d with PID = %d \n", a2, b2, c2, status2);
        printf(1, "Child Priority 7 : %d, Child Priority 17 : %d\n", pid1, pid2);
      }
    }
  }else{
    int a,b,c,d;
    set_priority(1);
    a = waitx(&b,&c,&d);
    printf(1, "PID: %d, WTIME: %d, RTIME: %d, TTIME: %d\n", a, b, c, d);
  }
  exit();
}
