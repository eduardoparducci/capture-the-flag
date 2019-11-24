/* Desenvolvido por: Eduardo  (170272)
 *                   Henrique (174638)
 */
#include "physics.hpp"

Physics::Physics(Map *map, ObstacleList *obstacles) {
   this->map = map;
   this->obstacles = obstacles;
   this->players = new PlayerList();
   this->running = false;
 }

void Physics::update(json state) {
  
  vector<Player *> *pl = this->players->getPlayers();
  Player *p = this->players->getPlayer(state["id"].get<unsigned>());

  // Verify if game is running
  if(!this->running) return;

  // Update keys pressed of the desired player
  p->setDirection(state["keys"]);  

  // Iterate every player
  for (int i = 0 ; i < (int)(*pl).size() ; i++) {

    p = (*pl)[i];
    
    // Update position
    Square s = p->getPosition();
    ::map<char, bool> d = p->getDirection();
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

    // Verify validity and store new position
    if(this->map->isValid(s) && !this->obstacles->hit(s)) {
      p->update(s);
    }
  }
}

void Physics::start() {
  this->running = true;
}

void Physics::stop() {
  this->running = false;
}

PlayerList *Physics::getPlayers(){
  return this->players;
}

Map *Physics::getMap(){
  return this->map;
}

ObstacleList *Physics::getObstacles(){
  return this->obstacles;
}

unsigned Physics::addPlayer(string name) {
  // Create new player with unique id and name defined by client
  Player *player = new Player(0.0f, 0.0f, 7.0f, 5.0f, name, {1.0f, 0.0f, 0.0f}, this->players->getPlayers()->size());
  this->players->addPlayer(player);
  cout << "Physics: New player:" << endl;
  cout << this->players->serialize().dump(4) << endl;
  return player->getId();
}

void Physics::removePlayer(unsigned id) {
  cout << "Physics: removing player id(" << id << ")" << endl; 
  this->players->removePlayer(id);
}
