#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 11 /* Hint */
#define PUZZLE_SIZE 9

/* example puzzle */
int puzzle[PUZZLE_SIZE+1][PUZZLE_SIZE+1] = {
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0}
		};

/* data structure for passing data to threads */
typedef struct
{
	int thread_num;
    int x;
	int y;
} parameters;

/* print puzzle */ 
#define LINE "====================== \n"
#define COLLINE "\n___________________________________ \n"
#define NAME "||  SUM   CHECKER  || \n"
void print_grid(int grid[10][10])
{
    int i,j;
    printf(LINE);
    printf(NAME);
    printf(LINE);

	for (i = 1; i < 10; i++)
	{
		for (j = 1; j < 10; j++)
		{
	        printf("|%1d |",grid[i][j]);
		}
        printf(COLLINE);
	}
    printf("\n");
}
// read file to check sudoku
void SetPuzzle(char filename[]){
    FILE *file = fopen(filename,"r");
    int i,j,ch,num;
    for (i=0;i<=9;i++){
        for(j=0;j<=9;j++){
            while(((ch = getc(file)) != EOF)){
                if(ch == EOF) break;
                if(ch == ',') break;
                if(ch == '\n') break;
                ungetc(ch,file);
                fscanf(file,"%d",&num);
                if(num!=-1)
                    puzzle[i][j] = num;            
            } 
        }
    }
    print_grid(puzzle);
    return;
}

void* row_check(void*);
void* col_check(void*);
void* subgrid_check(void*);

int checker = 0;
int rv = 1;

int main(int argc, char* argv[])
{
	//int rv = 1; // flag to check answer
    // input the sudoku file
    SetPuzzle("test.txt");

    pthread_t thread[NUMBER_OF_THREADS-1];
    int n;
    int i=0, j=0, s=0, t=0;

    for(n=0; n<NUMBER_OF_THREADS; n++) {
        if(rv) {
            // set arguments
            parameters* arguments = (parameters*)malloc(sizeof(parameters));
            arguments->thread_num = n;
            arguments->x = 0;
            arguments->y = 0;
            switch(n) {
                case NUMBER_OF_THREADS-1: // checking rows
                    row_check((void*)arguments);
                    break;
                case NUMBER_OF_THREADS-2: // checking columns
                    pthread_create(&thread[NUMBER_OF_THREADS-2], NULL, col_check, (void*)arguments);
                    break;
                default: // checking sub-grids
                    arguments->x = (n/3)*3;
                    arguments->y = (n%3)*3;
                    pthread_create(&thread[n], NULL, subgrid_check, (void*)arguments);
            }
        }
    }
    for(n=0; n<NUMBER_OF_THREADS-1; n++) {
        pthread_join(thread[n], NULL);
    }
   
	if (rv == 1)
		printf("Successful :) \n");
	else {
		printf("Must check again! :( \n");
        //printf("Error code: %d\n", rv);
    }

	return 0;
}

// thread row checking
void* row_check(void* arguments) {
    int i, j, sum;
    parameters* args = (parameters*)arguments;
    //sleep(2);
    for(i=9; i>0; i--) {
        sum = 0;
        for(j=9; j>0; j--) {
            sum += puzzle[i][j];
        }
        if(checker == 0) checker = sum;
        if(sum != checker) {
            rv = -1;
            //printf("Error code: %d(%d)\n(sum, checker): (%d, %d)\n", rv, i, sum, checker);
            break;
        }
    }
}
// thread column checking
void* col_check(void* arguments) {
    int i, j, sum;
    parameters* args = (parameters*)arguments;
    //sleep(2);
    for(j=9; j>0; j--) {
        sum = 0;
        for(i=9; i>0; i--) {
            sum += puzzle[i][j];
        }
        if(checker == 0) checker = sum;
        if(sum != checker) {
            rv = -2;
            //printf("Error code: %d(%d)\n", rv, j);
            break;
        }
    }
}
// thread sub-grid checking
void* subgrid_check(void* arguments) {
    int i, j, sum=0;
    parameters* args = (parameters*) arguments;
    //sleep(2);
    for(i=3; i>0; i--) {
        for(j=3; j>0; j--) {
            sum += puzzle[args->x+i][args->y+j];
        }
    }
    if(checker == 0) checker = sum;
    if(sum != checker) {
        rv = -3;
        //printf("Error code: %d(%d)\n(sum, checker): (%d, %d)\n", rv, args->thread_num, sum, checker);
    }
}