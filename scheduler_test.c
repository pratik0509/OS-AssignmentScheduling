#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

#define MAX1 100000000
#define MAX2 100000000
#define MAX3 100000000
int rndm = 1;

int rand()
{
  rndm = (rndm * 557) % 7919;
  return rndm;
}

void srand(int x)
{
  rndm = x;
}

int main (int argc,char *argv[])
{
  int pid1, pid2;
  pid1 = fork();
  int i;
  int j;
  int k;
  if(pid1 == 0){
    srand(2);
    int priority = rand() % 100;
    while(1) {
      i = 0;
      j = 0;
      k = 0;
      while(k < MAX3){
        while(j < MAX2){
          while(i < MAX1)
            ++i;
          ++j;
        }
        ++k;
      }
      printf(1, "Previous priority of the process 1 was: %d\n", set_priority(priority));
      priority = rand() % 100;
    }
  }else{
    pid2 = fork();
    if(pid2 == 0){
      srand(7);
      int priority = rand() % 100;
      while(1) {
        i = 0;
        j = 0;
        k = 0;
        while(k < MAX3){
          while(j < MAX2){
            while(i < MAX1)
              ++i;
            ++j;
          }
          ++k;
        }
        printf(1, "Previous priority of the process 2 was: %d\n", set_priority(priority));
        priority = rand() % 100;
      }
    }else{
      printf(1, "Parent 1\n");
    }
  }
  exit();
}
