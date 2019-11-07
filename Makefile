CC = gcc
CFLAGS = -c -Wall

all: server/database.o client/parser.o
	$(CC) -o server/database server/database.o -l sqlite3 -l cjson
	$(CC) -o client/parser client/parser.o -l cjson

database.o: server/database.c
	$(CC) $(CFLAGS) server/database.c

parser.o: client/parser.c
	$(CC) $(CFLAGS) client/parser.c

clean: 
	rm *.o server/database