#ifndef CLIENT_HPP
#define CLIENT_HPP

// external libraries
#include "../libraries.hpp"
#include "../data/json.hpp"
#include "../game/map.hpp"
#include "../game/obstacles.hpp"
#include "../game/players.hpp"

#define BUSY true
#define FREE false

#define JSON_BUFFER_SIZE 1000

using json = nlohmann::json;
using namespace std;

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
	Client(unsigned int gate, string ip, int buffer_size);
	bool init(Player *player, Map *map, ObstacleList *obstacles);
  string getString();
	bool sendString(string data);
  bool getBufferStatus();
  void cclose();
  Player *getPlayer();
  Map *getMap();
  ObstacleList *getObstacleList();
};

#endif

