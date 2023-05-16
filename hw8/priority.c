#include <stdio.h>

struct process{
    int pid;
    int priority;
    int burst_time;
    int waiting_time;
    int response_time;
};

int main(){
    int num_process;

    printf("enter number of process :\t");
    scanf("%d",&num_process);
    
    struct process p[num_process];
    struct process temp;
    
    for(int i=0;i<num_process;i++){
        printf("enter burst time process %i: ",i+1);
        scanf("%d", &p[i].burst_time);
        printf("enter priority of process %i: ",i+1);
        scanf("%d", &p[i].priority);
        p[i].pid=i+1;
    }


    for (int i = 0; i < num_process; i++)
    {
        for (int j = i + 1; j < num_process; j++)
        {
            if (p[i].priority > p[j].priority)
            {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
    
    for(int i=0;i<num_process;i++){
        if(i==0){
            p[i].waiting_time = 0;
            p[i].response_time = p[i].burst_time;
        }
        else{
            p[i].waiting_time = p[i-1].response_time;
            p[i].response_time = p[i].burst_time+p[i].waiting_time;
        }
    }

    int total_waiting = 0 , total_response=0;
    for(int i=0;i<num_process;i++){
        printf("%i  : priority = %i ,burst_time = %i , waiting_time = %i , response_time = %i\n",p[i].pid,p[i].priority,p[i].burst_time,p[i].waiting_time,p[i].response_time);
        total_response += p[i].response_time;
        total_waiting += p[i].waiting_time;
    }
    printf("----------------------\n");

    printf("waiting_average = %f\tresponse_average = %f\n",((float)total_waiting/num_process),((float)total_response/num_process));

    return 0;
}