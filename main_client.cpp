#include "libraries.hpp"

#include "game/map.hpp"
#include "game/obstacles.hpp"
#include "game/players.hpp"
#include "network/client.hpp"
#include "data/json.hpp"
#include "data/structures.hpp"
#include "gtk/gtk.hpp"

using namespace std;
int main(int argc, char **argv) {
  Client *client = new Client(3001, "127.0.0.1", 2000);
  Gtk *gtk = new Gtk();
  string player_name;
  cout << "Nome do jogador:";
  cin >> player_name;
  if(!client->init(player_name)) return 1;
  gtk->init(argc, argv, client);
  client->cclose();
  return 0;
}
