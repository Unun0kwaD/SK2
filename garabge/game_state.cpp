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

class GameState {
private:
    std::vector<Player> players;
    std::vector<Player> observers;
    b2World world=b2World({0.0f,0.0f});
    Ball ball=Ball(world, sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), BALL_RADIUS);
    Gate gate1;
    Gate gate2;
    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 6;
    Walls walls=Walls(world, WINDOW_WIDTH, WINDOW_HEIGHT);
    int score_left = 0;
    int score_right = 0;
    sf::Font m_font;



public:
GameState(){
    if (!m_font.loadFromFile("resources/arial.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }
    gate1=Gate(false,m_font);
    gate2=Gate(true,m_font);
}
    bool game_over = false;
    void startNewGame(){
        int score_left = 0;
        int score_right = 0;
        bool game_over = false;
        for (auto& player : players){
            player.moveToInitPosition();
            ball.reset();
        }
    }
    void addPlayer( float initialX, float initialY,sf::Color color, std::string name,std::string id) {
        players.emplace_back(Player(world,sf::Vector2f(initialX,initialY),sf::Color::Blue,name,id,m_font));
    }

    void updatePlayerPosition(const std::string& playerId, float newX, float newY) {
        for (auto& player : players) {
            if (player.get_id() == playerId) {
                player.ApplyForce(newX,newY);
                break;
            }
        }
    }
    void Step(){
        world.Step(timeStep, velocityIterations, positionIterations);

        // Check for collisions
        if(gate1.CheckCollision(ball))
            score_right=gate1.get_score();
        if(gate2.CheckCollision(ball))
            score_left=gate2.get_score();
        if (score_left==5 || score_right==5)
        {
            game_over=true;
            startNewGame();
        }
        
    }
    void removePlayer(std::string player_id){
        std::vector<Player>::iterator it = std::find_if(players.begin(), players.end(),
                           [&](Player& player) {
                               return player.get_id() == player_id;
                           });

        if (it != players.end()) {
            it->RemoveBody();
            players.erase(it);
        }
    }

    std::string createGameStateMessage() {
        std::stringstream ss;

        ss << "game_state { \"players\": [";
        for (auto& player : players) {
            ss << "{ \"id\": \"" << player.get_id() << "\", \"x\": " << player.get_x() << ", \"y\": " << player.get_y() << " }, ";
        }
        if (!players.empty()) {
            ss.seekp(-2, std::ios_base::end); // Remove the trailing comma
        }
        ss << "], \"ball\": { \"x\": " << ball.get_x() << ", \"y\": " << ball.get_y() << " } }";

        return ss.str();
    }
    void Draw(sf::RenderWindow& window){
        gate1.Draw(window);
        gate2.Draw(window);
        ball.Draw(window);
        for(int i=0; i<players.size();i++){
            players.at(i).Draw(window);
        }
        walls.Draw(window);
    }
};