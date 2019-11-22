GAMEFILES=game/players.cpp game/physics.cpp game/obstacles.cpp game/map.cpp
GAMEINCLUDE = -Igame/ $(GAMEFILES)
GTKINCLUDE = -Igtk/ gtk/gtk.cpp
SERVERINCLUDE = -Inetwork/ network/server.cpp
CLIENTINCLUDE = -Inetwork/ network/client.cpp
GLUTINCLUDE = -L./opt/lib/
SERVERFLAGS = $(GAMEINCLUDE) $(SERVERINCLUDE) -std=c++11 -lncurses -lpthread -o 
CLIENTFLAGS = $(GLUTINCLUDE) $(GAMEINCLUDE) $(CLIENTINCLUDE) $(GTKINCLUDE) -std=c++11 -lncurses -lpthread -lglut -lGL -lGLU -lm -o

all: clean server client

server: clrserver
	g++ $(SERVERFLAGS) server main_server.cpp

client: clrclient
	g++ $(CLIENTFLAGS) client main_client.cpp

clean: clrserver clrclient

clrserver:
ifneq (,$(wildcard ./server))
	rm ./server
endif

clrclient:
ifneq (,$(wildcard ./client))
	rm ./client
endif
