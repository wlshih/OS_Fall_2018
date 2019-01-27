#include <stdio.h>
#include <unistd.h>

int main(void) {
	pid_t child;
	int i;

	printf("Main process pid = %d\n", getpid());

	child = fork(); // fork0
	waitpid(child, NULL, 0);
	printf("%d --> %d\n", getppid(), getpid());
	for (i=1; i<3; i++) {
		if (child == 0) {
			child = fork(); // fork1
			waitpid(child, NULL, 0);
			child = fork(); // fork2
			waitpid(child, NULL, 0);
			printf("%d --> %d & %d\n", getppid(), getpid(), i);
		}
		else if (child > 0) {
			child = fork(); // fork3
			waitpid(child, NULL, 0);
			printf("%d --> %d * %d\n", getppid(), getpid(), i);
		}
		else {
			perror("error: ");
		}
	}

	//printf("This is a process, my pid = %d, my parent is %d\n", getpid(), getppid());

	return 0;
}