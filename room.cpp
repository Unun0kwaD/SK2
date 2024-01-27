#include "room.h"

void sendSize(int fd, uint16_t size)
{
    size = htons(size);
    send(fd, &size, sizeof(uint16_t), 0);
}
// void sendMessage(int fd, const char *message, int size)
// {
//     int n = send(fd, message, size, 0);
//     if (n < 0)
//         printf("send message error:");
//     if (n < sizeof(message))
//         printf("not whole message sent\n");
// }
void Room::sendGameState(int fd, char *message, int size)
{
    int n = send(fd, message, size, 0);
    if (n <= 0)
        printf("send message error:");
    if (n < sizeof(message))
        printf("not whole message sent\n");
}

Room::Room()
{
    num_clients = 0;
    waitTime = 30;
    ingame = false;
    memset(clientsFd, 0, sizeof(clientsFd));
    memset(health, 0, sizeof(health));
}
void Room::addClient(int fd, std::string name)
{
    std::unique_lock<std::mutex> lock(clientFdsMutex);
    this->clientsFd[state.addPlayer(name)] = fd;
    num_clients++;
    // if(num_clients==1)
    // set as admin
}
void Room::removeClient(int id)
{
    if (state.isActive(id))
    {
        shutdown(this->clientsFd[id], SHUT_RDWR);
        close(this->clientsFd[id]);
        this->clientsFd[id] = 0;

        state.removePlayer(id);
        num_clients--;
    }
}
void Room::roomLoop()
{
    int h;
    float x, y;
    char names[9 * 6];
    char statemessage[14 * 6 + 1 + 4], playerState[15];
    state.startNewGame();
    while (true)
    {
        // waitng room
        while (waitTime >= 0)
        {
            // check on message queue with mutex if new clients arrived

            // send information abaout time left to all clients

            if (num_clients < 1)
            {
                // for (int i = 0; i < 6; i++)
                // {
                //     if (state.isActive(i))
                //     {
                //         removeClient(i);
                //     }

                //     // TODO: sent information to serwer that its shutdown so that it will be destroyed
                // }
                // // destroyme.append(id_t);
                // return;
                waitTime=30;
            }
            else
            {
                std::unique_lock<std::mutex> lock(clientFdsMutex);
                memset(statemessage, 0, sizeof(statemessage));
                state.createGameStateMessage(statemessage);
                state.getPlayersNames(names);
                for (int i = 0; i < 6; i++)
                {
                    if (state.isActive(i))
                    {
                        int f = clientsFd[i];

                        sendSize(f, num_clients);
                        send(f, statemessage, 89, 0);
                        sendSize(f, waitTime);
                        send(f, names, 54, 0);
                        if (recv(f, playerState, 15, 0) <= 0)
                        {
                            health[i]++;
                            printf("Client not responding\n");
                            if (health[i] > 0)
                                removeClient(i);
                        }
                        else
                            health[i] = 0;
                        std::stringstream iss(playerState);
                        iss >> x >> y >> h;
                        if (h == 5)
                        {
                            // TODO rozłącz i usun gracza z gry
                            removeClient(i);
                        }
                        else if (h == 3 && i == 0)
                        {
                            waitTime = 1;
                        }
                    }
                }
                waitTime--;
            }
            sleep(1);
        }
        ingame = true;

        // ingame
        while (!state.game_over)
        {
            std::unique_lock<std::mutex> lock(clientFdsMutex);
            state.createGameStateMessage(statemessage);
            for (int i = 0; i < 6; i++)
            {
                if (state.isActive(i))
                    // broken pipe exception if clients disconnects
                    send(clientsFd[i], statemessage, 89, 0);
            }
            // recieve && apply forces by using poll; if n== 0 in the player state close the connection and delte player
            // CHANGE TO POLL
            for (int i = 0; i < 6; i++)
            {
                if (state.isActive(i))
                {
                    if (recv(clientsFd[i], playerState, 15, 0) <= 0)
                    {
                        health[i]++;
                        printf("Client not responding\n");
                        if (health[i] > 3)
                            removeClient(i);
                    }
                    else
                        health[i] = 0;
                    // printf("%s\n",playerState);
                    std::stringstream iss(playerState);
                    iss >> x >> y >> h;
                    if (h == 1)
                    {
                        state.updatePlayerPosition(i, x, y);
                    }
                    if (h == 5)
                    {
                        // rozłącz i usun gracza z gry
                        removeClient(i);
                    }
                }
            }
            state.Step();
            if (num_clients <= 0)
            {
                break;
            }
        }
        ingame = false;
        waitTime = 30;
        state.startNewGame();
    }
}

std::string Room::getStateName()
{
    std::string sname;
    if (ingame)
    {
        sname.append("in game ");
    }
    else
    {
        sname.append("in lobby ");
    }
    sname.append(1, '0' + num_clients);
    sname.append("/6");
    return sname;
}