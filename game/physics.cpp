/* Desenvolvido por: Eduardo  (170272)
 *                   Henrique (174638)
 */
#include "physics.hpp"

using namespace std;
using json = nlohmann::json;

Physics::Physics(Player *player, Map *map, ObstacleList *obstacles) {
   this->player = player;
   this->map = map;
   this->obstacles = obstacles;
 }

void Physics::update(json state) {

  // Update keys pressed in current state
  this->player->setDirection(state["keys"]);

  // Calculate new position using recently stored keys state
  Square s = this->player->getPosition();
  if(state["keys"]["a"].get<bool>()) {
    s.x_min -= 0.8;
    s.x_max -= 0.8;
  }
  if(state["keys"]["d"].get<bool>()) {
    s.x_min += 0.8;
    s.x_max += 0.8;
  }
  if(state["keys"]["w"].get<bool>()) {
    s.y_min += 0.8;
    s.y_max += 0.8;
  }
  if(state["keys"]["s"].get<bool>()) {
    s.y_min -= 0.8;
    s.y_max -= 0.8;
  }

  // Verify validity and store new position
  if(this->map->isValid(s) && !this->obstacles->hit(s)) {
    this->player->update(s);
  }
}

Player *Physics::getPlayer(){
  return this->player;
}
