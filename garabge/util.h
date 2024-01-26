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
#include <signal.h>
#include <cstring>
#include <memory>

void handleClient(int fd);
uint16_t recvSize(int fd);
std::string recvMessage(int fd);

uint16_t recvSize(int fd)
{
    uint16_t size;
    if (recv(fd, &size, sizeof(uint16_t), MSG_WAITALL) < 0)
        perror("read failed");
    size = ntohs(size);
    printf("Otrzymano rozmiar: %u\n",size);
    return size;
}
//recieves size first
std::string recvMessage(int fd)
{
    uint16_t size =recvSize(fd);
    char message[size+1];
    memset(message,0,size+1);
    int count = recv(fd, message, size, MSG_WAITALL);
    if (count < 0)
    {
        perror("read failed");
    }
    else if (count == 0)
    {
        close(fd);
        printf("Connection closed by client\n");
    }
    else if (count < size)
    {
        printf("not whole message recieved\n");
    }
    printf("%s\n",message);
    return std::string(message);
}


void sendMessage(int fd,std::string smessage)
{
    const  char* message=smessage.c_str();
    uint16_t size=smessage.length();
    printf("rozmiar wiadmości: %d\n",size);
    sendSize(fd,size);
    int n =send(fd, message, size,0);
    if (n < 0)
        perror("send message error:");
    if (n < sizeof(message))
        printf("not whole message sent\n");
}
void sendMessage(int fd,char *message,int size)
{
    int n = send(fd, message,size,0);
    if (n < 0)
        perror("send message error:");
    if (n < sizeof(message))
        printf("not whole message sent\n");
}

void recvMessage(int fd,char *message, int size)
{
    int count = recv(fd, message, size, MSG_WAITALL);
    if (count < 0)
    {
        perror("read failed");
    }
    else if (count == 0)
    {
        close(fd);
    }
    else if (count < sizeof(message))
    {
        printf("not whole message recieved");
    }
}
uint16_t recvSize(int fd)
{
    uint16_t size;
    if (recv(fd, &size, sizeof(uint16_t), MSG_WAITALL) < 0)
        perror("read failed");
    size = ntohs(size);
    return size;
}
void sendSize(int fd,uint16_t size)
{
    size = htons(size);
    send(fd, &size, sizeof(uint16_t),0);
}
