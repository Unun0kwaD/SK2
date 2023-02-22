#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#define SCALE 30.0f
class Ball {
public:
    Ball(b2World& world, const sf::Vector2f& position, float radius);

    void Draw(sf::RenderWindow& window);

private:
    void Update();
    b2Body* m_body;
    sf::CircleShape m_shape;
};
