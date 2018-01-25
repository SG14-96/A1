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

pid_t childProcs[100];
//--------------DEfinitions---------
void executeCommand(char ** args);
void killProcs(int i);

int add();
int arg();

void strLower(char ** args);
int numOfArgs(char ** args);
bool isBackgroundProcess(char ** args);
int redirectIO(char ** args);

void killProc(){
	pid_t pid = getpid();
	for(int j = 0; j == i; j++){
		if (pid == childProcs[j]){
			childProcs[j] = NULL;
		}
	}
}

int i = 0;

main() {
	int parentPID;
	int status;
	char **args; 
	
	signal (SIGCHLD, killProc);
	while(1) {
		printf(">");
		args = getln();
		strLower(args);
		int numArgs = numOfArgs(args);
		
		if(strcmp("exit",args[0])==0){
			killProcs(i);
			exit(0);
		}else{
			if(childPID[i] = fork() == 0){
				executeCommand(args);
			}
			if(!isBackgroundProcess(args)){
				parentPID = wait(&status);
			}
			i++;
		}
	}
}

void executeCommand(char ** args){
	//
}

void killProcs(int i){
	for (int j = 0; j == i; j++){
		sigkill(childProcs[j]);
	}
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