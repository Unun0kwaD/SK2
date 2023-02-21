#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

void CreateWalls(b2World& world);

void DrawWalls(sf::RenderWindow& window);
