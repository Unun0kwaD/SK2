
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
    bool ingame;  // for serwer to know if its ingame or new clients can join
    int waitTime; // time left to game to start automaticlly
    int num_clients;
    std::string name;
    GameState state;
    void roomLoop();
    // void gameLoop();
    void addClient(int fd, std::string name);

private:
    int h;
    float x, y;

    char statemessage[14 * 6 + 1];
    char names[9 * 6];
    char playerState[14];
    void sendGameState(int fd, char *message, int size);
    int recievePlayersState();
    std::mutex clientFdsMutex;
    std::vector<int> clientsFd;
};