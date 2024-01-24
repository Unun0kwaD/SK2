#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Player.h"
#include "Walls.h"
#include "Ball.h"
#include "Gate.h"

class GameState
{
private:
    int numPlayers;
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

public:
    bool game_over = false;
    GameState()
    {
        if (!m_font.loadFromFile("resources/arial.ttf"))
        {
            std::cerr << "Failed to load font" << std::endl;
        }
        gate1 = Gate(false, m_font);
        gate2 = Gate(true, m_font);
        numPlayers = 0;
    }
    void startNewGame()
    {
        int score_left = 0;
        int score_right = 0;
        bool game_over = false;
        for (auto &player : players)
        {
            player.moveToInitPosition();
            ball.reset();
        }
    }
    void addPlayer(std::string name)
    {
        sf::Color color;
        float initialX;
        float initialY;
        if (numPlayers % 2)
        {
            color = sf::Color::Red;
            initialX = WINDOW_WIDTH / 3.0f;
        }
        else
        {
            color = sf::Color::Blue;
            initialX = 2 * WINDOW_WIDTH / 3.0f;
        }
        if (numPlayers < 2)
        {
            initialY = WINDOW_HEIGHT / 2.0f;
        }
        else if (numPlayers < 4)
        {
            initialY = 3 * WINDOW_HEIGHT / 4.0f;
        }
        else
            initialY = WINDOW_HEIGHT / 4.0f;
        numPlayers++;
        players.emplace_back(Player(world, sf::Vector2f(initialX, initialY), color, name, numPlayers - 1, m_font));
    }
    void addPlayer(float initialX, float initialY, sf::Color color, std::string name, int id)
    {
        numPlayers++;
        players.emplace_back(Player(world, sf::Vector2f(initialX, initialY), sf::Color::Blue, name, id, m_font));
    }

    void updatePlayerPosition(int playerId, float newX, float newY)
    {
        players.at(playerId).ApplyForce(newX, newY);
    }
    void Step()
    {
        world.Step(timeStep, velocityIterations, positionIterations);

        // Check for collisions
        if (gate1.CheckCollision(ball))
            score_right = gate1.get_score();
        if (gate2.CheckCollision(ball))
            score_left = gate2.get_score();
        if (score_left == 5 || score_right == 5)
        {
            game_over = true;
            startNewGame();
        }
    }
    void removePlayer(int player_id)
    {
        auto iterToRemove = players.begin() + player_id;
        iterToRemove->RemoveBody();
        players.erase(iterToRemove);
        for (auto it = iterToRemove; it != players.end(); ++it)
        {
            it->id = it->id - 1;
        }
        numPlayers--;
    }

    float* createGameStateMessage()
    {
        float* coordinates = new float[14];
        memset(coordinates, 0, 14);
        coordinates[0] =static_cast<float>( ball.get_x());
        coordinates[1] = static_cast<float>(ball.get_y());
        for (int i = 0; i < players.size(); i++)
        {
            coordinates[2 + i * 2] = static_cast<float>(players[i].get_x());
            coordinates[3 + i * 2] = static_cast<float>(players[i].get_y());
        }
        return coordinates;
    }

    void updateFromMessage(float coordinates[14])
    {
        ball.setPosition(coordinates[0],coordinates[1]);
        int i;
        for (int i = 0; i < players.size(); i++)
        {
            players[i].moveToPosition(coordinates[2 + i * 2],coordinates[3 + i * 2]);
        }
        // while(i<14 &&coordinates[2 + i * 2]>0 && coordinates[2 + i * 2]>0 ){
        //     addPlayer("noname");
        //     players[i].moveToPosition(coordinates[2 + i * 2],coordinates[3 + i * 2]);
        // }

    }

    void Draw(sf::RenderWindow &window)
    {
        gate1.Draw(window);
        gate2.Draw(window);
        ball.Draw(window);
        for (int i = 0; i < players.size(); i++)
        {
            players.at(i).Draw(window);
        }
        walls.Draw(window);
    }
};