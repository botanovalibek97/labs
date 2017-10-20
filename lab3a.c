#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>

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
        int status;
        if (x == 0){
            if(strcmp(cargs[n-2],"&") == 0){
                cargs[n-2] = NULL;
            }
            execvp(cargs[0], cargs);    
        } 
        else if (x > 0){
            if (strcmp(cargs[n-2],"&") != 0) 
            {
                waitpid(x, &status, 0);
            }
        } 
    }
    return 0;
}