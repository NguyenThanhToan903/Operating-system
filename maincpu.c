#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAX 2147483647
#define SIZE 50

struct process
{
    int pid;
    float arrivaltime, waitingtime, responsetime, turnaroundtime, durationtime, completiontime;
};

struct queue
{
    struct process *arr[SIZE];
    int front, rear;
};

struct queue *createQueue()
{
    struct queue *q = (struct queue *)malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

int isFull(struct queue *q)
{
    return (q->rear == SIZE - 1);
}

int isEmpty(struct queue *q)
{
    return (q->front == -1 || q->front > q->rear);
}

void enqueue(struct queue *q, struct process *p)
{
    if (isFull(q))
    {
        printf("Queue is full.\n");
        return;
    }
    q->rear++;
    q->arr[q->rear] = p;
    if (q->front == -1)
    {
        q->front = 0;
    }
}

struct process *dequeue(struct queue *q)
{
    if (isEmpty(q))
    {
        printf("Queue is empty.\n");
        return NULL;
    }
    struct process *p = q->arr[q->front];
    q->front++;
    if (q->front > q->rear)
    {
        q->front = -1;
        q->rear = -1;
    }
    return p;
}

void swap_process(struct process *p1, struct process *p2)
{
    struct process temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

void gant_chart(struct process *proc, int process_number, int end)
{
    int i, j;

    for (i = 0; i < process_number; i++)
    {
        printf("p%d:", proc[i].pid);
        int p_dur = (int)proc[i].durationtime;
        int p_comp = (int)proc[i].completiontime;
        int p_arr = (int)proc[i].arrivaltime;
        int p_w = (int)proc[i].waitingtime;
        int p_r = (int)proc[i].responsetime;
        for (j = 0; j < end; j++)
        {
            if (j >= p_r && j < p_comp && i == 0)
            {
                printf("R");
            }
            else if (j >= p_comp)
            {
                printf("-");
            }
            else
            {
                float temp = proc[i].responsetime;
                if ((j < proc[i - 1].completiontime) && (j >= proc[i].arrivaltime))
                {
                    printf("W");
                }
                else if (j >= proc[i - 1].completiontime && j <= proc[i].completiontime)
                {
                    printf("R");
                }
                else
                {
                    printf("-");
                }
            }
        }
        printf("\n");
    }
}

void fcfs(struct process *proc, int process_number)
{
    printf("Dinh thoi FCFS\n");
    printf("---------------------------------\n");
    int i = 0, j, k;
    float time = 0, start = 0, end, waitingtime = 0, responsetime = 0, turnaroundtime = 0;
    // Sắp xếp tiến trình theo thời gian đến
    int p[process_number];
    for (i = 0; i < process_number; i++)
    {
        for (j = i + 1; j < process_number; j++)
        {
            if (proc[i].arrivaltime > proc[j].arrivaltime)
            {
                swap_process(&proc[i], &proc[j]);
            }
        }
    }
    for (i = 0; i < process_number; i++)
    {

        if (i == 0)
        {
            proc[i].waitingtime = proc[i].arrivaltime;
            proc[i].responsetime = proc[i].waitingtime;
            proc[i].completiontime = proc[i].durationtime + proc[i].arrivaltime;
        }
        else
        {
            proc[i].waitingtime = proc[i - 1].completiontime - proc[i].arrivaltime;
            proc[i].responsetime = proc[i].waitingtime;
            proc[i].completiontime = proc[i - 1].completiontime + proc[i].durationtime;
        }
        turnaroundtime += proc[i].completiontime - proc[i].arrivaltime;
        waitingtime += proc[i].waitingtime;
        responsetime += proc[i].responsetime;
        end = proc[i].completiontime;
    }
    float avgw = waitingtime / process_number;
    float avgr = responsetime / process_number;
    float avgt = turnaroundtime / process_number;

    // in định dạng cho từng tiến trình
    gant_chart(proc, process_number, end);
    printf("AVGW= %.2f\tAVGR= %.2f\tAVGT= %.2f\n", avgw, avgr, avgt);
    printf("---------------------------------\n");
}

void sjf_nonpreemptive(struct process *proc, int process_number)
{
    int completed = 0, time = 0, shortest = INT_MAX, finish_time, end, i, j, index;
    float time_before;
    float total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;
    int is_completed[process_number];
    int is_sorted[process_number]; // sorted real time

    printf("Dinh thoi SJF khong chap nhan gian doan\n");
    printf("---------------------------------------\n");

    while (completed != process_number)
    {
        if (completed == 0)
        {
            proc[completed].completiontime = proc[completed].arrivaltime + proc[completed].durationtime;
            proc[completed].responsetime = proc[completed].arrivaltime;
            proc[completed].turnaroundtime = proc[completed].completiontime - proc[completed].arrivaltime;
            proc[completed].waitingtime = proc[completed].responsetime;
            time_before = proc[completed].completiontime;
            is_sorted[process_number - completed - 1] = -1;
            int k = 0;
            for (i = completed + 1; i < process_number; i++)
            {
                if (proc[i].arrivaltime <= time_before)
                {
                    is_sorted[k] = i;
                    k++;
                }
            }

            for (i = 0; is_sorted[i] > 0; i++)
            {
                for (j = i + 1; is_sorted[j] > 0; j++)
                {
                    if (proc[is_sorted[i]].durationtime >= proc[is_sorted[j]].durationtime)

                    {
                        swap_process(&proc[is_sorted[i]], &proc[is_sorted[j]]);
                    }
                }
            }
        }
        else
        {
            proc[completed].completiontime = proc[completed - 1].completiontime + proc[completed].durationtime;
            proc[completed].responsetime = proc[completed - 1].completiontime - proc[completed].responsetime;
            proc[completed].waitingtime = proc[completed].responsetime;
            proc[completed].turnaroundtime = proc[completed].completiontime - proc[completed].arrivaltime;
            time_before = proc[completed].completiontime;
            int k = 0;
            for (i = completed + 1; i < process_number; i++)
            {
                if (proc[i].arrivaltime < time_before)
                {
                    is_sorted[k] = i;
                    k++;
                }
            }

            for (i = 0; i > 0; i++)
            {
                for (j = i + 1; is_sorted[j] > 0; j++)
                {
                    if (proc[is_sorted[i]].durationtime > proc[is_sorted[j]].durationtime)

                    {
                        swap_process(&proc[is_sorted[i]], &proc[is_sorted[j]]);
                    }
                }
            }
        }
        total_turnaround_time += proc[completed].completiontime - proc[completed].arrivaltime;
        total_waiting_time += proc[completed].waitingtime;
        total_response_time += proc[completed].responsetime;
        end = proc[completed].completiontime;
        completed++;
    }
    float avgw = total_waiting_time / process_number;
    float avgt = total_turnaround_time / process_number;
    float avgr = total_response_time / process_number;
    // in định dạng cho từng tiến trình
    gant_chart(proc, process_number, end);
    printf("AVGW= %.2f\tAVGR= %.2f\tAVGT= %.2f\n", avgw, avgr, avgt);
    printf("---------------------------------\n");
}

void round_robin(struct process *proc, int process_number, int quantum)
{
    int i, j, k;
    float avgt, avgw, avgr;
    struct queue *q = createQueue(process_number);
    for (i = 0; i < process_number; i++)
    {
        proc[i].waitingtime = 0;
        proc[i].responsetime = -1;
        proc[i].turnaroundtime = 0;
    }

    printf("Dinh thoi Round Robin\n");
    printf("---------------------------------------\n");
    int time = 0, completed = 0, current_process = -1;
    while (completed < process_number)
    {
        for (i = 0; i < process_number; i++)
        {
            if ((proc[i].arrivaltime <= time) && (proc[i].durationtime > 0) && (proc[i].responsetime == -1))
            {
                proc[i].responsetime = time - proc[i].arrivaltime;
                enqueue(q, &proc[i]);
            }
        }
        if (!isEmpty(q))
        {
            current_process++;
            if (current_process >= q->rear)
            {
                current_process = q->front;
            }
            struct process *p = dequeue(q);

            if (p->durationtime > quantum)
            {
                for (j = 0; j < quantum; j++)
                {
                    printf("R");
                }
                for (j = 0; j < quantum; j++)
                {
                    printf("W");
                }
                time += quantum;
                p->durationtime -= quantum;
                enqueue(q, p);
            }
            else
            {
                for (j = 0; j < quantum; j++)
                {
                    printf("R");
                }
                for (j = 0; j < quantum; j++)
                {
                    printf("W");
                }
                p->completiontime = time;
                p->turnaroundtime = p->completiontime - p->arrivaltime;
                p->waitingtime = p->turnaroundtime - p->durationtime;
                completed++;
            }
        }
        else
        {
            printf("W");
            time++;
        }
        printf("\n");
    }
    int end = time;
    float total_waiting_time = 0;
    float total_response_time = 0;
    float total_turnaround_time = 0;
    for (i = 0; i < process_number; i++)
    {
        total_waiting_time += proc[i].waitingtime;
        total_response_time += proc[i].responsetime;
        total_turnaround_time += proc[i].turnaroundtime;
    }
    avgw = total_waiting_time / process_number;
    avgr = total_response_time / process_number;
    avgt = total_turnaround_time / process_number;

    // printf("\n");
    // for (i = 0; i < process_number; i++)
    // {
    //     printf("p%d: ", proc[i].pid);
    //     for (j = 0; j < proc[i].durationtime; j++)
    //     {
    //         if (j < proc[i].responsetime || j >= proc[i].completiontime)
    //         {
    //             printf("-");
    //         }
    //         else if (j >= proc[i].responsetime && j < proc[i].completiontime && j % quantum == 0)
    //         {
    //             printf("R");
    //         }
    //         else
    //         {
    //             printf("W");
    //         }
    //     }
    //     printf("\n");
    // }
    // In ra kết quả theo định dạng yêu cầu

    printf("%d", end);
    printf("AVGW= %.2f\tAVGR= %.2f\tAVGT= %.2f\n", avgw, avgr, avgt);
    printf("---------------------------------\n");
}

int main()
{
    int process_number;
    printf("Nhap so luong tien trinh: ");
    scanf("%d", &process_number);
    int quantum;
    printf("Nhap quantum: ");
    scanf("%d", &quantum);
    struct process *proc = (struct process *)malloc(process_number * sizeof(struct process));

    for (int i = 0; i < process_number; i++)
    {
        printf("Nhap thong tin tien trinh thu %d (arrivaltime, durationtime): ", i + 1);
        scanf("%f %f", &proc[i].arrivaltime, &proc[i].durationtime);
        proc[i].pid = i + 1;
    }

    fcfs(proc, process_number);
    sjf_nonpreemptive(proc, process_number);
    round_robin(proc, process_number, quantum);
    free(proc);

    return 0;
}
