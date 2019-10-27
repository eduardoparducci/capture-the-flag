#ifndef GTK_HPP
#define GTK_HPP

#include "game.hpp"
#include "client.hpp"
#include <map>
#include <unistd.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

class Gtk {
private:
  Map *map;
  Client *client;
  //Physics *physics;
  ObstacleList *obstacles;
  Player *player;
  GLfloat xf;
  GLfloat yf;
  GLfloat win;
  GLint height;
  GLint width;
  std::map<char, bool> pressed_keys;

public:
  void init(int argc, char **argv, Player *player, Map *map, ObstacleList *o, Client *c);
  void frameHandler();
  void timeHandler();
  void resizeWindowHandler(GLsizei w, GLsizei h);
  GLfloat getX();
  GLfloat getY();
  void setX(GLfloat x);
  void setY(GLfloat y);
  void updateKeys(char key, bool is_pressed);
  void drawPlayer();
  void drawObstacles();
  void drawMap();
  void drawInfo();
  void gridOn();
};
#endif

