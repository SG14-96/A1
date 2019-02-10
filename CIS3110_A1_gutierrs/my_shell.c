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
#include <math.h>

#ifndef DIE
#define DIE do {printf("Failure.\n"); exit(EXIT_FAILURE)}
#endif


extern char **getln();

//--------------Definitions---------
void executeCommand(char ** args, bool asBackground);
int numOfArgs(char ** args);

void arg(char ** args);
char ** formatArgs(char ** args);
char ** getFilenames(char ** args);
bool stringContains(char * string, const char * stringPattern);
void strLower(char ** args);
bool isBackgroundProcess(char ** args);
int redirectIO(char ** args);
int modifyIO(bool modInput,char * newfileInputName, bool modOutput,char * newfileOutputName);
void gcd(char ** args);
int hexToDec(char * hex);
int eaucledian(int num1, int num2);

int i = 0;

int main(void) {
	char username[1024];
	username[1023] = '\0';
	printf("%d\n",gethostname(username,1024));

	uid_t userEffectiveID = geteuid();

	char symbol;
	if (userEffectiveID != 0) {
		//program is running as user.
		symbol = '$';
	}else{
		//the program is running as root.
		symbol = '#';
	}
	char **args;

	signal (SIGCHLD, NULL);
	while(1) {
		printf("[%s]%c",username,symbol);
		args = getln();
		if(args[0] == NULL){
			printf("Shell: Invalide Argument.\n");
			continue;
		}
		
		if(strcmp("exit",args[0])==0){
			exit(0);
		}else if (strcmp("cd",args[0])==0) {
			int ret = chdir(args[1]);
			if (ret != 0) {
				printf("Could not change direcotry.\n");
			}
			
		}else if (strcmp("gcd",args[0])==0) {
			gcd(args);
		}else if (strcmp("args",args[0])==0) {
			arg(args);
		}else{
			if(isBackgroundProcess(args)){
				executeCommand(args,true);
			}else if(!isBackgroundProcess(args)){
				executeCommand(args,false);
			}
		}
	}
	exit(1);
}

void executeCommand(char ** args, bool asBackground){

	pid_t ID;

	ID = fork();
	if (ID == -1) {
		printf("SHELL: Error executing command.\n");
		exit(-1);
	}else if (ID > 0) {
		//parent process executes here.
		if (asBackground) {
			// return to the prompt.
		}else{
			wait(NULL);
		}
	}else if (ID == 0) {
		//child process executes here.
		if (redirectIO(args) == 1) {
			printf("io out\n");
			char ** fileNames = getFilenames(args);
			if (modifyIO(true,fileNames[0],false,NULL) != 0) {
				printf("ERROR:There was an issue modifying the IO.\n");
			}
			char ** formattedArgs = formatArgs(args);
			execvp(formattedArgs[0],formattedArgs);
			free(fileNames);
			free(formattedArgs);
		}else if(redirectIO(args) == 2){
			char ** fileNames = getFilenames(args);
			if (modifyIO(false,NULL,true,fileNames[1]) != 0) {
				printf("ERROR:There was an issue modifying the IO.\n");
			}
			char ** formattedArgs = formatArgs(args);
			execvp(formattedArgs[0],formattedArgs);
			free(fileNames);
			free(formattedArgs);
		}else if(redirectIO(args) == 3){
			char ** fileNames = getFilenames(args);
			if (modifyIO(true,fileNames[0],true,fileNames[1]) != 0) {
				printf("ERROR:There was an issue modifying the IO.\n");
			}
			char ** formattedArgs = formatArgs(args);
			execvp(formattedArgs[0],formattedArgs);
			free(fileNames);
			free(formattedArgs);
		}else{
			execvp(args[0],args);
		}
	}
}

//------Utility functions---------------
char ** formatArgs(char ** args){
	int numArgs = numOfArgs(args);
	char ** formattedArgs = malloc(sizeof(char*)*numArgs);
	int j = 0;
	for(int i = 0; i < numArgs; i++){
		if (!stringContains(args[i],">") && !stringContains(args[i],"<") && !stringContains(args[i],".txt")) {
			formattedArgs[j] = strdup(args[i]);
			j++;
		}
	}
	return formattedArgs;
}
char ** getFilenames(char ** args){
	char ** returnArgs = malloc(sizeof(char*)*2);
	returnArgs[0] = NULL;
	returnArgs[1] = NULL;
	for(int i = 0; args[i] != NULL; i++) {
		if ((strcmp(">",args[i]) == 0)) {
			i++;
			returnArgs[0] = strdup(args[i]);
		}
		if ((strcmp("<",args[i]) == 0)) {
			i++;
			returnArgs[1] = strdup(args[i]);
		}
	}
	return returnArgs;
}

