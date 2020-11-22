#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#define MAX 100
int number=0;
void getCommand(char *input, char* argv[],int* num)
{
    char args[10][50];
    for(int i=0;i<10;i++){
        argv[i]=&args[i][0];
    }
    int k=0;int j=0; 
    while(input[j] != '\0')
    {
        while (input[j]==' '){
            j++;
        }
        argv[k++]=input+j;
        while (input[j]!=' '&&input[j]!='\n'){
            j++;
        }
        input[j]='\0';
        j++;
    }
    argv[k]=0;
    *num=k;   
}

void run(char*argv[],int num)
{ 
    for(int i=1;i<num;i++){
        if(argv[i][0]=='>'){ // 输出重定向
            close(1);
            open(argv[i+1],O_CREATE|O_WRONLY);
            argv[i]=0;
        }
        if(argv[i][0]=='<'){ // 输入重定向
            close(0);
            open(argv[i+1],O_RDONLY);
            argv[i]=0;
        }
    }
    exec(argv[0], argv);
}
int checkPipe(char*argv[],int i,int num){
    int fd[2];
    int j;
    pipe(fd);
    number++;
    for(;i<num;i++){
        if(argv[i][0]=='|'){
            argv[i]=0;// '|'换成'\0'
            break;
        }
    }
    if(i==num&&number==1) return 0;
    for(j=i+1;j<num;j++)
    {
        if(argv[j][0]=='|') break;
    }
    if(fork()==0){
        close(1);
        dup(fd[1]);
        close(fd[0]);
        close(fd[1]);
        run(argv,i);
    }else{
        close(0);
        dup(fd[0]);
        close(fd[0]);
        close(fd[1]);
        if(j==num) run(argv+i+1,num-i-1);
        else checkPipe(argv,j,num);
    }  
    return 1;
}
int main()
{
    char buf[MAX];
    char* argv[10];
    
    buf[0]=1;
    while (buf[0]!='\0'){
        int num=-1;
        fprintf(2,"@ ");
        memset(buf, 0, sizeof(buf));
        gets(buf,sizeof(buf));
        if (fork() == 0){
            getCommand(buf, argv,&num);
            //printf("%s %d\n",argv,argc);
            if(!checkPipe(argv,0,num))
            run(argv,num);
        }
        wait(0);
    }
    exit(0);
}