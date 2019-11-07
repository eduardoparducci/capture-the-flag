#ifndef SERVER_HPP
#define SERVER_HPP

#include "../libraries.hpp"
#include "../data/json.hpp"
#include "../game/physics.hpp"

#define JSON_BUFFER_SIZE 1000
#define BUSY true
#define FREE false

using json = nlohmann::json;
using namespace std;

class Server {
private:
  // Game info
  Physics *physics;

  // Connection info
  int socket_fd;
  int connection_fd;
  bool running;
  bool buffer_status;
  int buffer_size;
  char *buffer;
  struct sockaddr_in myself;
  struct sockaddr_in client;
  socklen_t client_size;
  unsigned int gate;
  string ip;
  thread pkg_thread;
  
public:
  Server(unsigned int gate, std::string ip, unsigned int buffer_size);
  bool init(Physics *physics);
  void sclose();
  void slisten();
  string get_string();
  bool send_string(string data);
  void updateGame(string movement);
  void updateGameJson(json movement);
};

#endif
