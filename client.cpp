#include "client.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <chrono>
#include <stdlib.h>

Client::Client() {
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  std::cout << "Socket created " << std::endl;
}


void wait_package(char **buffer, int buffer_size, bool *buffer_status, bool *running, int socket_fd) {  
  while(*running) {
    if(*buffer_status==FREE) {
      *buffer[0]= '\0';
      recv(socket_fd, *buffer, buffer_size, 0);
      *buffer_status = BUSY;
    }
    // ADJUST TO 10ms LATER
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
  }
  return;
}

bool Client::init(unsigned int gate, std::string ip, int buffer_size) {
  // Buffer setup
  this->buffer_size = buffer_size;
  this->buffer_status = FREE;
  this->buffer = (char *)malloc(buffer_size * sizeof(char));

  this->target.sin_family = AF_INET;
  this->target.sin_port = htons(gate);
  inet_aton(ip.c_str(), &(this->target.sin_addr));
  
  std::cout << "Trying to connect on gate:" << gate << " ip:" << ip << std::endl;

  if (connect(this->socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    std::cout << "Problems occured, exiting..." << std::endl;
    return false;
  }
  std::cout << "Connection stablished!" << std::endl;
  this->running = true;
  std::thread newthread(wait_package, &(this->buffer), this->buffer_size, &(this->buffer_status), &(this->running), this->socket_fd);
  (this->pkg_thread).swap(newthread);

  return true;
}

void Client::cclose() {
  this->running = false;
  (this->pkg_thread).join();
  free(this->buffer);
  close(socket_fd);
}

bool Client::send_string(std::string data) {
  std::cout << "Sending:"<< data << " ... ";
  if(send(this->socket_fd, data.c_str(), data.size()+1, 0) < 0) {
    std::cout << "Error!" << std::endl;
    return false;
  }
  std::cout << "Success!" << std::endl;
  return true;
}

std::string Client::get_string() {
  if(this->buffer_status==FREE) return "";
  this->buffer_status = FREE;
  return this->buffer;  
}


