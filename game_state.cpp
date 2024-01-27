
#include "game_state.h"

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

GameState::GameState()
{
    if (!m_font.loadFromFile("resources/arial.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }
    gate1 = Gate(false, m_font);
    gate2 = Gate(true, m_font);
    numPlayers = 0;
    timer.setFont(m_font);
    timer.setCharacterSize(40);
    timer.setFillColor(sf::Color::White);
    timer.setOrigin(timer.getLocalBounds().width / 2, timer.getLocalBounds().height / 2);

    timer.setPosition(20, 20);
}
void GameState::startNewGame()
{
    int score_left = 0;
    int score_right = 0;
    gate1.reset();
    gate2.reset();
    bool game_over = false;
    for (auto &player : players)
    {
        player.moveToInitPosition();
        ball.reset();
    }
}
void GameState::addPlayer(std::string name)
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
void GameState::addPlayer(float initialX, float initialY, sf::Color color, std::string name, int id)
{
    numPlayers++;
    players.emplace_back(Player(world, sf::Vector2f(initialX, initialY), sf::Color::Blue, name, id, m_font));
}

void GameState::updatePlayerPosition(int playerId, float newX, float newY)
{
    players.at(playerId).ApplyForce(newX, newY);
}
void GameState::Step()
{
    ball.Update();
    for (Player player : players)
    {
        player.Update();
    }

    world.Step(timeStep, velocityIterations, positionIterations);

    // Check for collisions
    if (gate1.CheckCollision(ball))
    {
        score_right = gate1.get_score();
        MovePlayersToInitPositions();
    }
    if (gate2.CheckCollision(ball))
    {
        score_left = gate2.get_score();
        MovePlayersToInitPositions();
    }
    if (score_left == 5 || score_right == 5)
    {
        game_over = true;
        startNewGame();
    }
}
void GameState::removePlayer(int player_id)
{
    auto iterToRemove = players.begin() + player_id;
    iterToRemove->RemoveBody();
    players.erase(iterToRemove);
    // for (auto it = iterToRemove; it != players.end(); ++it)
    // {
    //     it->id = it->id - 1;
    // }
    numPlayers--;
}

void GameState::createGameStateMessage(char *message)
{
    float *coordinates = new float[14];
    memset(coordinates, 0, 14);
    coordinates[0] = static_cast<float>(ball.get_x());
    coordinates[1] = static_cast<float>(ball.get_y());
    for (int i = 0; i < players.size(); i++)
    {
        coordinates[2 + i * 2] = static_cast<float>(players[i].get_x());
        coordinates[3 + i * 2] = static_cast<float>(players[i].get_y());
    }

    size_t bufferSize = 14 * 6 + 1 + 4;

    int len = snprintf(message, bufferSize, "%2.2f %2.2f", coordinates[0], coordinates[1]);
    for (int i = 0; i < players.size(); i++)
    {
        len += snprintf(message + len, bufferSize - len, " %2.2f %2.2f", coordinates[2 + i * 2], coordinates[3 + i * 2]);
    }
    len += snprintf(message + len, bufferSize - len, " %d %d", score_left, score_right);

    // Don't forget to free the allocated memory
    delete[] coordinates;
}

void GameState::updateFromMessage(char *message) // float coordinates[14])
{
    // memset(coords, 0, sizeof(float) * 14);
    std::stringstream iss(message);

    float x, y;
    int i = 0;

    iss >> x >> y;
    ball.setShapePosition(x,y);

    for (int i = 0; i < numPlayers; i++)
    {
        iss >> x >> y;
        if (x != 0.0f || y != 0.0f)
            players[i].setShapePosition(x, y);
        else
            break;
    }
    iss>>score_left>>score_right;
    gate1.set_score(score_left);
    gate2.set_score(score_right);
    if(score_left>=5 || score_right>=5 ){
        game_over=true;
    }
    // while(i<14 &&coordinates[2 + i * 2]>0 && coordinates[2 + i * 2]>0 ){
    //     addPlayer("noname");
    //     players[i].moveToPosition(coordinates[2 + i * 2],coordinates[3 + i * 2]);
    // }
}

void GameState::Draw(sf::RenderWindow &window)
{
    gate1.Draw(window);
    gate2.Draw(window);
    ball.Draw(window);
    for (int i = 0; i < players.size(); i++)
    {
        players.at(i).Draw(window);
    }
    if (timeleft > 0)
        window.draw(timer);
    walls.Draw(window);
}

void GameState::DisplayTime(int time)
{
    timeleft = time;
    timer.setString(std::to_string(time));
    // timer.setOrigin(3*WINDOW_HEIGHT/2, WINDOW_WIDTH/2);
}
void GameState::setPlayersNames(char *names)
{
    for (int i = 0; i < numPlayers; i++)
    {
        char playerName[8];
        strncpy(playerName, names + i * 9, 8);
        players[i].SetName(playerName);
    }
}
void GameState::getPlayersNames(char *names)
{
    memset(names, 0, 54);
    for (int i = 0; i < numPlayers; i++)
    {
        const std::string &playerName = players[i].GetName();
        // Copy the player name into the 'names' array
        strncpy(names + i * 9, playerName.c_str(), 8);
        // Ensure null-termination
        names[(i + 1) * 9 - 1] = '\0';
    }
}
void GameState::MovePlayersToInitPositions()
{
    for (Player player : players)
    {
        player.moveToInitPosition();
    }
}