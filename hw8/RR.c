#include <stdio.h>

struct process{
    int pid;
    int burst_time;
    int waiting_time;
    int response_time;
    int remaining_time;
};

int main(){
    int num_process,time_quantum;

    printf("enter number of process :\t");
    scanf("%d",&num_process);

    struct process p[num_process];

    for(int i=0;i<num_process;i++){
        printf("enter burst time process %i: ",i+1);
        scanf("%d", &p[i].burst_time);
        p[i].pid=i+1;
        p[i].remaining_time = p[i].burst_time;
    }

    printf("enter the time quantum :\t");
    scanf("%d",&time_quantum);

    int i=0 , cnt_finish = 0;
    int finish [num_process] ;

    for(int j=0;j<num_process;j++)
        finish[j]=0;

    int total_time = 0;
    while (cnt_finish!=num_process)
    {
        int index = i%num_process;
        i++;
        if(finish[index]==1)
            continue;
        
        if(p[index].remaining_time<=time_quantum){
            printf("proc %i ----> %i clock ----> finish\n"
                    ,p[index].pid,p[index].remaining_time);
            total_time +=p[index].remaining_time;
            p[index].remaining_time = 0;
            finish[index] = 1;
            cnt_finish++;
            p[index].response_time = total_time;
            p[index].waiting_time = p[index].response_time - p[index].burst_time;
        }
        else{
            printf("proc %i ----> %i clock \n",p[index].pid,time_quantum);
            p[index].remaining_time -= time_quantum;
            total_time += time_quantum;
        }
    }
    printf("\n\n");

    int total_waiting = 0 , total_response=0;
    for(int i=0;i<num_process;i++){
        printf("%i  : burst_time = %i , waiting_time = %i , response_time = %i\n"
                ,p[i].pid,p[i].burst_time,p[i].waiting_time,p[i].response_time);
        total_response += p[i].response_time;
        total_waiting += p[i].waiting_time;
    }
    printf("----------------------\n");

    printf("waiting_average = %f\tresponse_average = %f\n"
            ,((float)total_waiting/num_process),((float)total_response/num_process));
    return 0;
}