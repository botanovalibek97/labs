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
        int x = fork();
        if (x > 0){
            if (strcmp(cargs[n-2],"&") == 0) 
            {
                cargs[n-2] = cargs[n-1]; n--;
                wait(NULL);
            }
        } 
        if (x == 0){
            if (n - 3 >= 0 && strcmp(cargs[n - 3], "2>") == 0){
                int fd = open(cargs[n-2],O_WRONLY|O_CREAT|O_TRUNC, 0644);   
                dup2(fd,STDERR_FILENO);
                close(fd);
                cargs[n-2] = cargs[n-1]; n--;
                cargs[n-2] = cargs[n-1]; n--;
            }
            if (n - 3 >= 0 && strcmp(cargs[n-3], ">") == 0){
                int fd = open(cargs[n-2],O_WRONLY|O_CREAT|O_TRUNC, 0644);   
                dup2(fd,STDOUT_FILENO);
                close(fd);
                cargs[n-2] = cargs[n-1]; n--;
                cargs[n-2] = cargs[n-1]; n--;
            } 
            else if (n - 3 >= 0 && strcmp(cargs[n-3], ">>") == 0){
                int fd = open(cargs[n-2],O_WRONLY|O_CREAT|O_APPEND, 0644);
                dup2(fd,STDOUT_FILENO);
                close(fd);
                cargs[n-2] = cargs[n-1]; n--;
                cargs[n-2] = cargs[n-1]; n--;
            }
            if (n-3 >= 0 && strcmp(cargs[n-3],"<") == 0){
                int fd = open(cargs[n-2],O_RDONLY, 0644);
                dup2(fd,STDIN_FILENO);
                close(fd);
                cargs[n-2] = cargs[n-1]; n--;
                cargs[n-2] = cargs[n-1]; n--;
            }
            execvp(cargs[0], cargs);
        } 
    }
    return 0;
}