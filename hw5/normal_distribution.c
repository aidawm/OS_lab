#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    int hist[25]={0};
    int sample_number ;

    printf("enter number of sample : ");
    scanf("%i",&sample_number);

    clock_t begin = clock();//starting timer
    srand(time(0));
    for (int i = 0; i < sample_number; i++)
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
    clock_t end = clock();
    printf("Runtime = %f s\n\n\n", ((double)end - begin) / CLOCKS_PER_SEC);
    print(hist, sample_number);

    return 0 ;

}