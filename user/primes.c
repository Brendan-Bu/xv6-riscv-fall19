#include "kernel/types.h"
#include "user/user.h"

void prime(int *address, int num){
	int p[2],i;
	printf("prime %d\n",*address);
	if(num==1) return;
	pipe(p);
    if(fork() == 0){
        for(i = 0; i < num; i++)
        {
			write(p[1],  address + i, sizeof(int));
	    }
        exit();
    }
    else
    {
      /* code */
		int next[34];
		int num = 0;
	    int buffer;
		close(p[1]);
		while(read(p[0], &buffer, sizeof(int)) != 0)
        {
		    	if(buffer % *address != 0)
               {
				   *(next+num) = buffer;
				   num++;
			   }
		    }
		  prime(next, num);
	  	exit();
  } 	
   wait();
}

int main(){
    int first[34];
	for(int i = 0; i < 34; i++)
    {
		first[i] = i+2;
	}
	prime(first, 34);
  exit();
}