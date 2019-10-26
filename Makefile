all: clean server client

server: clrserver
	g++ -oserver main_server.cpp server.cpp -lncurses -lpthread

client: clrclient
	g++ -oclient main_client.cpp client.cpp -lncurses -lpthread

clean: clrserver clrclient

clrserver:
ifneq (,$(wildcard ./server))
	rm ./server
endif

clrclient:
ifneq (,$(wildcard ./client))
	rm ./client
endif
