#include <iostream>
#include <string>
#include "server.hpp"
int main() {
  Server *server = new Server(3001,"127.0.0.1");
  char input_buffer[20];

  server->init();
  server->slisten();
  server->receive_string(input_buffer, 19);
  std::cout << "Received: " << input_buffer << std::endl;
  server->send_string("PONG");
  server->sclose();
}
