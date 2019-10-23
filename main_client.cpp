#include <iostream>
#include <string>
#include "client.hpp"
#include <unistd.h>
int main() {
  Client *client = new Client();
  char input_buffer[20];

  client->init();
  client->send_string("PING");
  client->receive_string(input_buffer,20);
  std::cout << "Received: " << input_buffer << std::endl;
  
}