#ifndef SERVER_HPP
#define SERVER_HPP

#include "../libraries.hpp"
#include "../data/json.hpp"
#include "../game/physics.hpp"

#define JSON_BUFFER_SIZE 1000
#define MAX_CONNECTIONS 8
#define BUSY true
#define FREE false

using json = nlohmann::json;
using namespace std;

class Server {
private:
  // Game info
  Physics *physics;
  bool running;

  // Connection info
  int socket_fd;
  int connection_fd[MAX_CONNECTIONS];
  bool used_connections[MAX_CONNECTIONS];
  bool buffer_status[MAX_CONNECTIONS];
  int buffer_size;
  char *buffer[MAX_CONNECTIONS];
  struct sockaddr_in myself;
  struct sockaddr_in client;
  socklen_t client_size;
  unsigned int gate;
  string ip;
  thread pkg_thread[MAX_CONNECTIONS];
  thread connection_thread;
  
public:
  Server(unsigned int gate, std::string ip, unsigned int buffer_size);
  bool init(Physics *physics);
  void sclose();
  void slisten();
  json getPackage();
  bool sendPackage(string data);
  bool addClient(json data);
  void removeClient(json data);
  void updateGame(json state);
  int addConnection(int connection_fd);
  void removeConnection(int user);
  Physics *getPhysics();
};

#endif
