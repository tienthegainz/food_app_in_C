// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cjson/cJSON.h>
#include <sys/time.h>
#include <errno.h>
#include <netdb.h>

#define MAX_CHAR 2000
#define SA struct sockaddr

struct r_info
{
    char name[MAX_CHAR];
    char date[MAX_CHAR];
    char food[MAX_CHAR];
    int seat;
    char phone[MAX_CHAR];
};

void order_food(char *localhost, int port, char *food);

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Missing arguments\n");
        return -1;
    }
    int port = atoi(argv[1]);
    order_food("127.0.0.1", port, "Bong cuoi");
    return 0;
}

void order_food(char *localhost, int port, char *food)
{
    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(localhost);
    servaddr.sin_port = htons(port);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                   sizeof(timeout)) < 0)
        printf("setsockopt failed\n");

    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                   sizeof(timeout)) < 0)
        printf("setsockopt failed\n");
    //
    struct r_info data;
    int check = 0;
    char *msg = NULL;
    char buffer[MAX_CHAR];
    do
    {
        printf("Enter your name:\n");
        scanf("%s%*c", data.name);
        printf("Enter your phone number:\n");
        scanf("%s%*c", data.phone);
        printf("Enter your seat:");
        scanf("%d%*c", &data.seat);
        printf("Enter your date:\n");
        scanf("%s%*c", data.date);
        printf("Is this correct(1 is true):\n");
        scanf("%d%*c", &check);
    } while (check != 1);
    // make json
    cJSON *json = cJSON_CreateObject();

    if (cJSON_AddStringToObject(json, "name", data.name) == NULL)
    {
        exit(-1);
    }
    if (cJSON_AddStringToObject(json, "food", food) == NULL)
    {
        exit(-1);
    }
    if (cJSON_AddStringToObject(json, "phone", data.phone) == NULL)
    {
        exit(-1);
    }
    if (cJSON_AddStringToObject(json, "date", data.date) == NULL)
    {
        exit(-1);
    }
    if (cJSON_AddNumberToObject(json, "seat", data.seat) == NULL)
    {
        exit(-1);
    }
    msg = cJSON_Print(json);
    if (msg == NULL)
    {
        fprintf(stderr, "Failed to print monitor.\n");
        cJSON_Delete(json);
        exit(-1);
    }
    puts(msg);
    write(sockfd, msg, strlen(msg));
    // close the socket
    if (read(sockfd, buffer, sizeof(buffer)) == -1)
    {
        printf("ERROR: %s\n", strerror(errno));
    }
    puts(buffer);

    close(sockfd);
    return;
}