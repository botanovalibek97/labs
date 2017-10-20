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
        char str[50];//command
        int n = 0;//all number of tokens
        char *cargs[100];//all tokens
        char *token;//every word from  split
        const char *delim = " \n\t";
        printf("Enter your command: ");//command is smth like this: echo hi | sed s/i/ey/g | sed s/y/ll/g | sed s/$/o/g | cat
        fgets(str, 256, stdin);

        token = strtok(str,delim);
        if (strcmp(token,"exit") == 0) break;
        while(1) 
        {
            cargs[n++] = token;            
            if (token == NULL) break;
            token = strtok(NULL, delim);
        }
        
        char *cmd[20][10];
        int n_childs = 0;
        int k = 0;

        for (int i = 0; i < n; i++)
        {
            if(i != n - 1 && strcmp(cargs[i],"|") == 0) {
                cmd[n_childs][k++] = NULL;
                n_childs++;
                k = 0;
            }       
            else
                cmd[n_childs][k++] = cargs[i];     
        }
        n_childs++;//in order to add last child too, cause when cycle ends last child not counted
        for(int i = 0; i < n_childs; i++){
            for(int j = 0; j < 10; j++){
                if(cmd[i][j] == NULL){
                    printf("NULL \n");
                    break;
                }
                else
                    printf("%s ", cmd[i][j]);
            }
        }

        int fd[2];
        pid_t pid;
        int fd_in = 0;

        for (int i = 0; i < n_childs; i++)
        {
            pipe(fd);
            if((pid = fork()) == -1){
                exit(EXIT_FAILURE);
            }
            else if(pid == 0){//child process
                dup2(fd_in, 0); //change the input according to the old one
                if(i != n_childs - 1)//checking whether we have next child
                    dup2(fd[1], 1);//if we have then STDOUT is fd[1], i.e. write function of pipe
                close(fd[0]);
                execvp(cmd[i][0], cmd[i]);
                exit(EXIT_FAILURE);
            }
            else{//parent process
                wait(NULL);//wait for child
                close(fd[1]);
                fd_in = fd[0];//save the input for the next command
            }
        }
    }
    return 0;
}



