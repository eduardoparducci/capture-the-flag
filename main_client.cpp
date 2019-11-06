#include <iostream>
#include <string>
#include "client.hpp"
#include "gtk.hpp"
#include "game.hpp"
#include "json.hpp"

using namespace std;
int main(int argc, char **argv) {
  Map *map = new Map({100.0f, 100.0f, -100.0f, -100.0f}, {100.0f, 100.0f, 10.0f, -100.0f});
  Obstacle *o0 = new Obstacle ({30.0f, 30.0f, 20.0f, 20.0f},{0,0,0});
  Obstacle *o1 = new Obstacle ({30.0f, -30.0f, 20.0f, -40.0f},{0,0,0});
  ObstacleList *obs = new ObstacleList();
  Player *player = new Player(0.0f, 0.0f, 7.0f, 5.0f, "Eduardo", {1.0f, 0.0f, 0.0f});
  Client *client = new Client(3001, "127.0.0.1", 200);
  //Gtk *gtk = new Gtk();
  string server_data;

  obs->add_obstacle(o0);
  obs->add_obstacle(o1);
  if(!client->init(player, map, obs)) return 1;
  while(1) { 
    client->sendString(player->toJson());
    std::this_thread::sleep_for (std::chrono::milliseconds(1000));    
  }
  //gtk->init(argc, argv, client);
  client->cclose();
  return 0;
}
