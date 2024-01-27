

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Player.h"
#include "Walls.h"
#include "Ball.h"
#include "Gate.h"
#include "client_handler.h"

int main(int argc, char **argv)
{
    if (argc != 3)
        error(1, 0, "Need 2 arguments: ip and port number");
    Handler handler;
    // connect to server and ask for rooms
    handler.connectToServer(argv[1], argv[2]); //"127.0.0.1", "9999");
    handler.getRoomsInfo();
    handler.selectRoom();
    // while(true)std::cout<<handler.recvGameState()<<std::endl;

    // select room or create one by sending proper message
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Football", sf::Style::Close);
    window.setFramerateLimit(30);

    GameState game;
    handler.game.startNewGame();
    float x, y;
    // sf::Clock clock;
    char coords[14 * 6 + 4 + 1];
    char names[54];
    int timeLeftInWaitingRoom = 10, players, decision = 1;
    while (window.isOpen())
    {
        // get the game state and update it
        // send message about keys that are pressed or exiting

        // waiting room
        if (timeLeftInWaitingRoom > 0)
        {
            players = handler.recvSize();
            handler.recvGameState(coords);
            timeLeftInWaitingRoom = handler.recvSize();
            handler.recvMessage(names, 54);
            handler.sendPlayerState(decision, 0.0f, 0.0f);
            if (decision == 5)
            {
                handler.disconnect();

                window.close();
                return 0;
            }
            game.DisplayTime(timeLeftInWaitingRoom);
            bool test = 0;
            while (game.numPlayers < players)
            {
                test = 1;
                game.addPlayer("");
            }
            if (test)
                game.setPlayersNames(names);
            game.updateFromMessage(coords);
        }
        else
        {
            handler.recvGameState(coords);
            handler.sendPlayerState(decision, x, y);
            game.updateFromMessage(coords);
            x = 0.0f;
            y = 0.0f;
            if (window.hasFocus())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                {
                    y = -20.0f;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                {
                    y = 20.0f;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                {
                    x = -20.0f;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                {
                    x = 20.0f;
                }
            }
            if (game.game_over)
            {
                timeLeftInWaitingRoom = 30;
            }
            if (decision == 5)
            {
                handler.disconnect();
                return 0;
            }
        }
        decision = 1;
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                decision = 5;
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Key::Enter)
                    decision = 3;
                else if (event.key.code == sf::Keyboard::Key::Escape)
                    decision = 5;
                break;
            default:
                break;
            }
        }

        window.clear(sf::Color(10, 200, 10));
        game.Draw(window);
        window.display();
    }
    return 0;
}