#include <stdio.h>
#include <unistd.h>

int main() {
	pid_t child;

	printf("Main process id : %d.\n", getpid());

	child = fork(); // fork1
	
	if (child < 0)
		perror("fork1 error : ");
	else if (child == 0) {
		printf("Fork%d, I'm the child %d, my parent is %d.\n", 1, getpid(), getppid());
		child = fork(); // fork2
		
		if (child < 0)
			perror("fork2 error : ");
		else if (child == 0) {
			printf("Fork%d, I'm the child %d, my parent is %d.\n", 2, getpid(), getppid());
		}
		else {
			waitpid(child, NULL, 0);
		}
	}
	else {
		waitpid(child, NULL, 0);
	}
	
	child = fork(); // fork3
	
	if (child < 0)
		perror("fork3 error : ");
	else if (child == 0) {
		printf("Fork%d, I'm the child %d, my parent is %d.\n", 3, getpid(), getppid());
	}
	else {
		waitpid(child, NULL, 0);
	}
	
	return 0;
}