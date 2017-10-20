#include<time.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <getopt.h>
#include<stdio.h>
#include <sys/types.h>
#include<sys/wait.h>

int main(int argc, char **argv){
	int c;
	char *fib = "5", *fact = "5";
	int status;
	while(1){
		const char* short_options = "m:n:";
		c = getopt(argc, argv, short_options);
		if(c == -1) break;
		switch(c){
			case 'n':
				fact = optarg;
				break;
			case 'm':
				fib = optarg;
				break;
		}
	}
	int rc = fork();	
	if(rc < 0){
		exit(1);
	}
	else{
		if(rc == 0){//child process #1						
		    execl("/usr/bin/python","/usr/bin/python","fact.py", fact, (char*) NULL); 
		}
		else{//rc==1 parent process
			int rc2 = fork();
			int status2;
			if (rc2 < 0){
				exit(1);
			}
			else{
				if(rc2 == 0){//child #2
					execl("/usr/bin/java","/usr/bin/java","fib", fib, (char*) NULL);
				}
				else{//rc == 2 parent process	
					waitpid(0, &status, 0);
					waitpid(0, &status2, 0);
				}

			}
		}	
	}
	return 0;
}

