#include "server.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <stdlib.h>

#include "json.hpp"
//std::mutex buffer_access;

void wait_package(char **buffer, int buffer_size, bool *buffer_status, bool *running, int connection_fd) {
  while(*running) {
    if(*buffer_status==FREE) {
      *buffer[0]= '\0';
      recv(connection_fd, *buffer, buffer_size, 0);
      *buffer_status = BUSY;
    }
    // ADJUST TO 10ms LATER
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
  }
  return;
}

void wait_json_package(nlohmann::json *json_buffer, bool *json_buffer_status, bool *running, int connection_fd) {
  char *data = (char *) malloc(JSON_BUFFER_SIZE * sizeof(char));
  while(*running) {
    if(*json_buffer_status==FREE) {
      recv(connection_fd, data, JSON_BUFFER_SIZE, 0);
      *json_buffer = nlohmann::json::parse(data);
      *json_buffer_status = BUSY;
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
  }
  free(data);
}

Server::Server(unsigned int gate, std::string ip, unsigned int buffer_size) {
  // Buffer setup
  this->buffer_size = buffer_size;
  this->buffer_status = FREE;
  this->buffer = (char *)malloc(buffer_size * sizeof(char));
  
  // General Server setup
  this->running = false;
  this->client_size = (socklen_t)sizeof(this->client);
  this->gate = gate;
  this->ip = ip;
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  
  std::cout << "Socket created " << std::endl;
}

bool Server::init(Physics *physics) {
  // Game setup
  this->physics = physics;
  
  // General Server setup
  this->myself.sin_family = AF_INET; //protocol
  this->myself.sin_port = htons(this->gate); //gate
  inet_aton(this->ip.c_str(), &(this->myself.sin_addr)); //address

  std::cout << "Trying to open gate " << this->gate <<std::endl;

  if (bind(this->socket_fd, (struct sockaddr*)&this->myself, sizeof(this->myself)) != 0) {
    std::cout << "Problems occured, exiting..." << std::endl;
    return false;
  }
  std::cout << "Opened gate "<< this->gate << " successfully!" << std::endl;
  return true;
}

void Server::sclose() {
  this->running = false;
  (this->pkg_thread).join();
  free(this->buffer);
  close(this->socket_fd);
}

void Server::slisten() {
  listen(this->socket_fd, 2);
  std::cout << "Listening on gate " << this->gate << std::endl;
  std::cout << "Waiting..." << std::endl;
  this->connection_fd = accept(this->socket_fd, (struct sockaddr*)&this->client, &this->client_size);
  this->running = true;
  std::thread newthread(wait_package, &(this->buffer), this->buffer_size, &(this->buffer_status), &(this->running), this->connection_fd);
  //std::thread newthread(wait_package, &(this->buffer), &(this->buffer_size), &(this->buffer_status), &(this->running), this->connection_fd);
  (this->pkg_thread).swap(newthread);
}

std::string Server::get_string() {
  if(this->buffer_status==FREE) return "";
  std::string data(this->buffer);
  this->buffer_status = FREE;
  //std::cout << "Got some data" << std::endl;
  return this->buffer;  
}

nlohmann::json Server::getJson() {
  nlohmann::json data;
  if(this->json_buffer_status==BUSY) {
    data = this->json_buffer;
    this->json_buffer_status = FREE;
    std::cout << "Got some JSON! :)" << std::endl;
  }
    return data;  
}

bool Server::send_string(std::string data) {
  // std::cout << "Sending:"<< data << " ... ";
  if(send(this->connection_fd, data.c_str(), data.size()+1, 0) < 0) {
    // std::cout << "Error!" << std::endl;
    return false;
  }
  // std::cout << "Success!" << std::endl;
  return true;
}

void Server::updateGame(std::string movement) {
  //std::cout << "Updating server physics ... ";
  if(movement.length()) this->physics->update(movement.at(0) , movement.at(1)=='+'? true : false);
  else this->physics->update(' ',false);
  //std::cout << "Done!" << std::endl;
  std::string data = this->physics->getPlayer()->serialize();
  //std::cout << "Serialized data to send:" << data << std::endl;
  this->send_string(data);
}

void Server::updateGameJson(nlohmann::json movement) {
  std::cout << "Updating server physics with JSON... ";
  this->physics->updateJson(movement);
  std::cout << "Done!" << std::endl;
  
  std::string data = this->physics->getPlayer()->serialize();
  //std::cout << "Serialized data to send:" << data << std::endl;
  this->send_string(data);
}

