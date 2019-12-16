// Server UDP
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <sys/time.h>
#include "database.h"
#include <cjson/cJSON.h>
#include <sqlite3.h>

#define MAXLINE 3000
#define SA struct sockaddr
#define LOCALHOST "127.0.0.1"
#define END_QUERY "$QUERY$"

void query_food(sqlite3 *db, int connfd);

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Missing arguments\n");
        return -1;
    }
    int port = atoi(argv[1]);
    int sockfd, connfd;
    unsigned int len;
    struct sockaddr_in servaddr, cli;
    sqlite3 *db = open_database();

    // socket create and verification
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Open port to listen

    printf("Listening on port %s/%d\n", LOCALHOST, port);
    printf("Press Ctrl+C to stop\n");
    fork();
    fork();
    while (1)
    {
        if ((listen(sockfd, 5)) != 0)
        {
            printf("Listen failed...\n");
            exit(0);
        }
        else
            printf("Server listening..\n");
        len = sizeof(cli);
        // Accept the data packet from client and verification
        connfd = accept(sockfd, (SA *)&cli, &len);

        if (connfd < 0)
        {
            printf("server acccept failed...\n");
            exit(0);
        }
        else
            printf("server acccept the client...\n");

        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 20;
        timeout.tv_usec = 0;

        if (setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                       sizeof(timeout)) < 0)
            printf("setsockopt failed\n");

        if (setsockopt(connfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                       sizeof(timeout)) < 0)
            printf("setsockopt failed\n");
        query_food(db, connfd);
        close(connfd);
    }

    return 0;
}

void query_food(sqlite3 *db, int connfd)
{
    char buffer[MAXLINE];

    while (1)
    {
        bzero(buffer, sizeof(buffer));
        if (read(connfd, buffer, sizeof(buffer)) == -1)
        {
            printf("ERROR: %s\n", strerror(errno));
        }
        if (strcmp(buffer, "") == 0 || strcmp(buffer, "\n") == 0)
            continue;
        else if (strcmp(buffer, END_QUERY) == 0)
            break;
        // printf("Received: %s - %ld\n", buffer, strlen);
        // while (1)
        // {
        //     sleep(50);
        // }
        char *query = make_query(buffer);
        // printf("%s\n", query);
        char *result = select_from_database(db, query);
        // printf("%s\n%ld\n", result, strlen(result));

        write(connfd, result, strlen(result));
    }

    return;
}