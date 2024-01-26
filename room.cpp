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
    waitTime = 30;
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

                // send information abaout time left to all clients
                {

                    if (num_clients < 1)
                    {
                        /*
                        for (int i = 0; i < clientsFd.size(); i++)
                        {
                            shutdown(clientsFd[i], SHUT_RDWR);
                            close(clientsFd[i]);
                            // TODO: sent information to serwer that its shutdown so that it will be destroyed
                            return;
                        }
                        */
                    }
                    else
                    {
                        std::unique_lock<std::mutex> lock(clientFdsMutex);
                        state.createGameStateMessage(statemessage);
                        state.getPlayersNames(names);
                        for (int i = 0; i < num_clients; i++)
                        {
                            int f = clientsFd[i];

                            sendSize(f, num_clients);
                            send(f, statemessage, 85, 0);
                            sendSize(f, waitTime);
                            send(f, names, 54, 0);
                        }
                    }
                }
                waitTime--;
                sleep(1);
            }
        ingame = true;

        // ingame
        while (!state.game_over)
        {
            std::unique_lock<std::mutex> lock(clientFdsMutex);
            state.createGameStateMessage(statemessage);
            for (int i = 0; i < num_clients; i++)
            {
                send(clientsFd[i], statemessage, 85, 0);
            }
            //recieve && apply forces by using poll; if n== 0 in the player state close the connection and delte player
            //CHANGE TO POLL
            for (int i = 0; i < num_clients; i++)
            {
                recv(clientsFd[i], playerState, 14, 0);
                // printf("%s\n",playerState);
                std::stringstream iss(playerState);
                iss>>x>>y>>h;
                if(h){
                    state.updatePlayerPosition(i,x,y);
                }
            }
            state.Step();
        }
    }
}