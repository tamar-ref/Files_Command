.PHONY: all server client

all: server client

server:
	gcc server/server.c -o server/server

client:
	gcc client/client.c -o client/client