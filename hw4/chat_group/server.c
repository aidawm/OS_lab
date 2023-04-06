#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>


struct acceptedClient {
    int client_FD;
    struct sockaddr_in clientAddress;
    bool isSuccessfull 
};

struct acceptedClient* accept_client(int serverFd){
    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(serverFd,(struct sockaddr *)&clientAddress,&clientAddressSize);

    struct acceptedClient* client = malloc(sizeof(struct acceptedClient));

    client->client_FD = clientSocketFD;
    client->clientAddress = clientAddress;
    client->isSuccessfull = !(clientSocketFD < 0);

    return client;
}

int setup_server(char const *argv[]){

    //0. define variables
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(argv[1])); 
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

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

    return server_fd;
}

void recieve_client_messages(int client_FD){
    char buffer[1024]={0};
    while(1){
        ssize_t valread = recv(client_FD, buffer, 1024,0);
        if(valread ==0)
            break;
        buffer[valread] = 0;
        printf("client = %s",buffer);
    }
    close(client_FD);
}

void reciever_thread(int client_FD){
    pthread_t id;
    pthread_create(&id,NULL,recieve_client_messages,client_FD);
}

int main(int argc, char const *argv[]) {   


    int server_fd = setup_server(argv);

    //4. accept 
    while(1){
       struct acceptedClient *client = accept_client(server_fd);
        reciever_thread(client->client_FD);
    }
    



    //5. recieve messages 
    
    shutdown(server_fd,SHUT_RDWR);

    return 0 ;
}