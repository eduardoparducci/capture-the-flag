#include <iostream>
#include <string>
#include "client.hpp"
#include <unistd.h>

using namespace std;
int main() {
  Client *client = new Client();
  string server_data, client_data;
  
  if(!client->init(3001, "127.0.0.1", 20)) return 1;

  while(1) {
    cout << "Message:";
    cin >> client_data;
    client->send_string(client_data);
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    server_data = client->get_string();
    if(server_data.size())
      std::cout << endl<< "Received: " << server_data << endl;
    if(client_data=="goodbye") break;
  }
  client->cclose();
  return 0;
}
