#include "wall.h"

const float WINDOW_WIDTH = 800.0f;
const float WINDOW_HEIGHT = 600.0f;

// Wall dimensions
const float WALL_THICKNESS = 0.1f;
const float WALL_HEIGHT = 30.0f;
const float WALL_WIDTH = 2.0f * (WALL_HEIGHT + 2.0f * WALL_THICKNESS);

// Wall positions
const float TOP_WALL_Y = -10.0f;
const float BOTTOM_WALL_Y = 60.0f;
const float LEFT_WALL_X = -15.0f;
const float RIGHT_WALL_X = 915.0f;

// Wall textures
sf::Texture wallTexture;
sf::Sprite wallSprite;

void CreateWall(b2World& world, float x, float y, float width, float height) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.type = b2_staticBody;

    b2PolygonShape shape;
    shape.SetAsBox(width / 2.0f, height / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.3f;

    b2Body* body = world.CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);
}

void CreateWalls(b2World& world) {
    CreateWall(world, 0.0f, TOP_WALL_Y, WINDOW_WIDTH, WALL_THICKNESS);
    CreateWall(world, 0.0f, BOTTOM_WALL_Y, WINDOW_WIDTH, WALL_THICKNESS);
    CreateWall(world, LEFT_WALL_X, 0.0f, WALL_THICKNESS, WALL_HEIGHT);
    CreateWall(world, RIGHT_WALL_X, 0.0f, WALL_THICKNESS, WALL_HEIGHT);

    wallTexture.loadFromFile("wall.png");
    wallSprite.setTexture(wallTexture);
}

void DrawWall(sf::RenderWindow& window, float x, float y, float width, float height) {
    wallSprite.setPosition(x - width / 2.0f, y - height / 2.0f);
    wallSprite.setScale(sf::Vector2f(width / wallTexture.getSize().x, height / wallTexture.getSize().y));
    window.draw(wallSprite);
}

void DrawWalls(sf::RenderWindow& window) {
    DrawWall(window, 0.0f, TOP_WALL_Y, WINDOW_WIDTH, WALL_THICKNESS);
    DrawWall(window, 0.0f, BOTTOM_WALL_Y, WINDOW_WIDTH, WALL_THICKNESS);
    DrawWall(window, LEFT_WALL_X, 0.0f, WALL_THICKNESS, WALL_HEIGHT);
    DrawWall(window, RIGHT_WALL_X, 0.0f, WALL_THICKNESS, WALL_HEIGHT);
}
