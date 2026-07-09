.PHONY: all server client

all: server client

server:
	gcc src/server/server.c \
	    src/server/validation.c \
	    src/general/parser.c \
	    src/general/basic_validation.c \
	    -o src/server/server

client:
	gcc src/client/client.c \
	    src/general/parser.c \
	    src/general/basic_validation.c \
	    -o src/client/client
