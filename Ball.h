#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "constants.h"

#define SCALE 30.0f
class Ball {
public:
    Ball(b2World& world, const sf::Vector2f& position, float radius);
    void Draw(sf::RenderWindow& window);
    sf::CircleShape& GetShape();
    void reset();
    void setPosition(float x, float y);
    float get_x();
    float get_y();
private:

    void SetPosition(const sf::Vector2f& position);
    void Update();
    b2Body* m_body;
    sf::CircleShape m_shape;
};
