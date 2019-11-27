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
  Client *client;
  Gtk *gtk = new Gtk();
  string player_name;
  string team;
  string ip;
  unsigned int gate;

  cout << "Connection config..." << endl;
  cout << "IP:";
  cin >> ip;
  cout << "GATE:";
  cin >> gate;
  client = new Client(gate,ip,5000);
  
  cout << "Player name:";
  cin >> player_name;
  cout << "Team (blue | red):";
  cin >> team;
  if(!client->init(player_name, team)) return 1;
  gtk->init(argc, argv, client);
  client->cclose();
  return 0;
}
