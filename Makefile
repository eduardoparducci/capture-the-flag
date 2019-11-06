all: clean server client

server: clrserver
	g++ -L~/opt -oserver main_server.cpp server.cpp game.cpp -lncurses -lpthread -std=c++11

client: clrclient
	g++ -L./opt/lib/ -oclient main_client.cpp client.cpp gtk.cpp game.cpp -lncurses -lpthread -lglut -lGL -lGLU -lm -std=c++11

clean: clrserver clrclient

clrserver:
ifneq (,$(wildcard ./server))
	rm ./server
endif

clrclient:
ifneq (,$(wildcard ./client))
	rm ./client
endif
