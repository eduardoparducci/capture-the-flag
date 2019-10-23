#include "server.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Server::Server(unsigned int gate, std::string ip) {
  this->client_size = (socklen_t)sizeof(this->client);
  this->gate = gate;
  this->ip = ip;
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  std::cout << "Socket created " << std::endl;
}

bool Server::init() {
  this->myself.sin_family = AF_INET; //protocolo
  this->myself.sin_port = htons(this->gate); //porta
  inet_aton(this->ip.c_str(), &(this->myself.sin_addr)); //endereco

  std::cout << "trying to open gate " << this->gate <<std::endl;

  if (bind(this->socket_fd, (struct sockaddr*)&this->myself, sizeof(this->myself)) != 0) {
    std::cout << "Problems occured, exiting..." << std::endl;
    return false;
  }
  std::cout << "Opened gate "<< this->gate << " successfully!" << std::endl;
  return true;
}

void Server::sclose() {
  close(this->socket_fd);
}

void Server::slisten() {
  listen(this->socket_fd, 2);
  std::cout << "Listening on gate " << this->gate << std::endl;
  std::cout << "Waiting..." << std::endl;
  this->connection_fd = accept(this->socket_fd, (struct sockaddr*)&this->client, &this->client_size);
}

void Server::receive_string(char *data, int max_data_size) {
  recv(this->connection_fd, data, max_data_size, 0);
  std::cout << "Data received" << std::endl;
}

bool Server::send_string(std::string data) {
  if(send(this->connection_fd, data.c_str(), data.size(), 0) < 0) {
    std::cout << "Error sending data..." << std::endl;
    return false;
  }
  return true;
}
