CC = gcc
CFLAGS = -c -Wall

all: server/database.o server/server.o client/parser.o client/client.o restaurant/parser.o restaurant/restaurant_server.o
	$(CC) -o server/server server/database.o server/server.o -l sqlite3 -l cjson
	$(CC) -o client/client client/parser.o client/client.o -l cjson
	$(CC) -o restaurant/r_server restaurant/parser.o restaurant/restaurant_server.o -l cjson

database.o: server/database.c
	$(CC) $(CFLAGS) server/database.c

server.o: server/server.c
	$(CC) $(CFLAGS) server/server.c

parser.o: client/parser.c
	$(CC) $(CFLAGS) client/parser.c

r_parser.o: restaurant/parser.c
	$(CC) $(CFLAGS) restaurant/parser.c

client.o: client/client.c
	$(CC) $(CFLAGS) client/client.c

r_server.o: restaurant/restaurant_server.c
	$(CC) $(CFLAGS) restaurant/restaurant_server.c

clean: 
	