#include "libraries.hpp"
#include "game/map.hpp"
#include "game/obstacles.hpp"
#include "game/players.hpp"
#include "game/physics.hpp"
#include "network/server.hpp"
#include "data/json.hpp"
#include "data/structures.hpp"

using json = nlohmann::json;
using namespace std;

int main() {
  Map *map = new Map({100.0f, 100.0f, -100.0f, -100.0f}, {100.0f, 100.0f, 10.0f, -100.0f});
  Obstacle *o0 = new Obstacle ({30.0f, 30.0f, 20.0f, 20.0f},{0,0,0});
  Obstacle *o1 = new Obstacle ({30.0f, -30.0f, 20.0f, -40.0f},{0,0,0});
  ObstacleList *obs = new ObstacleList();
  Physics *physics;
  Player *player = new Player(0.0f, 0.0f, 7.0f, 5.0f, "Eduardo", {1.0f, 0.0f, 0.0f}, 0);
  Server *server = new Server(3001,"127.0.0.1", 2000);
  json client_data, last_client_data;
  
  obs->add_obstacle(o0);
  obs->add_obstacle(o1);
  physics = new Physics(player,map,obs);
  
  if(!server->init(physics)) return 1;
  server->slisten();

  while(1) {
    client_data = server->getPackage();
    if(!client_data.empty()) {
      last_client_data = client_data;
    }
    server->updateGame(last_client_data);
    std::this_thread::sleep_for (std::chrono::milliseconds(20));
  }
  server->sclose();
  return 0;
}
