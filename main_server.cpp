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

  // Game variables
  Map *map = new Map({100.0f, 100.0f, -100.0f, -100.0f}, {100.0f, 100.0f, 10.0f, -100.0f});
  Obstacle *o0 = new Obstacle ({30.0f, 30.0f, 20.0f, 20.0f},{0,0,0});
  Obstacle *o1 = new Obstacle ({30.0f, -30.0f, 20.0f, -40.0f},{0,0,0});
  ObstacleList *obs = new ObstacleList();
  Physics *physics;

  // Connection variables
  Server *server = new Server(3001,"127.0.0.1", 2000);
  json client_data, last_client_data;

  // Configure new game
  obs->addObstacle(o0);
  obs->addObstacle(o1);
  physics = new Physics(map,obs);

  // Start server
  if(!server->init(physics)) return 1;
  server->slisten();

  // Start game loop
  server->getPhysics()->start();
  while(1) {
    client_data = server->getPackage();

    // Fetched package
    if(!client_data.empty()) {

      // New client
      if(!client_data["init"].empty()) {
        server->addClient(client_data);
      } else {
        
        // Player movement
        last_client_data = client_data;
      }
    }
    // Update game state
    server->updateGame(last_client_data);
    std::this_thread::sleep_for (std::chrono::milliseconds(20));
  }
  server->sclose();
  return 0;
}
