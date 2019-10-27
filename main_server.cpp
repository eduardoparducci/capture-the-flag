#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "server.hpp"

using namespace std;
int main() {
  Server *server = new Server(3001,"127.0.0.1", 20);
  string server_data, client_data;

  if(!server->init()) return 1;
  server->slisten();

  while(1) {
    client_data = server->get_string();
    if(client_data.size()) {
      cout << endl << "Client said: " << client_data << endl;
    }
    if(client_data=="q+") {
      server->send_string("closing");
      cout << "Closing..." << endl;
      break;
    }
  }
  server->sclose();
  return 0;
}
