#include <iostream>
#include <string>
#include "client.hpp"
#include "gtk.hpp"
#include "game.hpp"

using namespace std;
int main(int argc, char **argv) {
  Client *client = new Client();
  string server_data;
  Gtk *gtk = new Gtk();
  Map *map = new Map({100.0f, 100.0f, -100.0f, -100.0f}, {100.0f, 100.0f, 80.0f, -100.0f});
  Player *player = new Player(0.0f, 0.0f, 7.0f, 5.0f, "Eduardo", {1.0f, 0.0f, 0.0f});
  Obstacle *o0 = new Obstacle ({30.0f, 30.0f, 20.0f, 20.0f},{0,0,0});
  Obstacle *o1 = new Obstacle ({30.0f, -30.0f, 20.0f, -40.0f},{0,0,0});
  ObstacleList *obs = new ObstacleList();
  obs->add_obstacle(o0);
  obs->add_obstacle(o1);

  if(!client->init(3001, "127.0.0.1", 20)) return 1;
  gtk->init(argc, argv, player, map, obs, client);
  while(1) {
    server_data = client->get_string();
    if(server_data.size())
      std::cout << endl<< "Received: " << server_data << endl;
    if(server_data=="closing") break;
  }
  client->cclose();
  return 0;
}
