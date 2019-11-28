#include "client.hpp"

void wait_package(char **buffer, int buffer_size, bool *buffer_status, bool *running, int socket_fd) {
  int bytes_recv;
  cout << "Client: (pkg thread) new thread initialized." << endl;
 while(*running) {
    if(*buffer_status==FREE) {
      *buffer[0]= '\0';
      bytes_recv = recv(socket_fd, *buffer, buffer_size, 0);
      if(bytes_recv < 0) {
        *buffer[0]= '\0';
        *buffer_status=FREE;
      } else {
        *buffer_status = BUSY;
      }
    }
    std::this_thread::sleep_for (chrono::milliseconds(40));
  }
}

json Client::getPackage() {
  json pkg;
  
  if(this->buffer_status == BUSY) {
    
    string buffer_copy(this->buffer);
    this->buffer_status = FREE;

    // Verify package integrity    
    try {
      pkg = json::parse(buffer_copy);
      //cout << "Client: got package" << endl;
      //cout << pkg.dump(4) << endl;
    } catch(json::parse_error &e) {
      cout << "Client: ERROR parsing json. returning" << endl;
      pkg = {};
    }
  }
  return pkg;
}

bool Client::sendPackage(string data) {
  if(send(this->socket_fd, data.c_str(), data.length()+1 , 0) < 0) {
    return false;
  }
  //cout << "Client: SUCCESS sending " << data << endl;
  return true;
}

Client::Client(unsigned int gate, string ip, int buffer_size) {
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  // Buffer setup
  this->buffer_size = buffer_size;
  this->buffer_status = FREE;
  this->buffer = (char *)malloc(buffer_size * sizeof(char));
  cout << "Client: buffer created with " << buffer_size << "bytes" << endl;  

  // Client settings
  this->ip = ip;
  this->target.sin_family = AF_INET;
  this->target.sin_port = htons(gate);
  this->gate = gate;
  inet_aton(ip.c_str(), &(this->target.sin_addr));

  cout << "Client: socket created " << endl;
}

bool Client::init(string player_name, string team) {
  
  json client_info;
  json server_response;
  unsigned timeout = 0;

  // Creating connection to server and launch thread for fetching packages
  cout << "Client: trying to connect on gate:" << this->gate << " ip:" << this->ip << endl;
  if (connect(this->socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    cout << "Client: ERROR stablishing connection, exiting..." << endl;
    return false;
  }
  cout << "Client: connection stablished!" << endl;
  this->running = true;
  thread newthread(wait_package, &(this->buffer), this->buffer_size, &(this->buffer_status), &(this->running), this->socket_fd);
  (this->pkg_thread).swap(newthread);
  
  // Creating package and sending to server
  cout << "Client: adding player: " <<  player_name << endl;
  client_info["name"] = player_name;
  client_info["team"] = team;
  client_info["init"] = true;
  sendPackage(client_info.dump());
  
  // Waiting for server response with timeout (aprox. 10s)
  do {
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    server_response = getPackage();
    timeout++;
  } while(server_response["init"].empty() && timeout<100);

  // Verify if timeout occured without a response
  if(server_response["init"].empty()) {
    cout << "Client: ERROR server timed out." << endl;
    return false;
  } else {
    cout << "Client: player added successfully." << endl;
    this->id = server_response["id"].get<unsigned>();
    this->players = new PlayerList(server_response["players"]);
    this->myself = this->players->getPlayer(this->id);
    this->map = new Map(server_response["map"]);
    this->obstacles = new ObstacleList(server_response["obstacles"]);
  }

  // Log game status
  cout << "Obstacles: " << endl << this->obstacles->serialize().dump(4) << endl;
  cout << "Map: " << endl << this->map->serialize().dump(4) << endl;
  cout << "Players: " << endl << this->players->serialize().dump(4) << endl;
  cout << "I am: " << endl;
  this->myself->toString();
  
  return true;
}

Player *Client::getMyself() {
  return this->myself;
}

void Client::cclose() {
  json data;
  cout << "Client: terminating connection" << endl;
  data["close"] = true;
  data["id"] = this->id;
  sendPackage(data);
  this->running = false;
  (this->pkg_thread).join();
  free(this->buffer);
  close(socket_fd);
}

PlayerList *Client::getPlayerList() {
  return this->players;
}

unsigned Client::getId() {
  return this->id;
}

ObstacleList *Client::getObstacleList() {
  return this->obstacles;
}

Map *Client::getMap() {
  return this->map;
}

bool Client::getBufferStatus() {
  bool status;
  status = this->buffer_status;
  return status;
}
