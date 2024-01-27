#include "client_handler.h"

void Handler::sendMessage(std::string smessage)
{
    const char *message = smessage.c_str();
    uint16_t size = smessage.length();
    printf("rozmiar wiadmości: %d\n", size);
    sendSize(size);
    long unsigned int n = send(prime_sock, message, size, 0);
    if (n < sizeof(message))
        printf("not whole message sent\n");
}
void Handler::sendMessage(char *message, int size)
{
    long unsigned int n = send(prime_sock, message, size, 0);
    if (n < sizeof(message))
        printf("not whole message sent\n");
}

void Handler::recvMessage(char *message, int size)
{
    long unsigned int count = recv(prime_sock, message, size, MSG_WAITALL);
    if (count == 0)
    {
        close(prime_sock);
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
    send(prime_sock, &size, sizeof(uint16_t), 0);
}

int Handler::connectToServer(char *ip, char *port)
{
    // Resolve arguments to IPv4 address with a port number
    addrinfo *resolved, hints = {};
    hints.ai_flags = 0;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int res = getaddrinfo(ip, port, &hints, &resolved);
    if (res || !resolved)
    {
        error(1, 0, "getaddrinfo: %s", gai_strerror(res));
        return -1;
    }
    else
    {
        // create socket
        prime_sock = socket(resolved->ai_family, resolved->ai_socktype, 0);
        if (prime_sock == -1)
            error(1, errno, "socket failed");

        // attempt to connect
        res = connect(prime_sock, resolved->ai_addr, resolved->ai_addrlen);
        if (res < 0)
            error(1, errno, "connect failed");
    }
    // free memory
    freeaddrinfo(resolved);
    // std::thread t(receiver, sock);
    return prime_sock;
}
int Handler::getRoomsInfo()
{
    char buffer[256];
    std::strcpy(buffer, "getrooms");
    sendSize(10);
    sendMessage(buffer, 10);
    number_of_rooms = recvSize();
    printf("number_of_rooms: %u\n", number_of_rooms);
    if (number_of_rooms > 0)
    {
        uint16_t size = recvSize();
        printf("size:%u\n", size);
        char data[size + 1];
        memset(data, 0, size + 1);
        recvMessage(data, size);
        memcpy(roomsStates, &data[size - 2 * number_of_rooms], 2 * number_of_rooms);
        printf("%s\n", data);
        printf("%s\n", roomsStates);
    }
    return number_of_rooms;
}
int Handler::selectRoom()
{
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));

    int t;
    std::string name;
    do
    {
        std::cout << "Select available room by number (0 to create room):";
        std::cin >> t;
    } while (t > number_of_rooms || t < 0);
    if (t == 0)
    {
        std::strcpy(buffer, "createroom");
        sendSize(10);
        sendMessage(buffer, 10);
        do
        {
            std::cout << "Write down room name (3-8 letters):";
            std::cin >> name;
        } while (name.length() > 8 || name.length() < 3);
        sendMessage(name);
    }
    else
    {
        while (roomsStates[2 * (t - 1)] >= '6')
        {
            std::cout << "Room is fulll!!!\nSelect diffrient room." << std::endl;
            std::cout << "Select room by number (0 to create room):";
            std::cin >> t;
        }
        while (roomsStates[2 * (t - 1) + 1] == '1')
        {
            std::cout << "The game already started\nSelect diffrient room." << std::endl;
            std::cout << "Select room by number (0 to create room):";
            std::cin >> t;
            // TODO dodaj do kolejki oczekujących lub do obserwatorów
        }
        std::strcpy(buffer, "selectroom");
        sendSize(10);
        sendMessage(buffer, 10);
        sendSize(t - 1);
    }

    do
    {
        std::cout << "Write down your name (3-8 letters):";
        std::cin >> name;
    } while (name.length() > 8 || name.length() < 3);
    const char *message = name.c_str();
    uint16_t size = name.length();
    printf("rozmiar wiadmości: %d\n", size);
    sendSize(size);
    send(prime_sock, message, name.length(), 0);
    return 0;
}
void Handler::recvGameState(char *message) // coords[14])
{

    int size = 14 * 6 + 4 + 1;
    // char message[size];
    memset(message, 0, size);
    recvMessage(message, size);
    // printf(message);
    // printf("\n");
}

int Handler::sendPlayerState(int n, float x, float y)
{
    size_t bufferSize = 15;
    char message[bufferSize];
    memset(message, 0, bufferSize);
    snprintf(message, bufferSize, "%2.2f %2.2f %d", x, y, n % 10);
    send(prime_sock, message, 15, 0);
    return 0;
}
void Handler::disconnect()
{
    shutdown(prime_sock, SHUT_RDWR);
    close(prime_sock);
}