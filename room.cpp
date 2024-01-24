#include "room.h"

Room::Room(){
    state.startNewGame();
    num_clients=0;
}
void Room::addClient(int fd, std::string name){
    clientsFd.emplace_back(fd);
    state.addPlayer(name);
    waitTime=10;
}
void Room::roomLoop(){
    while(true){
    //waitng room
    while(waitTime>0){
        //check on message queue with mutex if new clients arrived
        //send information abaout time left to all clients
        //remove those who left
        waitTime--;
    }
    //send information to all players that game starts

    //ingame
    while(!state.game_over){
        //send game state to every client
        {
            // Lock the mutex before accessing the clientFds set
            std::unique_lock<std::mutex> lock(clientFdsMutex);
            // sendToAll(buffer, count);
        }
        //receive players acction using poll
        //if client didnt answer remove
        {

            std::unique_lock<std::mutex> lock(clientFdsMutex);
            // clientFds.erase(fd);
            // shutdown(fd, SHUT_RDWR);

            // close(fd);
            return; // Exit the loop for this client
        }
    }
    }
}