
#include "game_state.cpp"
#include <vector>
#include <unordered_set>
#include <netinet/in.h>
#include <unistd.h>

#include <mutex>
class Room{
    public:
    Room();
    bool ingame;//for serwer to know if its ingame or new clients can join
    int waitTime;//time left to game to start automaticlly
    int num_clients;
    std::string name;
    GameState state;
    std::vector<int> clientsFd;
    void roomLoop();
    // void gameLoop();
    void addClient(int fd, std::string name);
    private:
    void sendGameState();
    int recievePlayersState();
    std::mutex clientFdsMutex;
    std::unordered_set<int> clientFds;
};