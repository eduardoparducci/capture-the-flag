#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

#define BUSY true
#define FREE false

class Server {
private:
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
  
public:
  Server(unsigned int gate, std::string ip, unsigned int buffer_size);
  bool init();
  void sclose();
  void slisten();
  std::string get_string();
  bool send_string(std::string data);
};

#endif
