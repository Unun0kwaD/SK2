#include "client_handler.h"

int Handler::connectToServer(char *ip, char *port)
{
    // Resolve arguments to IPv4 address with a port number
    addrinfo *resolved, hints = {.ai_flags = 0, .ai_family = AF_INET, .ai_socktype = SOCK_STREAM};
    int res = getaddrinfo(ip, port, &hints, &resolved);
    if (res || !resolved)
        error(1, 0, "getaddrinfo: %s", gai_strerror(res));

    // create socket
    prime_sock = socket(resolved->ai_family, resolved->ai_socktype, 0);
    if (prime_sock == -1)
        error(1, errno, "socket failed");

    // attempt to connect
    res = connect(prime_sock, resolved->ai_addr, resolved->ai_addrlen);
    if (res < 0)
        error(1, errno, "connect failed");

    // free memory
    freeaddrinfo(resolved);
    // std::thread t(receiver, sock);
    return prime_sock;
}
int Handler::getRoomsInfo()
{
    char buffer[256];
    std::strcpy(buffer, "getrooms");
    sendMessage(buffer);
    number_of_rooms = recvSize();
    uint16_t size = recvSize();
    printf("number_of_rooms:%u\n", number_of_rooms);
    printf("size:%u\n", size);
    char data[size + 1];
    recvMessage(data, size + 1);
    printf("%s\n", data);
    return size;
}
void Handler::sendMessage(char *message)
{
    int n = write(prime_sock, message, sizeof(message));
    if (n < 0)
        perror("send message error:");
    if (n < sizeof(message))
        printf("not whole message sent\n");
}
void Handler::recvMessage(char *message, int size)
{
    int count = recv(prime_sock, message, size, MSG_WAITALL);
    if (count < 0)
    {
        perror("read failed");
    }
    else if (count == 0)
    {
        printf("Connection closed by server\n");
    }
    else if (count < sizeof(message))
    {
        printf("not whole message recieved");
    }
}
uint16_t Handler::recvSize()
{
    uint16_t size;
    if (recv(prime_sock, &size, sizeof(uint16_t), MSG_WAITALL) < 0)
        perror("read failed");
    size = ntohs(size);
    return size;
}
void Handler::sendSize(uint16_t size)
{
    size = htons(size);
    write(prime_sock, &size, sizeof(uint16_t));
}
int Handler::selectRoom()
{
    char buffer[256];
    int t = -1;
    while (t >= number_of_rooms || t < 0)
    {
        std::cout << "Select room by number (0 to create room):";
        std::cin >> t;
    }
    if (t == 0)
    {
        std::strcpy(buffer, "createroom");
        sendMessage(buffer);
    }
    else
    {
        std::strcpy(buffer, "selectroom");
        sendMessage(buffer);
        sendSize(t);
    }

    char team = 'a';
    while (team != 'r' && team != 'b')
    {
        std::cout << "Select team (r/b):";
        std::cin >> team;
    }
    t=(team=='r')*1+(team=='b')*2;
    sendSize(t);
}
