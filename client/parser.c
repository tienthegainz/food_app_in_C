#include <cjson/cJSON.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

struct info *parse_Json(char *string)
{
    static struct info data[5];
    int count = 0;
    cJSON *json = cJSON_Parse(string);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        goto end;
    }
    cJSON *element = NULL;
    cJSON_ArrayForEach(element, json)
    {
        if (count > 10)
        {
            break;
        }
        cJSON *id = cJSON_GetObjectItemCaseSensitive(element, "id");
        cJSON *name = cJSON_GetObjectItemCaseSensitive(element, "name");
        cJSON *domain = cJSON_GetObjectItemCaseSensitive(element, "domain");
        cJSON *food = cJSON_GetObjectItemCaseSensitive(element, "food");
        cJSON *address = cJSON_GetObjectItemCaseSensitive(element, "address");
        if (!cJSON_IsString(id) || !cJSON_IsString(name) || !cJSON_IsString(domain) || !cJSON_IsString(food) || !cJSON_IsString(address))
        {
            puts("Empty\n");
            goto end;
        }
        else
        {
            data[count].id = atoi(id->valuestring);
            strcpy(data[count].name, name->valuestring);
            strcpy(data[count].domain, domain->valuestring);
            strcpy(data[count].food, food->valuestring);
            strcpy(data[count].address, address->valuestring);
        }
        count++;
    }
    //printf("%d. %s %s %s %s\n", data[0].id, data[0].name, data[0].domain, data[0].food, data[0].address);
    return data;

end:
    cJSON_Delete(json);
    return NULL;
}

// int main(int argc, char const *argv[])
// {
//     char *json = "[{ \
// 		\"id\":	\"1\", \
// 		\"name\":	\"Tran Dan\", \
// 		\"domain\":	\"127.0.0.1:3500\", \
// 		\"food\":	\"Tra tac\", \
// 		\"address\":	\"Bolsa Street, California\" \
// 	}, { \
// 		\"id\":	\"2\", \
// 		\"name\":	\"Kha BanK\", \
// 		\"domain\":	\"127.0.0.1:3501\", \
// 		\"food\":	\"Tra tac\", \
// 		\"address\":	\"Tu Son, Bac Ninh\" \
// 	} \
//     ]";
//     struct info *data = parse_Json(json);
//     printf("%d. %s %s %s %s\n", data[0].id, data[0].name, data[0].domain, data[0].food, data[0].address);
//     return 0;
// }
