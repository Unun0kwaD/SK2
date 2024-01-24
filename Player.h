#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <constants.h>

#define SCALE 30.0f
#define RADIUS 30.0f

class Player {
public:
    Player(b2World& world, sf::Vector2f position, sf::Color color,std::string name,int id,sf::Font& m_font);
    // ~Player();
    void ApplyForce(b2Vec2 force);
    void ApplyForce(float x,float y);
    void ClampToWindowBounds(sf::Vector2f windowSize);
    void Draw(sf::RenderWindow& window);
    void moveToPosition(float x,float y);

    std::string GetName();
    int get_id() ;

    float get_x();
    float get_y();
    void setInitPosition(sf::Vector2f position);
    void moveToPosition(sf::Vector2f position);
    void moveToInitPosition();
    void RemoveBody();
    int id;
private:
    void createBody(b2World* world,sf::Vector2f position);
    b2Body* m_body;
    sf::CircleShape m_shape;
    int sockfd;
    std::string name;
    sf::Text m_name;
    sf::Vector2f init_position;
};

#endif