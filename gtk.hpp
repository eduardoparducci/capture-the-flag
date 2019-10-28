#ifndef GTK_HPP
#define GTK_HPP

#include "client.hpp"
#include <map>
#include <unistd.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

class Gtk {
private:
  Client *client;
  Square s;
  
  // Window info
  GLfloat xf;
  GLfloat yf;
  GLfloat win;
  GLint height;
  GLint width;

  // Keyboard info
  std::map<char, bool> pressed_keys;

public:
  void init(int argc, char **argv, Client *c);
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
  void update();
};
#endif

