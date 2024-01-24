
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

class Handler{
    public:
    int prime_sock;
    int connectToServer(char* ip, char* port);
    int getRoomsInfo();
    int selectRoom();
    int createRoom();
    int joinRoom();
    int recvGameState();
    int sendPlayerState();
};