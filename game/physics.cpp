/* Desenvolvido por: Eduardo  (170272)
 *                   Henrique (174638)
 */
#include "physics.hpp"

Physics::Physics(Player *player, Map *map, ObstacleList *obstacles) {
   this->player = player;
   this->map = map;
   this->obstacles = obstacles;
 }

void Physics::update(char key, bool value) {
  this->player->setDirection(key, value);
  Square s = this->player->getPosition();
  ::map<char, bool> d = this->player->getDirection();
  if(d['a']) {
    s.x_min -= 0.8;
    s.x_max -= 0.8;
  }
  if(d['d']) {
    s.x_min += 0.8;
    s.x_max += 0.8;
  }
  if(d['w']) {
    s.y_min += 0.8;
    s.y_max += 0.8;
  }
  if(d['s']) {
    s.y_min -= 0.8;
    s.y_max -= 0.8;
  }
  if(this->map->isValid(s) && !this->obstacles->hit(s)) {
    // cout << "Player position: (" << s.y_max << "," << s.x_max << ")" << endl; 
    this->player->update(s);
  } // else cout << "Player blocked at: (" << s.y_max << "," << s.x_max << ")" << endl;
}

void Physics::updateJson(json movement) {
  this->player->setDirectionJson(movement);
  Square s = this->player->getPosition();
  nlohmann::json d = this->player->getDirectionJson();
  //map<char, bool> d = this->player->getDirection();
  if(d.value("a",false)) {
    s.x_min -= 0.8;
    s.x_max -= 0.8;
  }
  if(d.value("d",false)) {
    s.x_min += 0.8;
    s.x_max += 0.8;
  }
  if(d.value("w",false)) {
    s.y_min += 0.8;
    s.y_max += 0.8;
  }
  if(d.value("s",false)) {
    s.y_min -= 0.8;
    s.y_max -= 0.8;
  }
  if(this->map->isValid(s) && !this->obstacles->hit(s)) {
    // cout << "Player position: (" << s.y_max << "," << s.x_max << ")" << endl; 
    this->player->update(s);
  } // else cout << "Player blocked at: (" << s.y_max << "," << s.x_max << ")" << endl;
}

Player *Physics::getPlayer(){
  return this->player;
}
