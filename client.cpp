#include "client.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Client::Client() {
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  std::cout << "Socket created " << std::endl;
}

bool Client::init() {
  target.sin_family = AF_INET;
  target.sin_port = htons(3001);
  inet_aton("127.0.0.1", &(target.sin_addr));
  printf("Tentando conectar\n");
  if (connect(this->socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    printf("Problemas na conexao\n");
    return false ;
  }
  printf("Conectei ao servidor\n");
}



bool Client::send_string(std::string data){
  std::string s;
  s = "PING";
  /* Agora, meu socket funciona como um descritor de arquivo usual */
  send(socket_fd, s.c_str(), s.size()+1, 0);
  sleep(1);
}



void Client::receive_string(char *data, int max_data_size){
/* Recebendo resposta */
  char reply[10];
  recv(this->socket_fd, data, max_data_size, 0);
  std::cout << "Data received" << std::endl;
  close(socket_fd);
  return ;
}


