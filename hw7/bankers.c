#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include<time.h>
#include <unistd.h>

// #####################################attributes
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3
int c=0;
int available[NUMBER_OF_RESOURCES];

int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = {
                                                        { 7, 5, 3 },
                                                        { 3, 2, 2 }, 
                                                        { 9, 0, 2 }, 
                                                        { 2, 2, 2 }, 
                                                        { 4, 3, 3 } 
                                                        };

int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] ={
                                                            { 0, 1, 0 }, 
                                                            { 2, 0, 0 }, 
                                                            { 3, 0, 2 }, 
                                                            { 2, 1, 1 }, 
                                                            { 0, 0, 2 } 
                                                        };

int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

void update_needs(){
    for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++){
			need[i][j] = maximum[i][j] - allocation[i][j];
            printf("%d\t",need[i][j]);
        }
        printf("\n");
	}
}

pthread_t t_id[NUMBER_OF_CUSTOMERS];
pthread_mutex_t lock;
// ############################################# methods
void print_status(int request[],int customer_num){
    printf("Customer %d is Requesting Resources:\n", customer_num);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf("%d  ",request[i]);
    }

    printf("\nAvailable = ");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf("%d  ", available[i]);
    }

    printf("\nNeed = ");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf("%d  ", need[customer_num][i]);
    }
    printf("\n----------------------------\n");
}

bool safe(){
    int finish[NUMBER_OF_CUSTOMERS] = {0};
    int y = 0; 
    int work[NUMBER_OF_RESOURCES];
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        work[i] = available[i]; // right now work array is equal to available array
    } 


	for (int k = 0; k < NUMBER_OF_CUSTOMERS; k++) { 
		for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) { 

			if (finish[i] == 0) {  //unfinished process
				bool flag = true; 
				for (int j = 0; j < NUMBER_OF_RESOURCES; j++) { 
					if (need[i][j] > work[j]){  //not enough resources
						flag = false; 
						break; 
					} 
				} 

				if (flag == true) {  //resources are available
					for (y = 0; y < NUMBER_OF_RESOURCES; y++) 
						work[y] += allocation[i][y]; //allocating resources
					finish[i] = 1; //process is finished 

				} 
			}
            else{
                break;
            } 
		} 
	} 
    for (int i=0;i<NUMBER_OF_CUSTOMERS;i++){
        if(finish[i]==0)
            return false;
    }
    return true;
}


int request_resources(int request[], int customer_num) {

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > available[i]) {
            c++;
            printf("%d : Request is more than available! ABORT!\n",c);
            
            return 0;
           }
    }

    for(int j=0 ; j<NUMBER_OF_RESOURCES ; j++){
        available[j] -= request[j];
        allocation[customer_num][j] += request[j];
        need[customer_num][j] -= request[j];
    }

    if(safe()){
        printf("Safe! Request is granted!\n");
        bool is_empty = true;
        for (int k = 0; k < NUMBER_OF_RESOURCES; k++)
            if(need[customer_num][k] > 0)
                is_empty = false;
        
        if(is_empty){
            printf("%d got all it needed!\n",customer_num);
            return 1; 
        }
        return 0 ;
        
    }else{
        for(int j=0 ; j<NUMBER_OF_RESOURCES ; j++){
            available[j] += request[j];
            allocation[customer_num][j] -= request[j];
            need[customer_num][j] += request[j];
        }
        printf("Not safe! Can't grant request!\n");
        return 0;
    }


}
int release_resources(int release[]){
    for(int i=0 ; i<NUMBER_OF_RESOURCES ; i++){
        available[i] += release[i];
    }
}

void* customer(void *arg){

    int customerNum = *(int *)arg;
    int flag = 0;
    int request [NUMBER_OF_RESOURCES];

    srand(time(NULL));

    int finish = false;
    int count_requests = 0;
    while (!finish)
    {   
        int count_res = 0;
        for(int i =0;i<NUMBER_OF_RESOURCES;i++){
            if(need[customerNum][i]==0){
                count_res++;
                request[i]=0;
                continue;
            }
           
            request[i] = rand()%(need[customerNum][i]+1) ;
        }
        if(count_res == NUMBER_OF_RESOURCES)
            break;

        int count =0;
        for(int i =0;i<NUMBER_OF_RESOURCES;i++){
            if(request[i]==0)
                count++;
        }
        if(count == NUMBER_OF_RESOURCES)
            continue;

        count_requests ++;
        pthread_mutex_lock(&lock);
        print_status(request,customerNum);
        finish=request_resources(request, customerNum);
        sleep(1);
        pthread_mutex_unlock(&lock);
    
    }
    pthread_mutex_lock(&lock);
    release_resources(maximum[customerNum]);
    for (int k = 0; k < NUMBER_OF_RESOURCES; k++)
            allocation[customerNum][k] = 0;
    pthread_mutex_unlock(&lock);


}

int main(int argc, char *argv[]){

    if (argc < NUMBER_OF_RESOURCES + 1) {
        printf("not enough arguments!\n");
        exit(1);
    }


    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {

        available[i] = atoi(argv[i+1]);
    }

    update_needs();

    
    int pid[] = {0,1,2,3,4};
    for (int i=0; i<NUMBER_OF_CUSTOMERS ; i++){
        pthread_create(&(t_id[i]),NULL,customer,&pid[i]);
    }

    for (int i=0; i<NUMBER_OF_CUSTOMERS ; i++){
        pthread_join(t_id[i], NULL);
    }

    printf("FINISH!\n");
    pthread_mutex_destroy(&lock);
    return 0;
}