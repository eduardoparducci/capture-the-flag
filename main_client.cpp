#include "libraries.hpp"

#include "game/map.hpp"
#include "game/obstacles.hpp"
#include "game/players.hpp"
#include "network/client.hpp"
#include "data/json.hpp"
#include "data/structures.hpp"
#include "gtk/gtk.hpp"

using namespace std;
int main(int argc, char **argv) {
  Map *map = new Map({100.0f, 100.0f, -100.0f, -100.0f}, {100.0f, 100.0f, 10.0f, -100.0f});
  Obstacle *o0 = new Obstacle ({30.0f, 30.0f, 20.0f, 20.0f},{0,0,0});
  Obstacle *o1 = new Obstacle ({30.0f, -30.0f, 20.0f, -40.0f},{0,0,0});
  ObstacleList *obs = new ObstacleList();
  Player *player = new Player(0.0f, 0.0f, 7.0f, 5.0f, "Eduardo", {1.0f, 0.0f, 0.0f}, 0);
  Client *client = new Client(3001, "127.0.0.1", 2000);
  Gtk *gtk = new Gtk();
  string server_data;

  obs->add_obstacle(o0);
  obs->add_obstacle(o1);
  if(!client->init(player, map, obs)) return 1;
  gtk->init(argc, argv, client);
  client->cclose();
  return 0;
}
