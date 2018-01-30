#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#ifndef DIE
#define DIE do {printf("Failure.\n"); exit(EXIT_FAILURE)}
#endif


extern char **getln();

//--------------Definitions---------
void executeCommand(char ** args);

void add(char ** args);
void sub(char ** args);
void arg(char ** args);

char ** getArgs(char ** args, char * inputFile, char * outputFile);
void strLower(char ** args);
int numOfArgs(char ** args);
bool isBackgroundProcess(char ** args);
bool redirectIO(char ** args);

int i = 0;

main() {
	char **args;

	signal (SIGCHLD, NULL);
	while(1) {
		printf(">");
		args = getln();
		if(args[0] == NULL){
			printf("Shell: Invalide Argument.\n");
			continue;
		}
		strLower(args);

		if(strcmp("exit",args[0])==0){
			exit(0);
		}else if (strcmp("add",args[0])==0) {
			add(args);
		}else if (strcmp("sub",args[0])==0) {
			sub(args);
		}else if (strcmp("arg",args[0])==0) {
			arg(args);
		}else{
			if(isBackgroundProcess(args)){
				executeCommand(args);
			}else if(!isBackgroundProcess(args)){
				executeCommand(args);
				signal(SIGCHLD,SIG_IGN);
			}
		}
	}
	exit(1);
}

void executeCommand(char ** args){
	char * inputFile = malloc(sizeof(char)*100);
	char * outputFile = malloc(sizeof(char)*100);

	char ** argums = getArgs(args,inputFile,outputFile);

	pid_t ID;
	int pipefd[2];

	ID = fork();
	if (ID == -1) {
		printf("SHELL: Error executing command.\n");
		exit(1);
	}else if (ID > 0) {
		wait(NULL);
	}else if (ID == 0) {
		if (redirectIO(args)) {
			if(inputFile != NULL){
				int fd = open(inputFile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			    dup2(fd, 1);
			    dup2(fd, 2);
			    close(fd);
			}else if(outputFile != NULL){
				pipe(pipefd);
				close(pipefd[0]);
			    dup2(pipefd[1], 1);
			    dup2(pipefd[1], 2);
			    close(pipefd[1]);
			}
			execvp(args[0],argums);
		}else{
			execvp(args[0],argums);
		}
	}else{
		char buffer[1024];
	    close(pipefd[1]);
	    while (read(pipefd[0], buffer, sizeof(buffer)) != 0){
	    }
	}

	if (argums != NULL) {
		free(argums);
	}
	if(inputFile != NULL){
		free(inputFile);
	}
	if (outputFile != NULL) {
		free(inputFile);
	}
}
//------additional functionality -------
void add(char ** args){
	int num;
	int convertNum;
	char * string = malloc(sizeof(char) * 100);

	for (int i = 1; args[i]!= NULL; i++){
		string = args[i];
		convertNum = strtol(string, NULL, 0);
		num = convertNum + num;
	}

	printf("The answer is %d\n",num);
	free(string);
}
void sub(char ** args){
	int subNum;
	int getSubNum;
	char * subString = malloc(sizeof(char) * 100);

	for (int i = 1; args[i]!=NULL; i++){
		subString = args[i];
		getSubNum = strtol(subString, NULL, 0);
		subNum = getSubNum - subNum;
		subNum--;

	}

	printf("The answer is %d\n",subNum);
	free(subString);
}
void arg(char ** args){
	int argsCount = 0;
	int j = 0;
	for(j = 0; args[j] != NULL; j++) {
		printf("Argument %d: %s\n", j, args[j]);
		argsCount = argsCount + 1;
	}
}
//------Utility functions---------------
char ** getArgs(char ** args, char * inputFile, char * outputFile){
	int j = 0;
	char ** returnArgs = malloc(sizeof(char*)*100);
	for (int i = 1; args[i] != NULL; i++){
		if(strcmp(">",args[i])==0){
			strcpy(outputFile,args[i+1]);
			i = i + 1;
		}else if (strcmp("<",args[i])==0) {
			strcpy(inputFile,args[i + 1]);
			i = i + 1;
		}else{
			returnArgs[j] = args[i];
			j++;
		}
	}
	return returnArgs;
}
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
