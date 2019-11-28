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
  Map *map = new Map({100.0f, 100.0f, -100.0f, -100.0f}, {-80.0f, 100.0f, -100.0f, -100.0f}, {100.0f, 100.0f, 80.0f, -100.0f});
  Obstacle *o0 = new Obstacle ({10.0f, 80.0f, 5.0f, 20.0f},{0.4,0.4,0.4});
  Obstacle *o1 = new Obstacle ({-5.0f, -20.0f, -10.0f, -80.0f},{0.4,0.4,0.4});
  ObstacleList *obs = new ObstacleList();
  Physics *physics;

  // Connection variables
  Server *server;
  json client_data, last_client_data;
  int i;

  string ip;
  unsigned int gate;
  cout << "Server config..." << endl;
  cout << "IP:";
  cin >> ip;
  cout << "GATE:";
  cin >> gate;

  server = new Server(gate,ip, 5000);
  
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
      // Verify new client
      for(i=0 ; i<(int)client_data.size() ; i++) {        
        if(!client_data[i]["init"].empty()) {
          server->addClient(client_data[i]);
        }
      }
      last_client_data = client_data;
    }
    for(i=0 ; i<(int)last_client_data.size() ; i++) {
      // Update game state
      server->updateGame(last_client_data[i]);
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(50));
  }
  server->sclose();
  return 0;
}
