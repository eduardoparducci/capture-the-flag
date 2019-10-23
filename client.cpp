#include "client.hpp"
#include <stdio.h>
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

bool Client::send_string(){
	
  /* Agora, meu socket funciona como um descritor de arquivo usual */
  send(socket_fd, "PING", 5, 0);
  printf("Escrevi mensagem de ping!\n");
  sleep(1);
}

void Client::receive_string(){
/* Recebendo resposta */
  char reply[10];
  recv(this->socket_fd, reply, 10, 0);
  printf("Resposta:\n%s\n", reply);
  close(socket_fd);
  return ;
}


