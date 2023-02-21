#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Player {
public:
    Player(b2World& world, float x, float y, bool isPlayer1);

    void Update();

    void Draw(sf::RenderWindow& window);

    void Move(float x, float y);

    void Kick(float x, float y);

private:
    b2Body* body;
    bool player1;
    sf::Texture texture;
    sf::Sprite sprite;
    float speed = 8.0f;
    float kickStrength = 100.0f;
};
