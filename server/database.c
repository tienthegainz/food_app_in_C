#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include <cjson/cJSON.h>
#include <sqlite3.h>

sqlite3 *open_database()
{
    sqlite3 *db;

    if (sqlite3_open(DATABASE_PATH, &db))
    {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        return (0);
    }
    else
    {
        printf("Opened database successfully from %s\n", DATABASE_PATH);
    }
    return db;
}

static int callback(void *data, int argc, char **argv, char **azColName)
{
    // init json object
    cJSON *query = cJSON_CreateObject();
    for (int i = 0; i < argc; i++)
    {
        //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        cJSON *col = cJSON_CreateString(argv[i] ? argv[i] : "NULL");
        cJSON_AddItemToObject(query, azColName[i], col);
    }
    cJSON_AddItemToArray(data, query);
    return 0;
}

char *make_query(char *food_name)
{
    char *query = (char *)malloc(sizeof(char) * MAX_CHAR);
    strcat(query, "SELECT restaurant.id, name, domain, food, address FROM food, restaurant WHERE food LIKE '%");
    strcat(query, food_name);
    strcat(query, "%'");
    return query;
}

char *select_from_database(sqlite3 *db, char *query)
{
    // init json as array
    cJSON *data = cJSON_CreateArray();
    char *zErrMsg = 0;
    if (data == NULL)
    {
        goto end;
    }
    /* Execute SQL statement */
    int rc = sqlite3_exec(db, query, callback, (void *)data, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }
    // printout json
    char *string = cJSON_Print(data);
    if (string == NULL)
    {
        fprintf(stderr, "Failed to print monitor.\n");
        return NULL;
    }
    //printf("%s\n", string);
end:
    cJSON_Delete(data);
    return string;
}

int main(int argc, char const *argv[])
{
    sqlite3 *db = open_database();
    char *query = make_query("tac");
    printf("%s", query);
    char *result = select_from_database(db, query);
    printf("%s", result);
    sqlite3_close(db);
    return 0;
}
