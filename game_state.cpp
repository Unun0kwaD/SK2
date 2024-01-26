
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
        score_right = gate1.get_score();
    if (gate2.CheckCollision(ball))
        score_left = gate2.get_score();
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

    size_t bufferSize = 14 * 6 + 1;

    int len = snprintf(message, bufferSize, "%2.2f %2.2f", coordinates[0], coordinates[1]);
    for (int i = 0; i < players.size(); i++)
    {
        len += snprintf(message + len, bufferSize - len, " %2.2f %2.2f", coordinates[2 + i * 2], coordinates[3 + i * 2]);
    }

    // Don't forget to free the allocated memory
    delete[] coordinates;
}

void GameState::updateFromMessage(float coordinates[14])
{
    ball.setShapePosition(coordinates[0], coordinates[1]);
    int i;
    for (int i = 0; i < 14; i++)
    {
        if (coordinates[2 + i * 2] != 0.0f && coordinates[3 + i * 2] != 0.0f)
            players[i].setShapePosition(coordinates[2 + i * 2], coordinates[3 + i * 2]);
        else
            break;
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
void GameState::setPlayersNames(std::string names[6]){
    for (auto it = players.begin(); it != players.end(); ++it) {
        auto index = std::distance(players.begin(), it);
        if (index < 6) {
            it->SetName(names[index]);
        } else {
            break;
        }
    }
}
void GameState::getPlayersNames(char* names) {
    for (int i=0; i<6;i++) {
        const std::string& playerName = players[i].GetName();
        // Copy the player name into the 'names' array
        strncpy(names + i * 9, playerName.c_str(), 8);
        // Ensure null-termination
        names[(i + 1) * 9 - 1] = '\0';
    }
}