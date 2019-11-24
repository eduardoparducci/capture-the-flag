/* Desenvolvido por: Eduardo  (170272)
 *                   Henrique (174638)
 */

#ifndef GAME_HPP
#define GAME_HPP

// external libraries
#include "../libraries.hpp"
#include "../data/json.hpp"
#include "../data/structures.hpp"

// header files
#include "players.hpp"
#include "obstacles.hpp"
#include "map.hpp"

using json = nlohmann::json;
using namespace std;

class Physics {
private:
  PlayerList *players;
  Map *map;
  ObstacleList *obstacles;
  bool running;
  
public:
  Physics(Map *map, ObstacleList *obstacles);
  PlayerList *getPlayers();
  Map *getMap();
  ObstacleList *getObstacles();
  unsigned addPlayer(string name, string team);
  void removePlayer(unsigned id);
  void update(json state);
  void start();
  void stop();
};

#endif
