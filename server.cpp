#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <poll.h>
#include <thread>
#include <unordered_set>
#include <mutex>
#include <vector>
#include <string>
#include "game_state.cpp"
#include <cstring>
#include <memory>

std::mutex clientFdsMutex;
std::unordered_set<int> clientFds;
std::mutex roomsMutex;
std::vector<std::shared_ptr<GameState>>  rooms;


uint16_t readPort(char *txt);
void handleClient(int fd);

const int one = 1;

int main(int argc, char **argv)
{
    // prime sockets waits for new clients
    // create initial room;
    GameState initRoom;
    auto newGameState = std::make_shared<GameState>();
    newGameState->name="Default";
    // Add the new GameState to the vector using emplace_back
    rooms.emplace_back(newGameState);
    auto port = readPort("9999");

    int servFd = socket(PF_INET, SOCK_STREAM, 0);
    if (servFd == -1)
        error(1, errno, "socket failed");

    if (setsockopt(servFd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)))
        error(1, errno, "setsockopt failed");

    sockaddr_in serverAddr{
        .sin_family = AF_INET,
        .sin_port = htons((short)port),
        .sin_addr = {INADDR_ANY}};
    if (bind(servFd, (sockaddr *)&serverAddr, sizeof(serverAddr)))
        error(1, errno, "bind failed");

    if (listen(servFd, 1))
        error(1, errno, "listen failed");
    int cfd;
    // send them rooms info
    while (true)
    {

        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);

        cfd = accept(servFd, (sockaddr *)&clientAddr, &clientAddrSize);
        if (cfd == -1)
            error(1, errno, "accept failed");

        clientFds.insert(cfd);

        // tell who has connected
        printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), cfd);

        std::thread t(handleClient, cfd);
        t.detach();
    }
    // create new room or join client with existing room
    // send gamestate to all clients
}
uint16_t readPort(char *txt)
{
    char *ptr;
    int port = strtol(txt, &ptr, 10);
    if (*ptr != 0 || port < 1 || port > 65535)
        error(1, 0, "illegal argument %s", txt);
    return port;
}
std::string roomsInfo()
{
    std::string info = "";
    int i = 0;
    std::unique_lock<std::mutex> lock(roomsMutex);
    for (const auto& room : rooms)
    {
        info.append(std::to_string(++i)+": "+room->name + "\n");
    }
    std::cout<<info<<std::endl;
    return info;
}

void handleClient(int fd)
{

    char buffer[256];

    while (true)
    {
        // Read a message

        memset(buffer, 0, sizeof(buffer));
        int count = read(fd, buffer, sizeof(buffer));

        // Handle disconnection
        if (count <= 0)
        {
            printf("removing %d\n", fd);
            std::unique_lock<std::mutex> lock(clientFdsMutex);
            clientFds.erase(fd);
            shutdown(fd, SHUT_RDWR);

            close(fd);
            return; // Exit the loop for this client
        }
        else if (strcmp(buffer, "getrooms") == 0)
        {
            printf("getrooms\n");
            uint16_t s =rooms.size();
            printf("num of rooms:%u\n",s);
            s=htons(s);
            write(fd, &s, sizeof(uint16_t));
            std::string info = roomsInfo();
            const  char* message=info.c_str();
            uint16_t size=info.length();
            printf("rozmiar wiadmości: %d\n",size);
            size=htons(size);
            write(fd, &size, sizeof(uint16_t));
            write(fd, message, size);
        }else if (strcmp(buffer, "createroom") == 0)
        {
            printf("createroom\n");
            uint16_t s =rooms.size();
            
            rooms.insert
            
        }
        else if (strcmp(buffer, "selectroom") == 0)
        {
            printf("createroom\n");
            uint16_t s =rooms.size();
            
        }
        // Broadcast the message to all clients
        {
            // Lock the mutex before accessing the clientFds set
            printf("from %d:%s\n", fd, buffer);
            std::unique_lock<std::mutex> lock(clientFdsMutex);
            // sendToAllBut(fd, buffer, count);
        }
    }
}