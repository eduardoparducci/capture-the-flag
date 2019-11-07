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
  Player *player;
  Map *map;
  ObstacleList *obstacles;
public:
  Physics(Player *player, Map *map, ObstacleList *obstacles);
  Player *getPlayer();
  void update(char key, bool value);
  void updateJson(json movement);
};

#endif
