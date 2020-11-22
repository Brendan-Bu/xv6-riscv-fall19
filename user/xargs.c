#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
int main(int argc, char *argv[])
{
  int i, j, pid,blk;
  char args[MAXARG][32];
  char *a[MAXARG],buf;
  if(argc < 2) {
    printf("Illegal Input\n");
    exit();
  }
  while(1){
    i = 0;
    blk = 0;//处理词之间多个空格情形
    memset(args, 0, MAXARG * 32);
    for(j = 1; j < argc; j++) {
      strcpy(args[i++], argv[j]);
    }
    j = 0;  
    while (i < MAXARG-1) {
      if (read(0, &buf, 1)<= 0) {
        wait(); 
        exit();
      }
      if (buf == '\n') {
        printf("enter meet\n");
        break;
      }
      if (buf == ' ') {
        printf("space meet\n");
        if (blk) {
          blk = 0;  
          i++;
          j = 0;
        }
        continue;
      }
      args[i][j++] = buf;
      blk = 1;
    }
    printf("already enter in\n");
    for (i = 0; i < MAXARG-1; i++) {
      a[i] = args[i];
    }
    a[MAXARG-1] = 0;
    if ((pid = fork()) == 0) {
      exec(args[0], a);
      exit();
    }
  }
}