#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

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
    int i;
    fprintf(stderr, "%s: ", (const char *)data);

    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

void select_from_database(sqlite3 *db, char *query)
{
    char *sql = "SELECT * FROM restaurant";
    const char *data = "Callback function called";
    char *zErrMsg = 0;

    /* Execute SQL statement */
    int rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }
}

int main(int argc, char const *argv[])
{
    sqlite3 *db = open_database();
    select_from_database(db, "Anything");
    sqlite3_close(db);
    return 0;
}
