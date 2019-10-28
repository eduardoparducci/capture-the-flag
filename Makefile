all: clean server client

server: clrserver
	g++ -oserver main_server.cpp server.cpp game.cpp -lncurses -lpthread

client: clrclient
	g++ -oclient main_client.cpp client.cpp gtk.cpp game.cpp -lncurses -lpthread -lglut -lGL -lGLU -lm

clean: clrserver clrclient

clrserver:
ifneq (,$(wildcard ./server))
	rm ./server
endif

clrclient:
ifneq (,$(wildcard ./client))
	rm ./client
endif
