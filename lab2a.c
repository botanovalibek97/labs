#include<time.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <getopt.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
int calcFact(int value){
	int res = 1;
	int i = 0;
	for(i = 1; i <= value; i++){
		res *= i;
	}
	return res;
}8
int calcFib(int value){
	int fibs[value+1];
	fibs[0] = 0;
	fibs[1] = 1;
	for(int i = 2; i < value + 1; i++){
		fibs[i] = fibs[i-1] + fibs[i-2];
	}
	return fibs[value]; 
} 


int main(int argc, char **argv){
	int c;
	int fib = 5, fact = 5;
	int status;
	while(1){
		const char* short_options = "m:n:";
		c = getopt(argc, argv, short_options);
		if(c == -1) break;
		switch(c){
			case 'n':
				fact = atoi(optarg);
				break;
			case 'm':
				fib = atoi(optarg);
				break;
		}
	}
	int rc = fork();	
	if(rc < 0){
		exit(1);
	}
	else{
		if(rc == 0){//child process #1						
			printf("fact(%d)=%d\n", fact, calcFact(fact));
		}
		else{//rc==1 parent process
			int rc2 = fork();
			if (rc2 < 0){
				exit(1);
			}
			else{
				if(rc2 == 0){//child #2
					printf("fib(%d)=%d\n", fib, calcFib(fib));
				}
				else{
					waitpid(0, &status, 0);
					waitpid(0, &status, 0);
					printf("Good work!");	
				}

			}
		}	
	}
	
	return 0;
}

