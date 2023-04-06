#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main()
{
	
	key_t key = 2023;
    int memory_size = 3 ;

	// shmget returns an identifier in shmid
	int shmid = shmget(key,sizeof(int) * memory_size,0666|IPC_CREAT);

	// shmat to attach to shared memory
	int *nums = (int*) shmat(shmid,(void*)0,0);

    int sum = 0 ;
    for(int i=0 ; i<memory_size;i++){
        printf("numer %i: %i\n",i+1,nums[i]);
        sum+= nums[i];
    }
	printf("Sum of data read from memory: %d\n",sum);
	
	//detach from shared memory
	shmdt(nums);
	
	// destroy the shared memory
	shmctl(shmid,IPC_RMID,NULL);
	
	return 0;
}