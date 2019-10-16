#include <iostream>
#include <string>
#include "server.hpp"
int main() {
  Server *server = new Server(3001,"127.0.0.1");
  std::string input_buffer;

  server->init();
  server->slisten();
  server->receive_string(&input_buffer, 5);
  std::cout << "Received: " << input_buffer << std::endl;
  server->send_string("PONG");
  server->sclose();
}
