CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

SERVER = number_server
CLIENT = number_client

all: $(SERVER) $(CLIENT)

$(SERVER): server.c
	$(CC) $(CFLAGS) server.c -o $(SERVER)

$(CLIENT): client.c
	$(CC) $(CFLAGS) client.c -o $(CLIENT)

run-server: $(SERVER)
	./$(SERVER) 5555

run-client: $(CLIENT)
	./$(CLIENT) 127.0.0.1 5555 7

clean:
	rm -f $(SERVER) $(CLIENT)
