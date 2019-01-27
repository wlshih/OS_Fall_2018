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
	int thread_number;
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
int main(int argc, char* argv[])
{
	int rv = 1; // flag to check answer
    // input the sudoku file
    SetPuzzle("test.txt");
    /** my code here **/
    // setup checker using the sum of first row
    int checker, sum, i, j, s, t;
    sum = 0;
    for(j=9; j>0; j--) {
        sum += puzzle[1][j];
    }
    // checking rows
    for(i=9; i>1; i--) {
        checker = 0;
        for(j=9; j>0; j--) {
            checker += puzzle[i][j];
        }
        if(checker != sum) {
            rv = -1;
        }
    }
    // checking columns
    if(rv == 1) {
        for(j=9; j>0; j--) {
            checker = 0;
            for(i=9; i>0; i--) {
                checker += puzzle[i][j];
            }
            if(checker != sum) {
                rv = -2;
            }
        }
    }
    // checking sub-grids
    if(rv == 1) {
        for(s=2; s>=0; s--) {
            for(t=2; t>=0; t--) {
                checker = 0;
                for(i=3; i>0; i--) {
                    for(j=3; j>0; j--) {
                        checker += puzzle[s*3+i][t*3+j];
                    }
                }
                if(checker != sum) {
                    rv = -3;
                    //printf("Check bit: %x\n", checker);
                }
            }
        }
    }
    /** my code end here **/
    
	if (rv == 1)
		printf("Successful :) \n");
	else {
		printf("Must check again! :( \n");
        //printf("Error code: %d\n", rv);
    }

	return 0;
}
