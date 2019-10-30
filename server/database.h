#include <stdio.h>
#include <sqlite3.h>
#define DATABASE_PATH "../data.db"

sqlite3 *open_database();
static int callback(void *data, int argc, char **argv, char **azColName);
void select_from_database(sqlite3 *db, char *query);