#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){
    
    while(1){
        char str[50];
        int n = 0;
        char *cargs[10];
        char *token;
        int fd[2];
        pipe(fd);
        const char *delim = " \n\t";

        printf("Enter your command: ");
        fgets(str, 256, stdin);

        token = strtok(str,delim);
        if (strcmp(token,"exit") == 0) break;
        while(1) 
        {
            cargs[n++] = token;            
            if (token == NULL) break;
            token = strtok(NULL, delim);
        }
        int k = 0;
        char *cargs1[10];//first command
        for(int i = 0; i < n; i++){
        	k++;
        	if(strcmp(cargs[i],"|") == 0) break;//command without |
    		cargs1[i] = cargs[i];
    	}
    	cargs1[k] = NULL;
    	char *cargs2[10];//second command
    	for(int i = k; i < n; i++){
			cargs2[i - k] = cargs[i];
		}   
 		int status;
 		int rc = fork();	
		if(rc == 0){//child process #1						
			dup2(fd[1], 1);
			close(fd[1]); close(fd[0]);
			execvp(cargs1[0], cargs1);
		}
		else{//rc==1 parent process
			int rc2 = fork();
			if(rc2 == 0){//child #2
				close(fd[1]);	
				dup2(fd[0], 0);
				close(fd[0]);
	            execvp(cargs2[0],cargs2);
			}
			else{//parent
				close(fd[0]);
				close(fd[1]);
				waitpid(rc, &status, 0);
				waitpid(rc2, &status, 0);
			}
		}
	}
    return 0;
}