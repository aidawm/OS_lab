#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char const *argv[]) {
    //0. define variables
    struct sockaddr_in sock_addr;
    memset(&sock_addr, '0', sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(atoi(argv[2]));
    int check_addr = inet_pton(AF_INET, argv[1], &sock_addr.sin_addr);

    if (check_addr <0){
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    

    // 1. create socket 
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socketFD <0){
        printf("\n Socket creation error \n");
        return -1;
    }
  
    // 2. connection 
    if (connect(socketFD, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    char *message = NULL;
    ssize_t message_size = 0;

    while(1){
        ssize_t char_count = getline(&message,&message_size,stdin);

        if(char_count >0){
            if(strcmp(message,"exit\n")==0){
                break;
            }
            ssize_t send_count = send(socketFD,message,message_size,0);
        }
    }

    return 0;
}