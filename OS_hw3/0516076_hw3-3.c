#include <stdio.h>
#include <stdlib.h>
#define PROCESS_MAX 1000

typedef struct _PROCESS {
    int p_id;
    int cpu_burst;
    int arr_time;
    int remain_time;
    //int wait_time;
    int end_time;
    int run;
} PROCESS;
PROCESS p[PROCESS_MAX] = {0, 0, 0, 0, 0, 0};

int queue[PROCESS_MAX];
int front = 0;
int rear = 0;
void push(int);
int pop();
int isempty();
int isfull();

void schedRR(PROCESS*, int, int);

int dec_dgt(int, int);

int main(int argc, char** argv) {
    int i;
    int n;
    int q;
    FILE* fp;

    // read test file
    if((fp = fopen(argv[1], "r")) == NULL) {
        printf("exit: read file error\n");
        exit(-1);
    }
    fscanf(fp, "%d", &n);
    for(i=0; i<n; i++) {
        p[i].p_id = i;
        fscanf(fp, "%d", &p[i].arr_time);
    }
    for(i=0; i<n; i++) {
        fscanf(fp, "%d", &p[i].cpu_burst);
        p[i].remain_time = p[i].cpu_burst;
    }
    fscanf(fp, "%d", &q);
    fclose(fp);

    schedRR(p, n, q);
/*
    int wait_sum = 0;
    int turnaround_sum = 0;
    for(i=0; i<n; i++) {
        int turnaround = p[i].end_time - p[i].arr_time;
        int wait_time = turnaround - p[i].cpu_burst;
        printf("process %d: %d, %d (%d, %d)\n", i, wait_time, turnaround, p[i].arr_time, p[i].cpu_burst);
        wait_sum += wait_time;
        turnaround_sum += turnaround;
    }
    printf("avg. waiting time: %g\navg. turnaround time: %g\n", (float)wait_sum/n, (float)turnaround_sum/n);
*/
    // output the result
    if((fp = fopen("ans3.txt", "w")) == NULL) {
        printf("exit: create file error\n");
        exit(-1);
    }
    
    int wait_sum = 0;
    int turnaround_sum = 0;
    int w_dgts, t_dgts;
    for(i=0; i<n; i++) {
        int turnaround = p[i].end_time - p[i].arr_time;
        int wait_time = turnaround - p[i].cpu_burst;
        fprintf(fp, "%d %d\n", wait_time, turnaround);
        wait_sum += wait_time;
        turnaround_sum += turnaround;
    }
    w_dgts = dec_dgt(wait_sum, n) + 5;
    t_dgts = dec_dgt(turnaround_sum, n) + 5;
    fprintf(fp, "%.*g\n%.*g\n", w_dgts, (float)wait_sum/n, t_dgts, (float)turnaround_sum/n);

    fclose(fp);

    return 0;
}

void push(int x) {
    if(isfull()) {
        printf("error: the queue is full\n");
        exit(-1);
    }
    queue[rear] = x;
    rear = (rear+1)%PROCESS_MAX;
}

int pop() {
    if(isempty()) {
        return -1;
    }
    int val = queue[front];
    front = (front+1)%PROCESS_MAX;
    return val;
}

int isfull() {
	return (front == (rear+1)%PROCESS_MAX) ? 1 : 0;
}

int isempty() {
	return (front == rear) ? 1 : 0;
}

void schedRR(PROCESS* p, int n, int q) {
    int timer = q;
    int time = 0;
    int current_p = -1;
    int terminate = 0;
    int i;
    while(1) {
        // search for arrived process and add to the queue
        for(i=0; i<n; i++) {
            if(p[i].arr_time == time){
                int arrived = p[i].p_id;
                push(arrived);
            }
        }
        // check if the running process is still runing
        if(current_p >= 0) {
            //printf("preemption: (%d, %d)\n", p[current_p].remain_time, minHeap[0].remain_time);
            if(p[current_p].remain_time == 0){
                p[current_p].end_time = time;
                terminate ++;
                //printf("terminate: %d\n", terminate);
                current_p = -1;
            }
            // preemption: check if timer is up
            else if(timer == 0) {
                //printf("*%d\n", time);
                push(p[current_p].p_id);
                current_p = -1;
            }
            else {
                p[current_p].remain_time --;
                timer --;
            }
        }
        // run the next process from the queue
        if(current_p < 0) {
            current_p = pop();
            // pop success
            if(current_p >= 0) {
                p[current_p].run = 1;
                p[current_p].remain_time --;
                timer = q;
                timer --;
            }
        }
        // break if all process is finished
        if(terminate == n) {
            break;
        }
        /*
        printf("(%d)\tready(%d): ", current_p, heap_size);
        for(i=0; i<heap_size; i++) {
            printf("(%d)", minHeap[i].p_id);
        }
        printf("\n");
        */
        time ++;
        //printf("time: %d\n", time);
    }
}

int dec_dgt(int a, int b) {
    int cnt = 0;
    int dec = a / b;
    while(dec != 0) {
        dec /= 10;
        cnt ++;
    }
    return cnt;
}