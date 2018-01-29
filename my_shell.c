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

<<<<<<< HEAD
//--------------Definitions---------
=======
pid_t childProcs[100];
//--------------DEfinitions---------
>>>>>>> bf166b4f08884fd2af2ab202754e5fe62e516c2f
void executeCommand(char ** args);
void killProcs(int i);

int add(char ** args);
int sub(char ** args);
void arg(char ** args);

void strLower(char ** args);
int numOfArgs(char ** args);
bool isBackgroundProcess(char ** args);
bool redirectIO(char ** args);


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
	pid_t parentPID;
	int status;
<<<<<<< HEAD
	char **args;

	signal (SIGCHLD, NULL);
=======
	char **args; 
	
	signal (SIGCHLD, killProc);
>>>>>>> bf166b4f08884fd2af2ab202754e5fe62e516c2f
	while(1) {
		printf(">");
		args = getln();
		if(args[0] == NULL){
			printf("Shell: Invalide Argument.\n");
			continue;
		}
		strLower(args);
		int numArgs = numOfArgs(args);

		if(strcmp("exit",args[0])==0){
<<<<<<< HEAD
			//killProcs(i);
=======
			killProcs(i);
>>>>>>> bf166b4f08884fd2af2ab202754e5fe62e516c2f
			exit(0);
		}else if (strcmp("add",args[0])==0) {
			int result = add(args);
		}else if (strcmp("sub",args[0])==0) {
			int result = sub(args);
		}else if (strcmp("arg",args[0])==0) {
			arg(args);
		}else{
<<<<<<< HEAD
			if(!isBackgroundProcess(args)){
				executeCommand(args);
			}else if(isBackgroundProcess(args)){
				executeCommand(args);
				parentPID = wait(&status);
=======
			if(childPID[i] = fork() == 0){
				executeCommand(args);
>>>>>>> bf166b4f08884fd2af2ab202754e5fe62e516c2f
			}
			if(!isBackgroundProcess(args)){
				parentPID = wait(&status);
			}
			i++;
		}
	}
	exit(1);
}

void executeCommand(char ** args){
<<<<<<< HEAD
	pid_t ID;

	int j = 0;
	int i = 0;
	char ** arguments = malloc(sizeof(char*)*100);
	arguments[j] = args[i];
	arguments[j + 1] = args[i + 1];
	arguments[j + 2] = NULL;

	ID = fork();
	if (ID == -1) {
		printf("SHELL: Error executing command.\n");
		exit(1);
	}else if (ID > 0) {
		wait(NULL);
	}else if (ID == 0) {
		if (redirectIO(args)) {
			char * inputFile = NULL;
			char * outputFile = NULL;
			
			/* code */
			// call freopn then call exec.
		}else{
			execvp(arguments[j],arguments);
		}
	}
	free(arguments);
=======
	//
}

void killProcs(int i){
	for (int j = 0; j == i; j++){
		sigkill(childProcs[j]);
	}
>>>>>>> bf166b4f08884fd2af2ab202754e5fe62e516c2f
}
//------additional functionality -------
int add(char ** args){
	return 0;
}
int sub(char ** args){
	return 0;
}
void arg(char ** args){
	//
}
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
			args[i] = NULL;
  			return true;
  		}
	}
	return false;
}

bool redirectIO(char ** args){
	for(int i = 0; args[i] != NULL; i++){
  		if((strcmp(">",args[i]) == 0) || (strcmp("<",args[i]) == 0)){
  			return true;
  		}else{
  			return false;
  		}
	}
}
