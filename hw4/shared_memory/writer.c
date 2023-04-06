#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
  
int main()
{
    
    key_t key = 2023;
    int memory_size = 3 ;
    // shmget returns an identifier in shmid
    int shmid = shmget(key, sizeof(int) * memory_size, 0666|IPC_CREAT);
  
    // shmat to attach to shared memory
    int *nums = (int*) shmat(shmid,(void*)0,0);
    
    for (int i=0;i<memory_size;i++){
        printf("enter %i number: ",i+1);
        scanf("%d", &nums[i]);
    }
        
    //detach from shared memory 
    shmdt(nums);
  
    return 0;
}