#ifndef SERVER_HPP
#define SERVER_HPP

#include "game.hpp"
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

#include "json.hpp"
#define JSON_BUFFER_SIZE 1000

#define BUSY true
#define FREE false

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
  std::string ip;
  std::thread pkg_thread;

  // JSON buffer test
  nlohmann::json json_buffer;
  bool json_buffer_status;
  
public:
  Server(unsigned int gate, std::string ip, unsigned int buffer_size);
  bool init(Physics *physics);
  void sclose();
  void slisten();
  std::string get_string();
  bool send_string(std::string data);
  void updateGame(std::string movement);

  void updateGameJson(nlohmann::json movement);
  nlohmann::json getJson();
};

#endif
