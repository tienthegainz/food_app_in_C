#include <cjson/cJSON.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

struct info parse_Json(char *string)
{
    static struct info data;
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

    cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "name");
    cJSON *date = cJSON_GetObjectItemCaseSensitive(json, "date");
    cJSON *food = cJSON_GetObjectItemCaseSensitive(json, "food");
    cJSON *seat = cJSON_GetObjectItemCaseSensitive(json, "seat");
    cJSON *phone = cJSON_GetObjectItemCaseSensitive(json, "phone");
    if (!cJSON_IsString(date) || !cJSON_IsString(name) || !cJSON_IsString(food) || !cJSON_IsNumber(seat) || !cJSON_IsString(phone))
    {
        puts("Empty\n");
        goto end;
    }
    else
    {
        data.seat = seat->valueint;
        strcpy(data.name, name->valuestring);
        strcpy(data.date, date->valuestring);
        strcpy(data.food, food->valuestring);
        strcpy(data.phone, phone->valuestring);
    }
    return data;

end:
    cJSON_Delete(json);
    return data;
}

/*int main(int argc, char const *argv[])
{
    char *json = "{ \
		\"name\":	\"Tran Dan\", \
		\"date\":	\"12-12-2019\", \
		\"food\":	\"Tra tac\", \
        \"seat\":	5, \
		\"phone\":	\"1234567\" \
	} \
    ";
    struct info data = parse_Json(json);
    printf("Name: %s -- Phone: %s\nNumber of people: %d\nFood: %s -- Date: %s\n", data.name, data.phone, data.seat, data.food, data.date);
    return 0;
}
*/
