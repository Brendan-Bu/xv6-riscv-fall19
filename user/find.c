#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
void find(char *path, char *findName){
	int fd;
	struct stat state;	
    char *point ,buf[100], *p;
	struct dirent dir;
	if((fd = open(path, 0)) < 0){
		printf("cannot open %s\n", path);
		return;
	}
    fstat(fd, &state); 
	if(state.type==2){	
            for(point=path+strlen(path); point >= path && *point != '/'; point--);
            point++;
			if(strcmp(point, findName) == 0){
		    printf("%s\n", path);
	       }
	   }	
	else if(state.type==1){
		strcpy(buf, path);
		p = buf+strlen(buf);
		*p++ = '/';
		while(read(fd, &dir, sizeof(dir)) == sizeof(dir)){
			if( strcmp(dir.name, "")==0 || strcmp(dir.name, ".")==0 || strcmp(dir.name, "..")==0)
				continue;				
			strcpy(p, dir.name);
			find(buf, findName);
			}
		}
	close(fd);		
	}
int main(int argc, char *argv[]){
	if(argc < 3){
		printf("Illegal Input\n");
		exit();
	}
	find(argv[1], argv[2]);
	exit();
}