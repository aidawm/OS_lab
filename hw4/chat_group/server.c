#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char const *argv[]) {   

    //0. define variables
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(argv[1])); 
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    char buffer[1024]={0};

    //1. create socket 
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    //2. bind port
    int bind_result = bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    if(bind_result <0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //3. listen 
    int listen_result = listen(server_fd, 10);

    if(listen_result <0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Listen on %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    //4. accept 
    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(server_fd,(struct sockaddr *)&clientAddress,&clientAddressSize);
    if (clientSocketFD < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Hello client %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
    //5. recieve messages 
    
    while(1){
        ssize_t valread = recv(clientSocketFD, buffer, 1024,0);
        printf("%li\n",valread);
        if(valread ==0)
            break;
        buffer[valread] = 0;
        printf("client = %s",buffer);
    }
    close(clientSocketFD);
    shutdown(server_fd,SHUT_RDWR);

    return 0 ;
}