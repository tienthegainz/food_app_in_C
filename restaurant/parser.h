#include <stdio.h>
#define MAX_CHAR 1000

struct info
{
    char name[MAX_CHAR];
    char date[MAX_CHAR];
    char food[MAX_CHAR];
    int seat;
    char phone[MAX_CHAR];
};

struct info parse_Json(char *string);