#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_LINE 80

int setup(char inputBuffer[], char *arg[], int *background){
	int length, start, cnt, i;

	length = read(STDIN_FILENO, inputBuffer, MAX_LINE);
	//printf("%s\n", inputBuffer);
	start = -1;
	cnt = 0;
	*background = 0;
	if (length < 0){
       	perror("error reading the command");
		exit(-1);           /* terminate with error code of -1 */
  	}

	//input buffer
	for (i = 0; i < length; i++) {
		switch (inputBuffer[i]) {
			case ' ':
				if (start != -1) {
					arg[cnt] = &inputBuffer[start];
					cnt++;
				}
				start = -1;
				inputBuffer[i] = '\0';
				break;
			case '\t':
				if (start != -1) {
					arg[cnt] = &inputBuffer[start];
					cnt++;
				}
				start = -1;
				inputBuffer[i] = '\0';
				break;
			case '\n':
				if (start != -1) {
					arg[cnt] = &inputBuffer[start];
					cnt++;
				}
				start = -1;
				inputBuffer[i] = '\0';
				arg[cnt] = NULL;
				break;
			default:
				if (start == -1) {
					start = i;
				}
				if (inputBuffer[i] == '&') {
					*background = 1;
					inputBuffer[i] = '\0';
				}

		}
	}

	if (*background)
		arg[--cnt] = NULL;
	else 
		arg[cnt] = NULL;
	
	return 1;
}



int main(void)
{
	char *arg[MAX_LINE/2+1]; /*command line arguments*/
	char inputBuffer[MAX_LINE];
	int should_run = 1; /*flag to determine when to exit program*/
	pid_t child;

	int length, i, start;
	int cnt, background;
	
	while(should_run){
		printf("osh>");
		fflush(stdout);
	/** setup for input arguments **/

		should_run = setup(inputBuffer, arg, &background);

		if (strcmp(inputBuffer, "exit") == 0)
			return 0;

		//if(background) printf("process running in background...\n");

		child = fork();

		if (child < 0) {
			perror("fork error: ");
			break;
		}
		else if (child == 0) {
			execvp(arg[0], arg);
			printf("pid =%d\n", getpid());
		}
		// parent process
		else {
			if (background == 0) {
				waitpid(child, NULL, 0);
			}
		}

		/**
		* your code!
		* After reading user input, the step are:
		* (1) fork a child process using fork()
		* (2) the child process will invoke execvp()
		* (3) if command included &, parent will not invoke wait()
		* -----------------------------------------------------------
		* read(STDIN_FILENO, inputBuffer, MAX_LINE): read command line
		* fork(): create child process
		* execvp(char *command, char *params[]): execute system calls
		* wait()
		**/
	}

	return 0;
}

