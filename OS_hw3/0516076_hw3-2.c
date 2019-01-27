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

PROCESS minHeap[PROCESS_MAX];
int heap_size = 0;
void minHeapify(int);
void swap(PROCESS*, PROCESS*);
int extract();
void insert(PROCESS);

void schedSRTF(PROCESS*, int);

int dec_dgt(int, int);

int main(int argc, char** argv) {
    int i;
    int n;
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
    fclose(fp);

    schedSRTF(p, n);
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
    if((fp = fopen("ans2.txt", "w")) == NULL) {
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
void minHeapify(int i) {
    /*int x;
    printf("{");
    for(x=0; x<heap_size; x++)
        printf("(%d)", minHeap[x].p_id);
    printf("}");*/
    int smallest = i;
    int left = 2*i+1;
    int right = 2*i+2;
    if(left < heap_size) {
        if(minHeap[left].remain_time < minHeap[smallest].remain_time) {
            smallest = left;//printf("<");
        }
        if(minHeap[left].remain_time == minHeap[smallest].remain_time && minHeap[left].arr_time < minHeap[smallest].arr_time) {
            smallest = left;//printf("=");
        }
    }
    if(right < heap_size) {
        if(minHeap[right].remain_time < minHeap[smallest].remain_time) {
            smallest = right;//printf("<");
        }
        if(minHeap[right].remain_time == minHeap[smallest].remain_time && minHeap[right].arr_time < minHeap[smallest].arr_time) {
            smallest = right;//printf("=");
        }
    }
    if(i != smallest) {
        swap(&minHeap[i], &minHeap[smallest]);
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
        return minHeap[0].p_id;
    }
    int id = minHeap[0].p_id;
    minHeap[0] = minHeap[heap_size-1];
    heap_size --;
    minHeapify(0);
    return id;
}
void insert(PROCESS x) {
    heap_size ++;
    int i = heap_size-1;
    int parent = (i-1)/2;
    minHeap[i] = x;
    while(i != 0 && minHeap[i].remain_time < minHeap[parent].remain_time) {
    	swap(&minHeap[i], &minHeap[parent]);
    	i = parent;
    	parent = (i-1)/2;
    }

}

void schedSRTF(PROCESS* p, int n) {
    int time = 0;
    int current_p = -1;
    int terminate = 0;
    int i;
    while(1) {
        //printf("t(%d)", time);
        // search for arrived process and add to the heap
        for(i=0; i<n; i++) {
            if(p[i].arr_time == time){
                PROCESS arrived = p[i];
                insert(arrived);
                //printf("(->%d)", i);
            }
        }
        //printf("compare(%d, %d) ", p[current_p].remain_time, minHeap[0].remain_time);
        // check if the running process is still runing
        if(current_p >= 0) {
            //printf("preemption: (%d, %d)\n", p[current_p].remain_time, minHeap[0].remain_time);
            if(p[current_p].remain_time == 0){
                p[current_p].end_time = time;
                terminate ++;
                current_p = -1;
            }
            // preemption: compare current process with heap top
            else if(p[current_p].remain_time > minHeap[0].remain_time) {
                //printf("* ");
                insert(p[current_p]);
                current_p = -1;
            }
            else {
                p[current_p].remain_time --;
            }
        }
        // run the shortest process from the heap
        if(current_p < 0) {
            current_p = extract();
            // extract success
            if(current_p >= 0) {
                 p[current_p].run = 1;
                 p[current_p].remain_time --;
             }
        }
        // break if all process is finished
        if(terminate == n) {
            break;
        }
        /*
        printf("(%d) ready(%d): ", current_p, heap_size);
        for(i=0; i<heap_size; i++) {
            printf("(%d)", minHeap[i].p_id);
        }
        printf("\n");
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
