#ifndef WALLS_H
#define WALLS_H

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

#define SCALE 30.0f
#define WALL_THICKNESS 3.0f
#define GATE_THICKNESS 10.0f

class Walls {
public:
    Walls(b2World& world, float windowWidth, float windowHeight);

    void Draw(sf::RenderWindow& window);

private:
    b2Body* m_bodies[6];
    sf::RectangleShape m_shapes[6];
};

#endif
