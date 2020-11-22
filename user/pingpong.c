#include "kernel/types.h"
#include "user/user.h"
int main(){
    int p1[2],p2[2];
    char buffer1[] = "ping";
    char buffer2[] = "pong";
    long length = sizeof(buffer1);
    pipe(p1);
    pipe(p2);
    if(fork() == 0){
		if(read(p1[0], buffer1, length) != length){
			printf("read from p1 error!");
			exit();
		}
		printf("%d: received %s\n", getpid(),buffer1);
		if(write(p2[1], buffer2, length) != length){
			printf("write to p2 error!");
			exit();
		}
        exit();
    }
	else{
		if(write(p1[1], buffer1, length) != length){
		printf("write to p1 error!");
		exit();
	}
	if(read(p2[0], buffer2, length) != length){
		printf("read from p2 error!");
		exit();
	}
	printf("%d: received %s\n", getpid(),buffer2);
    wait();
	exit();
	}
	return 0;
}