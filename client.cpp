#include "client.hpp"
#include "game.hpp"
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
#include <mutex>

std::mutex buffer_access;

void wait_package(char **buffer, int buffer_size, bool *buffer_status, bool *running, int socket_fd) {  
  while(*running) {
    if(*buffer_status==FREE) {
      *buffer[0]= '\0';
      recv(socket_fd, *buffer, buffer_size, 0);
      *buffer_status = BUSY;
    }
    // ADJUST TO 10ms LATER
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
  }
  return;
}

Client::Client(unsigned int gate, std::string ip, int buffer_size) {
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  // Buffer setup
  buffer_access.lock();
  this->buffer_size = buffer_size;
  this->buffer_status = FREE;
  this->buffer = (char *)malloc(buffer_size * sizeof(char));
  buffer_access.unlock();
  std::cout << "Buffer created with " << buffer_size << "bytes" << std::endl;
  

  this->ip = ip;
  this->target.sin_family = AF_INET;
  this->target.sin_port = htons(gate);
  inet_aton(ip.c_str(), &(this->target.sin_addr));

  std::cout << "Socket created " << std::endl;
}

bool Client::init(Player *player, Map *map, ObstacleList *obstacles) {
  this->player = player;
  this->map = map;
  this->obstacles = obstacles;
  std::cout << "Trying to connect on gate:" << this->gate << " ip:" << this->ip << std::endl;

  if (connect(this->socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    std::cout << "Problems occured, exiting..." << std::endl;
    return false;
  }
  std::cout << "Connection stablished!" << std::endl;
  this->running = true;
  std::thread newthread(wait_package, &(this->buffer), this->buffer_size, &(this->buffer_status), &(this->running), this->socket_fd);
  (this->pkg_thread).swap(newthread);
  
  std::cout << "Adding player: " <<  player->getName() << "(Not implemented yet)" << std::endl;
  return true;
}

void Client::cclose() {
  this->running = false;
  (this->pkg_thread).join();
  free(this->buffer);
  close(socket_fd);
}

bool Client::sendString(std::string data) {
  std::cout << "Sending:"<< data << " ... ";
  if(send(this->socket_fd, data.c_str(), data.size()+1, 0) < 0) {
    std::cout << "Error!" << std::endl;
    return false;
  }
  std::cout << "Success!" << std::endl;
  return true;
}

Player *Client::getPlayer() {
  return this->player;
}

ObstacleList *Client::getObstacleList() {
  return this->obstacles;
}

Map *Client::getMap() {
  return this->map;
}

std::string Client::getString() {
  if(this->buffer_status==FREE) {
    return "";
  }
  std::string data(this->buffer);
  this->buffer_status = FREE;
  return data;  
}

bool Client::getBufferStatus() {
  bool status = this->buffer_status;
  return status;
}
