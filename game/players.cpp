#include "players.hpp"

using namespace std;
using json = nlohmann::json;

Player::~Player() {
}

Player::Player(float x, float y, float height, float width, string name, RGB color, unsigned id) {
  this->name = name;
  this->id = id;
  this->color = color;
  this->direction['a'] = false;
  this->direction['w'] = false;
  this->direction['s'] = false;
  this->direction['d'] = false;

  //Calculate coordinates based on the color
  this->position.y_max = y+height/2;
  this->position.y_min = y-height/2;
  this->position.x_max = x+width/2;
  this->position.x_min = x-width/2;
}

Player::Player(json player) {
  this->name = player["name"].get<string>();
  this->id = player["id"].get<unsigned>();
  this->direction['a'] = false;
  this->direction['w'] = false;
  this->direction['s'] = false;
  this->direction['d'] = false;
  this->position = {
                    player["position"]["x_max"].get<float>(),
                    player["position"]["y_max"].get<float>(),
                    player["position"]["x_min"].get<float>(),
                    player["position"]["y_min"].get<float>()
  };
  this->color = {
                 player["color"]["r"].get<float>(),
                 player["color"]["g"].get<float>(),
                 player["color"]["b"].get<float>()
  };
}

void Player::setId(unsigned id) {
  this->id = id;
}

void Player::resize(Square p) {
  this->position = p;
}

void Player::update(Square p) {
  this->position.x_max = p.x_max;
  this->position.y_max = p.y_max;
  this->position.x_min = p.x_min;
  this->position.y_min = p.y_min;
}

void Player::setDirection(json keys) {
  this->direction['a'] = keys["a"].get<bool>();
  this->direction['w'] = keys["w"].get<bool>();
  this->direction['s'] = keys["s"].get<bool>();
  this->direction['d'] = keys["d"].get<bool>();
}

void Player::toString() {
  cout << "Player: " << this->name << endl;
  cout << "  Color: RGB(" << this->color.r << "," << this->color.g << "," << this->color.b << ")" << endl;
  cout << "  Position: XYxy(" << this->position.x_max << \
                               "," << this->position.y_max << \
                               "," << this->position.x_min << \
                               "," << this->position.y_min << \
                               ")" << endl;
}

string Player::getName() {
  return this->name;
}

json Player::serialize() {
  json player;
  player["id"] = this->id;
  player["name"] = this->name;
  player["position"]["x_max"] = this->position.x_max;
  player["position"]["x_min"] = this->position.x_min;
  player["position"]["y_max"] = this->position.y_max;
  player["position"]["y_min"] = this->position.y_min;
  player["color"]["r"] = this->color.r;
  player["color"]["g"] = this->color.g;
  player["color"]["b"] = this->color.b;

  return player;
}

unsigned Player::getId() {
  return this->id;
}

map<char,bool> Player::getDirection() {
  return this->direction;
}

Square Player::getPosition() {
  return this->position;
}

RGB Player::getColor() {
  return this->color;
}



PlayerList::PlayerList() {
  this->players = new vector<Player *>(0);
}

PlayerList::PlayerList(json players) {
  int i;
  Player *p;
  this->players = new vector<Player *>(0);
  for (i=0 ; i<(int)players.size() ; i++) {
    p = new Player(players[i]);
    addPlayer(p);
  }
}

json PlayerList::serialize() {
  json players;
  vector<Player *> *o = this->players;
  for (int i = 0 ; i < (int)(*o).size() ; i++) {
    players.push_back((*o)[i]->serialize());
  }
  return players;
}

unsigned PlayerList::addPlayer(Player *o) {
  (this->players)->push_back(o);
  return o->getId();
}

vector<Player*> *PlayerList::getPlayers(){
  return this->players;
}

Player *PlayerList::getPlayer(unsigned id) {
  vector<Player *> *p = this->players;
  int i;
  
  for (i=0 ; i<(int)(*p).size() ; i++) {
    if((*p)[i]->getId()==id) {
      return (*p)[i];
    }
  }
  return nullptr;
}

void PlayerList::update(json players) {
  int i;
  Square s;
  Player *player;
  unsigned id;

  for (i=0 ; i<(int)players.size() ; i++) {
    // Parse data from JSON
    id = players[i]["id"].get<unsigned>();
    s = {
             players[i]["position"]["x_max"].get<float>(),
             players[i]["position"]["y_max"].get<float>(),
             players[i]["position"]["x_min"].get<float>(),
             players[i]["position"]["y_min"].get<float>()
    };

    // Get desired player
    player = getPlayer(id);

    // If player does not exist add it to the game
    if(!player) {
      player = new Player(players[i]);
      addPlayer(player);
    }
    player->update(s);
  }
}

void PlayerList::removePlayer(unsigned id) {
  json players;
  vector<Player *> *o = this->players;
  vector<Player> e;
  for (int i = 0 ; i < (int)(*o).size() ; i++) {
    if((*o)[i]->getId()==id) {
      delete((*o)[i]);
      while(i < (int)(*o).size()-1) {
        (*o)[i] = (*o)[i+1];
      }
      return;
    }
  }
}
