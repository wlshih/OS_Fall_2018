#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstring>
#define MIN 1
#define MAX 100
#define RANDOM rand()%99 + 1

using namespace std;

int main(int argc, char** argv) {
    FILE* fp;
    char* fn;
    int n;
    int r;
    
    //strcpy(fn, argv[1]);
    //strcat(fn, ".txt");
    printf("****\n");
    srand(time(NULL));
    n = RANDOM;
 /*   
    if((fp = fopen(fn, "w")) == NULL) {
        fprintf(stderr, "WRITE FILE ERROR\n");
    }
    fprintf(fp, "%d\n", n);
    for(int i=0; i<n; i++) {
        r = RANDOM;
        fprintf(fp, "%d ", r);
    }
    fprintf(fp, "\n");
    for(int i=0; i<n; i++) {
        r = RANDOM;
        fprintf(fp, "%d ", r);
    }
    fprintf(fp, "\n");
    if(argv[1] == "Q4") {
    	int a = RANDOM;
    	int b = RANDOM;
    	fprintf(fp, "%d\n", min(a, b));
    	fprintf(fp, "%d\n", max(a, b));
    }
    
    fclose(fp);
*/
    return 0;
}