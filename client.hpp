#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Client{
private:
	int socket_fd;
	struct sockaddr_in target;
public:
	Client();
	bool init();
	void receive_string();
	bool send_string();
	
};

#endif

