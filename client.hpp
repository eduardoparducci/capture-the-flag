#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include "game.hpp"

#define BUSY true
#define FREE false

class Client{
private:
  // Game info
  Map *map;
  ObstacleList *obstacles;
  Player *player;

  // Connection info
	int socket_fd;
	struct sockaddr_in target;
  bool running;
  bool buffer_status;
  int buffer_size;
  char *buffer;
  unsigned int gate;
  std::string ip;
  std::thread pkg_thread;
  
public:
	Client(unsigned int gate, std::string ip, int buffer_size);
	bool init(Player *player, Map *map, ObstacleList *obstacles);
  std::string getString();
	bool sendString(std::string data);
  bool getBufferStatus();
  void cclose();
  Player *getPlayer();
  Map *getMap();
  ObstacleList *getObstacleList();
};

#endif

