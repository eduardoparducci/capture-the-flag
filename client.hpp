#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

#define BUSY true
#define FREE false

class Client{
private:
	int socket_fd;
	struct sockaddr_in target;
  bool running;
  bool buffer_status;
  int buffer_size;
  char *buffer;
  std::thread pkg_thread;
  
public:
	Client();
	bool init(unsigned int gate, std::string ip, int buffer_size);
  std::string get_string();
	bool send_string(std::string data);
  void cclose();
};

#endif

