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
#include "parser.h"
#include <time.h>

// #define MAX_CHAR 2000
#define SA struct sockaddr
#define END_QUERY "$QUERY$"

struct r_info
{
    char name[MAX_CHAR];
    char date[MAX_CHAR];
    char food[MAX_CHAR];
    int seat;
    char phone[MAX_CHAR];
};

struct h_info
{
    char host[MAX_CHAR];
    int port;
};

int order_food(char *localhost, int port, char *food);
struct h_info parse_address(char *add);
int check_date(char *date);

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        printf("Missing arguments\n");
        return -1;
    }

    char localhost[MAX_CHAR];
    strcpy(localhost, argv[1]);
    int port = atoi(argv[2]);
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
    timeout.tv_sec = 20;
    timeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                   sizeof(timeout)) < 0)
        printf("setsockopt failed\n");

    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                   sizeof(timeout)) < 0)
        printf("setsockopt failed\n");

    char buff[MAX_CHAR];
    char choice;
    struct info *data;
    do
    {
        printf("--------------------------\n");
        printf("Nhập món ăn muốn tìm kiếm: ");
        scanf("%[^\n]s", buff);
        scanf("%*c");

        if (write(sockfd, buff, strlen(buff)) == -1)
        {
            printf("ERROR: %s\n", strerror(errno));
            exit(-1);
        }

        bzero(buff, sizeof(buff));

        // receive analyzed
        if (read(sockfd, buff, sizeof(buff)) == -1)
        {
            printf("ERROR: %s\n", strerror(errno));
            exit(-1);
        }
        // puts(buff);
        if (strcmp(buff, "") == 0)
        {
            printf("Empty respond from server. Server may down. Exiting.....\n");
            exit(-1);
        }

        data = parse_Json(buff);

        for (int i = 0; i < 5; i++)
        {
            printf("Id:%d\n Tên: %s\n Miền: %s\n Món: %s\n Đc: %s\n", i + 1, data[i].name, data[i].domain, data[i].food, data[i].address);
        }
        printf("--------------------------\n");
        printf("Nhập 1 để chọn nhà hàng, 2 để chọn lại món, khác để thoát: ");
        scanf("%c%*c", &choice);
        buff[0] = '\0';
    } while (choice == '2');
    write(sockfd, END_QUERY, strlen(END_QUERY));
    close(sockfd);
    while (choice == '1')
    {
        int id;
        printf("--------------------------\n");
        printf("Chọn theo id nhà hàng:");
        scanf("%d%*c", &id);
        struct h_info info = parse_address(data[id - 1].domain);
        if (info.port == 0)
        {
            printf("Can't understand this host. Database ERROR\n");
            return 0;
        }

        // order food
        if (order_food(info.host, info.port, data[id - 1].food) == 0)
        {
            printf("Rebooting...\n");
            printf("Nhập 1 để chọn nhà hàng, 2 để chọn lại món, khác để thoát: ");
            scanf("%c%*c", &choice);
            continue;
        }
        choice = '2';
    }

    return 0;
}

int order_food(char *localhost, int port, char *food)
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
        close(sockfd);
        return 0;
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
    int valid_date = -1;
    char *msg = NULL;
    char buffer[MAX_CHAR];
    do
    {
        printf("Enter your name:\n");
        scanf("%s%*c", data.name);
        printf("Enter your phone number:\n");
        scanf("%s%*c", data.phone);
        printf("Number of people:");
        scanf("%d%*c", &data.seat);
        printf("Enter your date(M/D/Y):\n");
        scanf("%s%*c", data.date);
        printf("Is this correct(1 is true):\n");
        scanf("%d%*c", &check);
        valid_date = check_date(data.date);
        if (valid_date != 1)
            printf("DATE ERROR\n");
    } while (check != 1 || valid_date != 1);
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
    // puts(msg);
    if (write(sockfd, msg, strlen(msg)) == -1)
    {
        printf("ERROR: %s\n", strerror(errno));
        close(sockfd);
        return 0;
    }
    bzero(buffer, sizeof(buffer));
    if (read(sockfd, buffer, sizeof(buffer)) == -1)
    {
        printf("ERROR: %s\n", strerror(errno));
        close(sockfd);
        return 0;
    }
    if (strcmp(buffer, "") == 0)
    {
        printf("Empty respond from server. Server may down. Exiting.....\n");
        close(sockfd);
        return 0;
    }
    puts(buffer);

    close(sockfd);
    return 1;
}

struct h_info parse_address(char *add)
{
    struct h_info info;
    char port[MAX_CHAR];
    int i = 0;
    int j = 0;
    for (; add[i] != '\0'; i++)
    {
        if (add[i] == ':')
        {
            i++;
            break;
        }
        info.host[i] = add[i];
    }
    for (; add[i] != '\0'; i++)
    {
        port[j] = add[i];
        j++;
    }
    // printf("%s\n", port);
    info.port = atoi(port);
    printf("HOST: %s -- PORT: %d\n", info.host, info.port);
    return info;
}

int check_date(char *date_str)
{
    struct tm date;
    strptime(date_str, "%m/%d/%Y", &date);
    // printf("Month: %d -- Day: %d -- Year: %d\n", date.tm_mon, date.tm_mday, date.tm_mday);
    if (date.tm_mon > 11 || date.tm_mon < 0)
    {
        return 0;
    }
    else if (date.tm_mday > 31 || date.tm_mday < 1)
    {
        return 0;
    }
    else if (date.tm_mon == 1 && (date.tm_mday > 29 || date.tm_mday < 1))
    {
        return 0;
    }
    return 1;
}