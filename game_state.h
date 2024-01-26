#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Player.h"
#include "Walls.h"
#include "Ball.h"
#include "Gate.h"

class GameState
{
private:
    std::vector<Player> players;
    std::vector<Player> observers;
    b2World world = b2World({0.0f, 0.0f});
    Ball ball = Ball(world, sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), BALL_RADIUS);
    Gate gate1;
    Gate gate2;
    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 6;
    Walls walls = Walls(world, WINDOW_WIDTH, WINDOW_HEIGHT);
    int score_left = 0;
    int score_right = 0;
    sf::Font m_font;
    sf::Text timer;
    int timeleft=0;
public:
    int numPlayers;
    bool game_over = false;

    GameState();

    void startNewGame();

    void addPlayer(std::string name);

    void addPlayer(float initialX, float initialY, sf::Color color, std::string name, int id);

    void updatePlayerPosition(int playerId, float newX, float newY);

    void Step();

    void removePlayer(int player_id);

    void createGameStateMessage(char *message);

    void updateFromMessage(float coordinates[14]);

    void setPlayersNames(char *names);

    void getPlayersNames(char *names);

    void Draw(sf::RenderWindow &window);
    void DisplayTime(int time);
};

#endif 
