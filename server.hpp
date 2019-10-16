#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Server {
private:
  int socket_fd;
  int connection_fd;
  struct sockaddr_in myself;
  struct sockaddr_in client;
  socklen_t client_size;
  unsigned int gate;
  std::string ip;

public:
  Server(unsigned int gate, std::string ip);
  bool init();
  void sclose();
  void slisten();
  void receive_string(std::string *data, int max_data_size);
  bool send_string(std::string data);
};

#endif
