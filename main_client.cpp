#include <iostream>
#include <string>
#include "client.hpp"
#include <unistd.h>
int main() {
  Client *client = new Client();
  std::string input_buffer;

  client->init();
  client->send_string();
  client->receive_string();
  std::cout << "Received: " << input_buffer << std::endl;
  
}