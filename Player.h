#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#define SCALE 30.0f
#define RADIUS 30.0f

class Player {
public:
    Player(b2World& world, sf::Vector2f position, sf::Color color,std::string name);

    void ApplyForce(b2Vec2 force);
    void ClampToWindowBounds(sf::Vector2f windowSize);
    void Draw(sf::RenderWindow& window);

    std::string GetName();

private:
    b2Body* m_body;
    sf::CircleShape m_shape;

    sf::Text m_name;
    sf::Font m_font;

    std::string name;
};

#endif