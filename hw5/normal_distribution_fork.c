#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void print(int *hist, int sample) {
    for (int i = 0; i < 25; i++) 
    {
        printf("%d\t", i - 12);

        printf(" %d\n", hist[i]);
    }

    
    if(sample < 1000){
        printf("\n\n\n\n"); 
        for (int i = 0; i < 25; i++) {
            printf("%d\t", i - 12);
            for (int j = 0; j < hist[i]; j++){
                    printf(".");
            }
            printf("\n");
        }
    }
}

int main(){

    int sample_number ;
    printf("enter number of sample : ");
    scanf("%i",&sample_number);

    key_t key = 2023;
    int memory_size = 25 ;
    int shmid = shmget(key, sizeof(int) * memory_size, 0666|IPC_CREAT);
    int *hist = (int*) shmat(shmid,(void*)0,0);

    int pid ;
    pid = fork();
    if(pid ==0){
        fork();
        fork();
    }
    int process_number = 5;
    clock_t begin = clock();//starting timer
    srand(time(0));
    for (int i = 0; i < (sample_number/process_number) ; i++)
    {
        int counter = 12;
        for (int j = 0; j < 12; j++)
        {
            int random = rand() % 101;
            if (random >= 49)
                counter++;
            else 
                counter--;        
        }
        hist[counter]++;
    }
    if(pid !=0){
        wait(NULL);
        clock_t end = clock();
        printf("Runtime = %f s\n\n\n", ((double)end - begin) / CLOCKS_PER_SEC);
        print(hist, sample_number);
        shmdt(hist);
        shmctl(shmid,IPC_RMID,NULL);
    }
    

    return 0 ;

}