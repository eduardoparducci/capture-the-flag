#include "gtk.hpp"
#include "game.hpp"
#include "client.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <map>

// Needs to be static because glut library has no access to internal GTK members
static Gtk *gtk;

/*===== CALLBACKS =====*/
// Need to be global function because because glut has no access to local methods

void keyboardPressDownCallback(unsigned char key, int x, int y) {
  gtk->updateKeys(key, true);
}

void keyboardPressUpCallback(unsigned char key, int x, int y) {
  gtk->updateKeys(key, false);
}

void frameCallback() {
  gtk->frameHandler();
}

void timerCallback(int i) {
  gtk->timeHandler();
}

// Função callback chamada quando o tamanho da janela é alterado 
void resizeWindowCallback(GLsizei w, GLsizei h) {
  gtk->resizeWindowHandler(w,h);
}

/*===== GTK methods =====*/

// Resize window item according to window size
void Gtk::resizeWindowHandler(GLsizei w, GLsizei h) {
    // Especifica as dimensões da Viewport
  this->width = w;
  this->height = h;
  glViewport(0, 0, this->width, this->height);
  // Inicializa o sistema de coordenadas
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D (-this->win, this->win, -this->win, this->win);
}

// Update frame
void Gtk::frameHandler() {
  glClear(GL_COLOR_BUFFER_BIT);
  this->drawPlayer();
  this->drawObstacles();
  this->drawInfo();
  this->drawMap();
  // Update frame
  glutSwapBuffers();
}

// Draw grid on the screen
void Gtk::gridOn() {
  glColor3f(0.5f,0.5f,0.5f);
  glLineWidth(1.0f);
  glBegin(GL_LINES);
  float x = -this->win;
  for(;x<=this->win; x+=10) {
    glVertex2f(-this->win,x);
    glVertex2f( this->win,x);
    glVertex2f(x,-this->win);
    glVertex2f(x, this->win);
  }
  glEnd();
  glColor3f(0.0f,0.0f,0.0f);
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex2f(-this->win,0);
  glVertex2f( this->win,0);
  glVertex2f(0,-this->win);
  glVertex2f(0, this->win);
  glEnd();

}

// Draw player
void Gtk::drawPlayer() {
  RGB color = this->player->getColor();
  Square s = this->player->getPosition();

  // Cofigure player appearence
  glColor3f(color.r, color.g, color.b); // Color

  // Draw dots on the screen
  glBegin(GL_QUADS);
  glVertex2i(s.x_min,s.y_min);
  glVertex2i(s.x_min,s.y_max);
  glVertex2i(s.x_max,s.y_max);
  glVertex2i(s.x_max,s.y_min);
  glEnd();

  // Player name
  glRasterPos2i(s.x_min, s.y_max+2);
  glColor3f(0.0f, 0.0f, 0.0f);
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)(this->player->getName().c_str()));
}

// Draw player
void Gtk::drawObstacles() {

  std::vector<Obstacle *> *o = this->obstacles->getObstacles();
  Square s;

  // Cofigure player appearence
  glColor3f(0.0f, 0.0f, 0.0f); // Black
  for (int i = 0 ; i < (int)(*o).size() ; i++) {
    s = (*o)[i]->getPosition();
    glBegin(GL_QUADS);
    glVertex2i(s.x_min,s.y_min);
    glVertex2i(s.x_min,s.y_max);
    glVertex2i(s.x_max,s.y_max);
    glVertex2i(s.x_max,s.y_min);
    glEnd();
  }
}

// Draw map
void Gtk::drawMap() {
  Square s = this->map->getBoundaries();
  RGB color = this->player->getColor();
  
  glLineWidth(4.0f); //width of line
  glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2i(s.x_min,s.y_min);
	glVertex2i(s.x_min,s.y_max);
	glVertex2i(s.x_max,s.y_max);
	glVertex2i(s.x_max,s.y_min);
  glEnd();

  // Basis
  glColor4f(color.r, color.g, color.b, 0.25f); // Color
  s = this->map->getBasis();
  glBegin(GL_QUADS);
  glVertex2i(s.x_min,s.y_min);
  glVertex2i(s.x_min,s.y_max);
  glVertex2i(s.x_max,s.y_max);
  glVertex2i(s.x_max,s.y_min);
  glEnd();


}

// Draw info at the bottom of the screen
void Gtk::drawInfo() {
  glRasterPos2i(-this->win+3, -this->win+3);
  glColor3f(0.0f, 0.0f, 0.0f);
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"Player: ");
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)this->player->getName().c_str());
}

// Update position
void Gtk::timeHandler() {
  Square s = this->player->getPosition();
 
  if(gtk->pressed_keys['a']) {
    s.x_min -= 0.8;
    s.x_max -= 0.8;
  }
  if(gtk->pressed_keys['d']) {
    s.x_min += 0.8;
    s.x_max += 0.8;
  }
  if(gtk->pressed_keys['w']){
    s.y_min += 0.8;
    s.y_max += 0.8;
  }
  if(gtk->pressed_keys['s']) {
    s.y_min -= 0.8;
    s.y_max -= 0.8;
  }
  if(this->map->isValid(s) && !this->obstacles->hit(s)) {
    this->player->update(s);
  }
  glutPostRedisplay();
  glutTimerFunc(20, timerCallback, 1);
}

// Initial config
void Gtk::init(int argc, char **argv, Player *player, Map *map, ObstacleList *o, Client *c) {
  gtk = this;
  this->xf = 3;
  this->yf = 3;
  this->win = 150.0f;
  this->height = 500;
  this->width = 700;
  this->player = player;
  this->map = map;
  this->obstacles = o;
  this->client = c;
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(this->width,this->height);
  glutInitWindowPosition(0,00);
  glutCreateWindow("Capture the flag");
  glutDisplayFunc(frameCallback);
  glutReshapeFunc(resizeWindowCallback);
  glutIgnoreKeyRepeat(1);
  glutKeyboardFunc(keyboardPressDownCallback);
  glutKeyboardUpFunc(keyboardPressUpCallback);
  glutTimerFunc(33, timerCallback, 1);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glMatrixMode(GL_PROJECTION);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLoadIdentity();
  gluOrtho2D(-this->win,this->win,-this->win,this->win);
  glMatrixMode(GL_MODELVIEW);
  glutMainLoop();
}

// Update keys
void Gtk::updateKeys(char key, bool is_pressed) {
  this->pressed_keys[key] = is_pressed;

  std::string data(1,key);
  if(is_pressed) data.append("+");
  else           data.append("-");
  this->client->send_string(data);
}
