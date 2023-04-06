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
    char client_name[10];
    int client_FD;
    struct sockaddr_in clientAddress;
    bool isSuccessfull 
};

struct acceptedClient client_list[10];
int online_clients = 0;

struct acceptedClient* accept_client(int serverFd){
    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(serverFd,(struct sockaddr *)&clientAddress,&clientAddressSize);

    struct acceptedClient* client = malloc(sizeof(struct acceptedClient));
    char name[10];
    ssize_t valread = recv(clientSocketFD, name, 10,0);
    client->client_FD = clientSocketFD;
    client->clientAddress = clientAddress;
    client->isSuccessfull = !(clientSocketFD < 0);
    if(valread>0){
        name[valread]=0;
        strcpy(client->client_name , name);
        printf("%s is online\n",client->client_name);

    }

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

void recieve_client_messages(struct acceptedClient *client){
    char buffer[1024]={0};
    while(1){
        ssize_t valread = recv(client->client_FD, buffer, 1024,0);
        if(valread ==0)
            break;
        buffer[valread] = 0;
        char message[1024];
        sprintf(message,"%s:%s",client->client_name,buffer);
        // char *message = strcat(client->client_name,buffer);
        send_message_to_others(client->client_FD,message);

        printf("%s : %s",client->client_name,buffer);
        
    }
    close(client->client_FD);
}

void reciever_thread(struct acceptedClient *client){
    pthread_t id;
    pthread_create(&id,NULL,recieve_client_messages,client);
}

void send_message_to_others(int client_FD,char* message){
    
    for(int i=0;i<online_clients;i++){
        if(client_list[i].client_FD != client_FD){
            printf("%li",sizeof(message));
            send(client_list[i].client_FD , message, 1024,0);
        }
    }
}

int main(int argc, char const *argv[]) {   


    int server_fd = setup_server(argv);

    //4. accept 
    while(1){
        if(online_clients ==10){
            continue;
        }
        struct acceptedClient *client = accept_client(server_fd);
        client_list[online_clients++] = *client;
        reciever_thread(client);
    }
    

    //5. recieve messages 
    
    shutdown(server_fd,SHUT_RDWR);

    return 0 ;
}