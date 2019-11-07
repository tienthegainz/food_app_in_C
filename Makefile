CC = gcc
CFLAGS = -c -Wall

all: server/database.o
	$(CC) -o server/database server/database.o -l sqlite3 -l cjson
database.o: server/database.c
	$(CC) $(CFLAGS) server/database.c

clean: 
	rm *.o server/database