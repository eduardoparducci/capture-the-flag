#include "client.hpp"

void wait_package(char **buffer, int buffer_size, bool *buffer_status, bool *running, int socket_fd) {
  while(*running) {
    if(*buffer_status==FREE) {
      *buffer[0]= '\0';
      recv(socket_fd, *buffer, buffer_size, 0);
      *buffer_status = BUSY;
    }
    std::this_thread::sleep_for (chrono::milliseconds(10));
  }
  return;
}

Client::Client(unsigned int gate, string ip, int buffer_size) {
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  // Buffer setup
  this->buffer_size = buffer_size;
  this->buffer_status = FREE;
  this->buffer = (char *)malloc(buffer_size * sizeof(char));
  cout << "Buffer created with " << buffer_size << "bytes" << endl;  

  this->ip = ip;
  this->target.sin_family = AF_INET;
  this->target.sin_port = htons(gate);
  inet_aton(ip.c_str(), &(this->target.sin_addr));

  cout << "Socket created " << endl;
}

bool Client::init(Player *player, Map *map, ObstacleList *obstacles) {
  this->player = player;
  this->map = map;
  this->obstacles = obstacles;
  cout << "Trying to connect on gate:" << this->gate << " ip:" << this->ip << endl;

  if (connect(this->socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    cout << "Problems occured, exiting..." << endl;
    return false;
  }
  cout << "Connection stablished!" << endl;
  this->running = true;
  thread newthread(wait_package, &(this->buffer), this->buffer_size, &(this->buffer_status), &(this->running), this->socket_fd);
  (this->pkg_thread).swap(newthread);
  
  cout << "Adding player: " <<  player->getName() << "(Not implemented yet)" << endl;
  return true;
}

void Client::cclose() {
  this->running = false;
  (this->pkg_thread).join();
  free(this->buffer);
  close(socket_fd);
}

bool Client::sendString(string data) {
  cout << "Sending:"<< data << " ... ";
  if(send(this->socket_fd, data.c_str(), data.size()+1, 0) < 0) {
    cout << "Error!" << endl;
    return false;
  }
  cout << "Success!" << endl;
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

string Client::getString() {
  if(this->buffer_status==FREE) {
    return "";
  }
  string data(this->buffer);
  this->buffer_status = FREE;
  return data;  
}

bool Client::getBufferStatus() {
  bool status = this->buffer_status;
  return status;
}