bool stringContains(char * string, const char * stringPattern){
	int i, j=0, k;
	for(i=0; string[i]; i++){
		if(string[i] == stringPattern[j]){
		for(k=i, j=0; string[k] && stringPattern[j]; j++, k++)
			if(string[k]!=stringPattern[j])
				return false;;
			if(!stringPattern[j]){
				return true;
			}
		}
	}
	return false;
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
		//printf("arg:%s\n",args[i]);
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

int redirectIO(char ** args){
	bool redirectOUT = false;
	bool redirectIN = false;
	for(int i = 0; args[i] != NULL; i++){
  		if((strcmp(">",args[i]) == 0)){
  			redirectOUT = true;
  		}else if((strcmp("<",args[i]) == 0)){
			redirectIN = true;
		}
	}
	if (redirectOUT == true && redirectIN == true) {
		return 3;
	}else if(redirectOUT == true && redirectIN == false){
		return 1;
	}else if (redirectOUT == false && redirectIN == true) {
		return 2;
	}else{
		return 0;
	}
}

int modifyIO(bool modInput,char * newfileInputName, bool modOutput,char * newfileOutputName){
	if (modInput && newfileInputName != NULL) {
		int out = open(newfileInputName, O_WRONLY | O_CREAT, 0644);
		if (out == -1) {
			perror("open:");
			return -1;
		}
		int r;
		r = close(1); /* this closes stdout */
		if (r != 0) {
			perror("close:");
			return -1;
		}
		r = dup2(out, 1); /* this duplicates your file descriptor to stdout */
		if (r == -1) {
			perror("dup2:");
			return -1;
		}
		return 0;
	}
	if (modOutput && newfileOutputName != NULL) {
		int in = open(newfileOutputName, O_RDONLY | O_CREAT, 0644);
		if (in == -1) {
			perror("open:");
			return -1;
		}
		int r;
		r = close(0); /* this closes stdout */
		if (r != 0) {
			perror("close:");
			return -1;
		}
		r = dup2(in, 1); /* this duplicates your file descriptor to stdout */
		if (r == -1) {
			perror("dup2:");
			return -1;
		}
		return 0;
	}
	return 1;
}

int hexToDec(char * hex){
	int  dec,i;
    int  count;
    int  digit;
 
    count=0;
    dec=0;
    for(i=(strlen(hex)-1);i>=0;i--){
        switch(hex[i]){
            case 'A':
                digit=10; break;
            case 'B':
                digit=11; break;
            case 'C':
                digit=12; break;
            case 'D':
                digit=13; break;
            case 'E':
                digit=14; break;
            case 'F':
                digit=15; break;
            default:
                digit=hex[i]-0x30;
        }
        dec= dec+ digit*pow((double)16,(double)count);
        count++;
    }
    return dec;
}









//------additional functionality -------
void gcd(char ** args){
	if (numOfArgs(args) != 3) {
		printf("Only 2 arguments allowed.\n");
		return;
	}
	int num1 = 0;
	int num2 = 0;
	if (stringContains(args[1],"0x")) {
		num1 = hexToDec(args[1]);
	}else{
		num1 = atoi(args[1]);
	}
	if (stringContains(args[2],"0x")) {
		num2 = hexToDec(args[2]);
	}else{
		num2 = atoi(args[2]);
	}
	
	printf("GCD(%s,%s) = %d\n",args[1],args[2],eaucledian(num1,num2));
	
}

int eaucledian(int num1, int num2){
	if (num2 == 0) {
		return num1;
	}else{
		return eaucledian(num2,num1 % num2);
	}
}

void arg(char ** args){
	int argsCount = numOfArgs(args);
	printf("argc = %d,",argsCount);
	printf("argv = ");
	for(int i = 0; i < argsCount; i++){
		printf("%s,",args[i]);
	}
	printf("\n");
}