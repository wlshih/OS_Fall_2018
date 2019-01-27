#include <stdio.h>
#include <stdlib.h>
#define PROCESS_MAX 1000

typedef struct _PROCESS {
    int p_id;
    int cpu_burst;
    int arr_time;
    int remain_time;
    int end_time;
    int run;
} PROCESS;
PROCESS p[PROCESS_MAX] = {0, 0, 0, 0, 0, 0};

typedef struct _QUEUE {
    int id[PROCESS_MAX];
    int front;
    int rear;
    
    void (*push)(struct _QUEUE*, int);
    int (*pop)(struct _QUEUE*);
    int (*isempty)(struct _QUEUE*);
    int (*isfull)(struct _QUEUE*);
} QUEUE;
QUEUE QUEUE_new();

int heap_size = 0;
void minHeapify(int);
void swap(PROCESS*, PROCESS*);
int extract();
void insert(PROCESS);

QUEUE hi_queue;
QUEUE md_queue;
PROCESS lo_heap[PROCESS_MAX];

void schedMuliFbck(PROCESS*, int, int*);

int dec_dgt(int, int);

int main(int argc, char** argv) {
    int i;
    int n;
    int q[2];
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
    fscanf(fp, "%d %d", q, q+1);
    fclose(fp);

    schedMuliFbck(p, n, q);
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
    if((fp = fopen("ans4.txt", "w")) == NULL) {
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


void push(QUEUE* q, int x) {
    if(q->isfull(q)) {
        printf("error: the queue is full\n");
        exit(-1);
    }
    q->id[q->rear] = x;
    q->rear = (q->rear+1)%PROCESS_MAX;
}

int pop(QUEUE* q) {
    if(q->isempty(q)) {
        return -1;
    }
    int val = q->id[q->front];
    q->front = (q->front+1)%PROCESS_MAX;
    return val;
}

int isfull(QUEUE* q) {
    return (q->front == (q->rear+1)%PROCESS_MAX) ? 1 : 0;
}

int isempty(QUEUE* q) {
    return (q->front == q->rear) ? 1 : 0;
}

QUEUE QUEUE_new() {
    QUEUE q = {
        .front = 0,
        .rear = 0,
        .push = &push,
        .pop = &pop,
        .isempty = &isempty,
        .isfull = &isfull
    };
    return q;    
}


void minHeapify(int i) {
    int smallest = i;
    int left = 2*i+1;
    int right = 2*i+2;
    if(left < heap_size) {
        if(lo_heap[left].cpu_burst < lo_heap[smallest].cpu_burst) {
            smallest = left;
        }
        if(lo_heap[left].cpu_burst == lo_heap[smallest].cpu_burst && lo_heap[left].arr_time < lo_heap[smallest].arr_time) {
            smallest = left;
        }
    }
    if(right < heap_size) {
        if(lo_heap[right].cpu_burst < lo_heap[smallest].cpu_burst) {
            smallest = right;
        }
        if(lo_heap[right].cpu_burst == lo_heap[smallest].cpu_burst && lo_heap[right].arr_time < lo_heap[smallest].arr_time) {
            smallest = right;
        }
    }
    if(i != smallest) {
        swap(&lo_heap[i], &lo_heap[smallest]);
        minHeapify(smallest);
    }
}

void swap(PROCESS* a, PROCESS* b) {
    PROCESS tmp = *a;
    *a = *b;
    *b = tmp;
}

int extract() {
    if(heap_size <= 0) {
        return -1;
    }
    if(heap_size == 1) {
        heap_size --;
        return lo_heap[0].p_id;
    }
    int id = lo_heap[0].p_id;
    lo_heap[0] = lo_heap[heap_size-1];
    heap_size --;
    minHeapify(0);
    return id;
}
void insert(PROCESS x) {
    heap_size ++;
    int i = heap_size-1;
    int parent = (i-1)/2;
    lo_heap[i] = x;
    while(i != 0 && lo_heap[i].cpu_burst < lo_heap[parent].cpu_burst) {
    	swap(&lo_heap[i], &lo_heap[parent]);
    	i = parent;
    	parent = (i-1)/2;
    	//printf("*\n");
    }

}


void schedMuliFbck(PROCESS* p, int n, int* q) {
    hi_queue = QUEUE_new();
    md_queue = QUEUE_new();
    int hi_timer = q[0];
    int md_timer = q[1];
    int hi_current = -1;
    int mid_current = -1;
    int lo_current = -1;

    int time = 0;
    int switch_queue = 0;
    int terminate = 0;
    int i;

    while(1) {
        //printf("t(%d) ", time);
        // search for arrived process and add to the high-queue
        for(i=0; i<n; i++) {
            if(p[i].arr_time == time){
                int arrived = p[i].p_id;
                hi_queue.push(&hi_queue, arrived);
                //printf("->");
            }
        }
        if(!hi_queue.isempty(&hi_queue)) {
            switch_queue = 0;
        }

        // --------High-level RR(q1)--------- //
        if(switch_queue == 0) {
            // check if the running process is still runing
            if(hi_current >= 0) {
                // process just finished
                if(p[hi_current].remain_time == 0){
                    p[hi_current].end_time = time;
                    terminate ++;
                    hi_current = -1;
                }
                // still remaining -> check timer preemption, add to mid-queue
                else if(hi_timer == 0) {
                    md_queue.push(&md_queue, hi_current);
                    hi_current = -1;
                }
                // continue run
                else {
                    p[hi_current].remain_time --;
                    hi_timer --;
                }
            }
            // run the next process from the queue
            if(hi_current < 0) {
                hi_current = hi_queue.pop(&hi_queue);
                // pop success
                if(hi_current >= 0) {
                    p[hi_current].run = 1;
                    p[hi_current].remain_time --;
                    hi_timer = q[0];
                    hi_timer --;
                }
                // no ready process, go to mid-queue
                else {
                    switch_queue = 1;
                }
            }
        }
        // ----------mid-level RR(q2)----------- //
        if(switch_queue == 1) {
            // check if the running process is still runing
            if(mid_current >= 0) {
                // process just finished
                if(p[mid_current].remain_time == 0){
                    //printf("*");
                    p[mid_current].end_time = time;
                    terminate ++;
                    mid_current = -1;
                }
                // still remaining -> check timer preemption, add to low-heap
                else if(md_timer == 0) {
                    insert(p[mid_current]);
                    mid_current = -1;
                }
                else {
                    p[mid_current].remain_time --;
                    md_timer --;
                }
            }
            // run the next process from the queue
            if(mid_current < 0) {
                mid_current = md_queue.pop(&md_queue);
                // pop success
                if(mid_current >= 0) {
                    p[mid_current].run = 1;
                    p[mid_current].remain_time --;
                    md_timer = q[1];
                    md_timer --;
                }
                // no ready process, go to lo-heap
                else {
                    switch_queue = 2;
                }
            }
        }
        // ------------low-level SJF----------- //
        if(switch_queue == 2) {
            // check if the running process is still runing
            if(lo_current >= 0) {
                if(p[lo_current].remain_time == 0){
                    p[lo_current].end_time = time;
                    terminate ++;
                    lo_current = -1;
                }
                else {
                    p[lo_current].remain_time --;
                }
            }
            // run the shortest process from the heap
            if(lo_current < 0) {
                lo_current = extract();
                if(lo_current >= 0) {
                    p[lo_current].run = 1;
                    p[lo_current].remain_time --;
                }
            }
        }

        // break if all process is finished
        if(terminate == n) {
            break;
        }
/*
        printf("queue: %d, ", switch_queue);
        if(switch_queue == 0) {
            printf("htmr: %d, ", hi_timer);
            printf("[%d]:%d\n", hi_current+1, p[hi_current].remain_time);
        }
        else if(switch_queue == 1) {
            printf("mtmr: %d, ", md_timer);
            printf("[%d]:%d\n", mid_current+1, p[mid_current].remain_time);
        }
        else printf("[%d]:%d\n", lo_current+1, p[lo_current].remain_time);
*/
        time ++;
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
