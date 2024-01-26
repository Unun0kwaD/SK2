#include "room.h"

void sendSize(int fd, uint16_t size)
{
    size = htons(size);
    send(fd, &size, sizeof(uint16_t), 0);
}
void sendMessage(int fd, const char *message, int size)
{
    int n = send(fd, message, size, 0);
    if (n < 0)
        printf("send message error:");
    if (n < sizeof(message))
        printf("not whole message sent\n");
}
void Room::sendGameState(int fd, char *message, int size)
{
    int n = send(fd, message, size, 0);
    if (n < 0)
        printf("send message error:");
    if (n < sizeof(message))
        printf("not whole message sent\n");
}

Room::Room()
{
    state.startNewGame();
    num_clients = 0;
    waitTime = 10;
}
void Room::addClient(int fd, std::string name)
{
    std::unique_lock<std::mutex> lock(clientFdsMutex);
    clientsFd.emplace_back(fd);
    state.addPlayer(name);
    num_clients++;
}
void Room::roomLoop()
{
    while (true)
    {
        // waitng room
        if (num_clients == 0)
        {
            sleep(1);
        }
        else
            while (waitTime >= 0)
            {
                // check on message queue with mutex if new clients arrived
                //  printf("room loop: \n");

                // send information abaout time left to all clients
                {
                    std::unique_lock<std::mutex> lock(clientFdsMutex);
                    char message[14 * 6 + 1];
                    state.createGameStateMessage(message);
                    std::cout << message << std::endl;
                    uint16_t size = 14 * 6 + 1;
                    state.getPlayersNames(names);

                    for (int i = 0; i < num_clients; i++)
                    {
                        int f = clientsFd[i];

                        sendSize(f, num_clients);
                        send(f, message, 85, 0);
                        sendSize(f, waitTime);

                        // send players names
                        send(f, names, 54, 0);
                    }
                }
                // remove those who left
                waitTime--;
                sleep(1);
            }
        ingame = true;

        // ingame
        while (!state.game_over)
        {
            // send game state to every client
            {
                // Lock the mutex before accessing the clientFds set
                std::unique_lock<std::mutex> lock(clientFdsMutex);
                // sendToAll(buffer, count);
            }
            // receive players acction using poll
            // if client didnt answer remove
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