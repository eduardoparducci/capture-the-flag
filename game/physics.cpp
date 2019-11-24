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
      s.x_min -= 3;
      s.x_max -= 3;
    }
    if(d['d']) {
      s.x_min += 3;
      s.x_max += 3;
    }
    if(d['w']) {
      s.y_min += 3;
      s.y_max += 3;
    }
    if(d['s']) {
      s.y_min -= 3;
      s.y_max -= 3;
    }

    // Verify validity and store new position
    if(this->map->isValid(s) && !this->obstacles->hit(s)) {
      if(p->getColor().b!=0 && this->map->isRedBasis(s)) {
        this->map->setBlueScore();
        s = {-87.6,3.5,-92.5,-3.5};
      }
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

unsigned Physics::addPlayer(string name, string team) {
  // Create new player with unique id, with name and team defined by client
  RGB color;
  float xi;
  
  if(team.compare("blue")==0) {
    color = {0,0,1};
    xi = -90.0;
  } else {
    color = {1,0,0};
    xi = 90.0;
  }
  Player *player = new Player(xi, 0.0f, 7.0f, 5.0f, name, color, this->players->getPlayers()->size());
  this->players->addPlayer(player);
  cout << "Physics: New player:" << endl;
  cout << this->players->serialize().dump(4) << endl;
  return player->getId();
}

void Physics::removePlayer(unsigned id) {
  cout << "Physics: removing player id(" << id << ")" << endl; 
  this->players->removePlayer(id);
}
