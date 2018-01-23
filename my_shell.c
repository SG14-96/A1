#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#ifndef DIE
#define DIE do {printf("Failure.\n"); exit(EXIT_FAILURE)}
#endif

extern char **getln();
//--------------DEfinitions---------
void executeCommand(char ** args);

int add();
int arg();

void strLower(char ** args);
int numOfArgs(char ** args);
bool isBackgroundProcess(char ** args);
int redirectIO(char ** args);

main() {
	int parentPID;
	int status;
	int i;
	char **args; 
	
	while(1) {
		printf(">");
		args = getln();
		strLower(args);
		int numArgs = numOfArgs(args);
		
		if(strcmp("exit",args[0])==0){
			//close processes.
			exit(0);
		}else{
			pid_t childPID;
			if(childPID = fork() == 0){
				if(isBackgroundProcess(args)){
					executeCommand(args);
				}else if(!isBackgroundProcess(args)){
					executeCommand(args);
					parentPID = wait(&status);
				}
			}
		}
	}
}

void executeCommand(char ** args){
	int pipeLink;
	pid_t childPID;

	if (pipe(pipeLink) == -1){
		DIE;
	}
	if (childPID = fork() == -1){
		DIE;
	}

	if (childPID == 0){
		/* code */
	}else{
		close(pipeLink);
		wait(NULL);
	}
	wait(1000);
}

//------additional functionality -------
int add(){
	return 0;
}
int arg(){
	return 0;
}
// Make your won.
//------Utility functions---------------
void strLower(char ** args){
	for(int i = 0; args[i] != NULL; i++) {
		char * string = args[i];
		for(int j = 0; string[j] != '\0'; j++){
  			string[j] = tolower(string[j]);
		}
	}
}

int numOfArgs(char ** args){
	int i = 0;
	for (i = 0; args[i] != NULL; i++){
		/* code */
	}
	return i;
}

bool isBackgroundProcess(char ** args){
	for(int i = 0; args[i] != NULL; i++){
  		if(strcmp("&",args[i]) == 0){
  			return true;
  		}
	}
	return false;
}

int redirectIO(char ** args){
	for(int i = 0; args[i] != NULL; i++){
  		if(strcmp(">",args[i]) == 0){
  			return 1;
  		}else if(strcmp("<",args[i]) == 0){
  			return -1;
  		}else{
  			return 0;
  		}
	}
}