#include <iostream>
#include <string>
#include "server.hpp"
int main() {
  Client *client = new Server();
  std::string input_buffer;

  client->init();
  client->receive_string();
  std::cout << "Received: " << input_buffer << std::endl;
  client->send_string();
}