#include <stdio.h>
#define MAX_CHAR 1000
struct info
{
    int id;
    char name[MAX_CHAR];
    char domain[MAX_CHAR];
    char food[MAX_CHAR];
    char address[MAX_CHAR];
};

struct info *parse_Json(char *string);