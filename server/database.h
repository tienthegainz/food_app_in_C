#include <stdio.h>
#include <sqlite3.h>
#include <cjson/cJSON.h>
#define DATABASE_PATH "../data.db"
#define MAX_CHAR 1000
#define MAX_QUERY 5

sqlite3 *open_database();
static int callback(void *data, int argc, char **argv, char **azColName);
char *select_from_database(sqlite3 *db, char *query);
char *make_query(char *food_name);