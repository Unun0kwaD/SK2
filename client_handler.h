
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


class Handler{
    public:
    int prime_sock;
    int connectToServer(char* ip, char* port);
    int getRoomsInfo();
    int selectRoom();
    int recvGameState();
    int sendPlayerState();
    private:
    uint16_t number_of_rooms;
    void sendMessage(char* message);
    void recvMessage(char* message,int size);
    uint16_t recvSize();
    void sendSize(uint16_t size);
};