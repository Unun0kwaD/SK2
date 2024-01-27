
#include <vector>
#include <unordered_set>
#include <netinet/in.h>
#include <unistd.h>
#include <game_state.h>
#include <iostream>
#include <sstream>

#include <mutex>
class Room
{
public:
    Room();
    int id=0;
    bool ingame=false;  // for serwer to know if its ingame or new clients can join
    int waitTime; // time left to game to start automaticlly
    int num_clients;
    std::string name;
    GameState state;
    void roomLoop();
    // void gameLoop();
    void addClient(int fd, std::string name);
    void removeClient( int id);
    std::string getStateName();

private:
    
    void sendGameState(int fd, char *message, int size);
    int recievePlayersState();
    std::mutex clientFdsMutex;
    int clientsFd[6];
    int health[6];
};