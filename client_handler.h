#include <netinet/in.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <game_state.h>
#include <sstream>

class Handler{
    public:
    GameState game;
    int prime_sock;
    int connectToServer(char* ip, char* port);
    int getRoomsInfo();
    int selectRoom();
    void recvGameState(char *message);
    int sendPlayerState(int n,float x, float y);
    uint16_t recvSize();
    void sendMessage(char* message,int size);
    void recvMessage(char* message,int size);
    void disconnect();
    private:
    char roomsStates[100];
    uint16_t number_of_rooms;
    void sendMessage(std::string smessage);
    void recvCoordinates(float* coordinates,int size);
    void sendSize(uint16_t size);
};