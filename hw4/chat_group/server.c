#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

struct acceptedClient
{
    char client_name[10];
    int client_FD;
    struct sockaddr_in clientAddress;
    bool isSuccessfull
};

struct group
{
    char name[10];
    int client_list[10];
    int cnt_clinet;
};

struct acceptedClient client_list[10];
struct group groups_list[10];
int online_clients = 0;
int groups_number = 0;

struct acceptedClient *accept_client(int serverFd)
{
    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(serverFd, (struct sockaddr *)&clientAddress, &clientAddressSize);

    struct acceptedClient *client = malloc(sizeof(struct acceptedClient));
    char name[10];
    ssize_t valread = recv(clientSocketFD, name, 10, 0);
    client->client_FD = clientSocketFD;
    client->clientAddress = clientAddress;
    client->isSuccessfull = !(clientSocketFD < 0);
    if (valread > 0)
    {
        name[valread] = 0;
        strcpy(client->client_name, name);
        printf("%s is online\n", client->client_name);
    }

    return client;
}

int setup_server(char const *argv[])
{

    // 0. define variables
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(argv[1]));
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    // 1. create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // 2. bind port
    int bind_result = bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    if (bind_result < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. listen
    int listen_result = listen(server_fd, 10);

    if (listen_result < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Listen on %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    return server_fd;
}
void create_group(int client_FD, char *group_name)
{
    struct group *c_group = malloc(sizeof(struct group));
    strcpy(c_group->name, group_name);
    c_group->client_list[0] = client_FD;
    c_group->cnt_clinet = 1;

    groups_list[groups_number++] = *c_group;
}

bool join_group(int client_FD, char *group_name)
{
    for (int i = 0; i < groups_number; i++)
    {
        if (strcmp(groups_list[i].name, group_name) == 0)
        {
            for (int j = 0; j < groups_list[i].cnt_clinet; j++)
            {
                if (client_FD == groups_list[i].client_list[j])
                    return true;
            }
            groups_list[i].client_list[groups_list[i].cnt_clinet++] = client_FD;
            char *message = "you joined to";
            strcat(message, group_name);
            send(client_FD, message, 1024, 0);
            return true;
        }
    }
    return false;
}
bool send_message_group(struct acceptedClient *client, char *group_name, char *message)
{
    for (int i = 0; i < groups_number; i++)
    {
        if (strcmp(groups_list[i].name, group_name) == 0)
        {
            for (int j = 0; j < groups_list[i].cnt_clinet; j++)
            {
                if (client->client_FD == groups_list[i].client_list[j])
                {
                    char buffer[1024];
                    sprintf(buffer, "%s_%s:%s", group_name, client->client_name, message);
                    // char *message = strcat(client->client_name,buffer);
                    send_message_to_others(client->client_FD, i, message);
                    return true;
                }
            }
        }
    }
    return false;
}
bool leave_group(int client_FD, char *group_name)
{
    for (int i = 0; i < groups_number; i++)
    {
        if (strcmp(groups_list[i].name, group_name) == 0)
        {
            for (int j = 0; j < groups_list[i].cnt_clinet; j++)
            {
                if (client_FD == groups_list[i].client_list[j]){
                    groups_list[i].client_list[j] = groups_list[i].client_list[(groups_list[i].cnt_clinet)-1];
                    groups_list[i].cnt_clinet--;
                    return true;
                }
                    
            }
        }
    }
    return false;
}

void process_client_command(struct acceptedClient *client, char *command)
{
    int command_size = strlen(command);
    command[command_size - 1] = 0;

    char *ptr = strtok(command, " ");
    char command_split[3][20];
    int cnt = 0;

    while (ptr != NULL)
    {
        printf("%i", cnt);
        strcpy(command_split[cnt], ptr);
        cnt++;
        printf("'%s'\n", ptr);
        ptr = strtok(NULL, " ");
    }

    printf("%i ", cnt);
    if (cnt > 0 && cnt <= 3)
    {
        printf("&&&");
        printf("%i", strcmp(command_split[0], "createGroup") == 0 && cnt == 2);
        if (strcmp(command_split[0], "createGroup") == 0 && cnt == 2)
        {
            create_group(client->client_FD, command_split[1]);
            printf("%s create %s group \n", client->client_name, command_split[1]);
            return;
        }

        if (strcmp(command_split[0], "join") == 0 && cnt == 2)
        {
            printf("here");
            join_group(client->client_FD, command_split[1]);
            printf("%s join to %s group \n", client->client_name, command_split[1]);
            return;
        }
        if (strcmp(command_split[0], "send") == 0 && cnt == 3)
        {
            send_message_group(client, command_split[1], command_split[2]);
            printf("%s send %s to %s group \n", client->client_name, command_split[2], command_split[1]);
            return;
        }
        if (strcmp(command_split[0], "leave") == 0 && cnt == 2)
        {
            leave_group(client->client_FD, command_split[1]);
            printf("%s leave %s group \n", client->client_name, command_split[1]);
            return;
        }
    }
    char *message = "your command isnt correct \n";
    send(client->client_FD, message, 1024, 0);
}

void recieve_client_messages(struct acceptedClient *client)
{
    char buffer[1024] = {0};
    while (1)
    {
        ssize_t valread = recv(client->client_FD, buffer, 1024, 0);
        if (valread == 0)
            break;

        process_client_command(client, buffer);

        printf("%s : %s", client->client_name, buffer);
    }
    close(client->client_FD);
}

void reciever_thread(struct acceptedClient *client)
{
    pthread_t id;
    pthread_create(&id, NULL, recieve_client_messages, client);
}

void send_message_to_others(int client_FD, int index, char *message)
{

    for (int i = 0; i < groups_list[i].cnt_clinet; i++)
    {
        if (groups_list[index].client_list[i] != client_FD)
        {
            printf("%li", sizeof(message));
            send(client_list[i].client_FD, message, 1024, 0);
        }
    }
}

int main(int argc, char const *argv[])
{

    int server_fd = setup_server(argv);

    // 4. accept
    while (1)
    {
        if (online_clients == 10)
        {
            continue;
        }
        struct acceptedClient *client = accept_client(server_fd);
        client_list[online_clients++] = *client;
        // 5. recieve messages
        reciever_thread(client);
    }

    shutdown(server_fd, SHUT_RDWR);

    return 0;
}