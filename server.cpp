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
#include <vector>
#include <string>
#include <game_state.h>
#include <cstring>
#include <memory>
#include <room.h>
#include <signal.h>

std::mutex roomsMutex;
std::vector<std::shared_ptr<Room>> rooms;

uint16_t readPort(char *txt);
void handleClient(int fd);
uint16_t recvSize(int fd);
std::string recvMessage(int fd);
const int one = 1;

int main(int argc, char **argv)
{ // prevent dead sockets from throwing pipe errors on write
    signal(SIGPIPE, SIG_IGN);
    // prime sockets waits for new clients
    // create initial room
    // auto DefaultRoom = std::make_shared<Room>();
    // DefaultRoom->name = "Default";
    // rooms.emplace_back(DefaultRoom);
    // std::thread t([room = rooms.back()]()
    //                     { room->roomLoop(); });
    //         t.detach();

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

    // new conections loop
    while (true)
    {

        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);

        cfd = accept(servFd, (sockaddr *)&clientAddr, &clientAddrSize);
        if (cfd == -1)
            error(1, errno, "accept failed");

        // tell who has connected
        printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), cfd);

        std::thread t(handleClient, cfd);
        t.detach();
    }
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
    for (const auto &room : rooms)
    {
        info.append(std::to_string(++i) + ": " + room->name + "\n");
    }
    std::cout << info << std::endl;
    return info;
}

void handleClient(int fd)
{
    // create new room or join client with existing room
    // send gamestate to all clients

    char buffer[256];

    while (true)
    {
        // Read a message

        memset(buffer, 0, sizeof(buffer));
        int count = recv(fd, buffer, recvSize(fd), MSG_WAITALL);

        // Handle disconnection
        if (count <= 0)
        {
            printf("removing %d\n", fd);
            shutdown(fd, SHUT_RDWR);

            close(fd);
            return; // Exit the loop for this client
        }
        else if (strcmp(buffer, "getrooms") == 0)
        {
            printf("getrooms\n");
            uint16_t s = rooms.size();
            printf("num of rooms:%u\n", s);
            uint16_t ss = htons(s);
            send(fd, &ss, sizeof(uint16_t), 0);
            if (s > 0)
            {
                std::string info = roomsInfo();
                const char *message = info.c_str();
                uint16_t size = info.length();
                uint16_t sizeh = htons(size);
                send(fd, &sizeh, sizeof(uint16_t), 0);
                send(fd, message, size, 0);
            }
        }
        else if (strcmp(buffer, "createroom") == 0)
        {
            printf("createroom\n");

            std::unique_lock<std::mutex> lock(roomsMutex);
            auto DefaultRoom = std::make_shared<Room>();
            DefaultRoom->name = recvMessage(fd);
            std::string name = recvMessage(fd);
            DefaultRoom->addClient(fd, name);
            rooms.emplace_back(DefaultRoom);
            auto threadFunction = [roomPtr = rooms.back()]()
            {
                roomPtr->roomLoop();
            };
            std::thread t([room = rooms.back()]()
                          { room->roomLoop(); });
            t.detach();
            return;
        }
        else if (strcmp(buffer, "selectroom") == 0)
        {
            printf("selectroom\n");
            uint16_t s = rooms.size();
            uint16_t choise = recvSize(fd);
            std::string name = recvMessage(fd);
            rooms.at(choise)->addClient(fd, name);
            return;
        }
    }
}
uint16_t recvSize(int fd)
{
    uint16_t size;
    if (recv(fd, &size, sizeof(uint16_t), MSG_WAITALL) < 0)
        perror("read failed");
    size = ntohs(size);
    printf("Otrzymano rozmiar: %u\n", size);
    return size;
}
std::string recvMessage(int fd)
{
    uint16_t size = recvSize(fd);
    char message[size + 1];
    memset(message, 0, size + 1);
    int count = recv(fd, message, size, MSG_WAITALL);
    if (count < 0)
    {
        perror("read failed");
    }
    else if (count == 0)
    {
        printf("Connection closed by server\n");
    }
    else if (count < size)
    {
        printf("not whole message recieved\n");
    }
    printf("%s\n", message);
    return std::string(message);
}