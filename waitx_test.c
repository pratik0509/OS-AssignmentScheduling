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
  		exec(argv[1], argv);
    	printf(1, "exec %s failed\n", argv[1]);
    }else{
      status = waitx(&a, &b, &c);
      printf(1, "Wait Time = %d\n Run Time = %d Total time = %d with Status %d \n", a, b, c, status);
 	}
 	exit();
}
