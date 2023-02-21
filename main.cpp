#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "wall.h"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;
constexpr float TIME_STEP = 1.0f / 60.0f;
constexpr int VELOCITY_ITERATIONS = 8;
constexpr int POSITION_ITERATIONS = 3;
constexpr float PLAYER_SPEED = 0.5f;

int main() {
    b2World world(b2Vec2(0.0f, 0.0f));

    CreateWalls(world);

    // Create ball
    b2BodyDef ballDef;
    ballDef.type = b2_dynamicBody;
    ballDef.position.Set(WIDTH / 2 / 30.0f, HEIGHT / 2 / 30.0f);
    b2Body* ball = world.CreateBody(&ballDef);

    b2CircleShape ballShape;
    ballShape.m_radius = 0.5f;

    b2FixtureDef ballFixtureDef;
    ballFixtureDef.shape = &ballShape;
    ballFixtureDef.density = 1.0f;
    ballFixtureDef.friction = 0.3f;
    ballFixtureDef.restitution = 0.75f;
    ball->CreateFixture(&ballFixtureDef);

    // Create player 1
    b2BodyDef player1Def;
    player1Def.type = b2_dynamicBody;
    player1Def.position.Set(WIDTH / 4 / 30.0f, HEIGHT / 2 / 30.0f);
    b2Body* player1 = world.CreateBody(&player1Def);

    b2PolygonShape player1Shape;
    player1Shape.SetAsBox(0.5f, 1.0f);

    b2FixtureDef player1FixtureDef;
    player1FixtureDef.shape = &player1Shape;
    player1FixtureDef.density = 1.0f;
    player1FixtureDef.friction = 0.3f;
    player1->CreateFixture(&player1FixtureDef);

    // Create player 2
    b2BodyDef player2Def;
    player2Def.type = b2_dynamicBody;
    player2Def.position.Set(3 * WIDTH / 4 / 30.0f, HEIGHT / 2 / 30.0f);
    b2Body* player2 = world.CreateBody(&player2Def);

    b2PolygonShape player2Shape;
    player2Shape.SetAsBox(0.5f, 1.0f);

    b2FixtureDef player2FixtureDef;
    player2FixtureDef.shape = &player2Shape;
    player2FixtureDef.density = 1.0f;
    player2FixtureDef.friction = 0.3f;
    player2->CreateFixture(&player2FixtureDef);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Football Game");

    while (window.isOpen()) {
        world.Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

        b2Vec2 ballPos = ball->GetPosition();
        float ballAngle = ball->GetAngle();
        b2Vec2 player1Pos = player1->GetPosition();
        float player1Angle = player1->GetAngle();

        b2Vec2 player2Pos = player2->GetPosition();
        float player2Angle = player2->GetAngle();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Move player 1
        b2Vec2 player1Vel(0.0f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player1Vel.y -= PLAYER_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player1Vel.y += PLAYER_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player1Vel.x -= PLAYER_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player1Vel.x += PLAYER_SPEED;
        }
        player1->SetLinearVelocity(player1Vel);

        // Move player 2
        b2Vec2 player2Vel(0.0f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            player2Vel.y -= PLAYER_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            player2Vel.y += PLAYER_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player2Vel.x -= PLAYER_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player2Vel.x += PLAYER_SPEED;
        }
        player2->SetLinearVelocity(player2Vel);

        window.clear();

        sf::CircleShape ballShape(0.5 * 30.0f);
        ballShape.setOrigin(0.5 * 30.0f, 0.5 * 30.0f);
        ballShape.setPosition(ballPos.x * 30.0f, ballPos.y * 30.0f);
        ballShape.setRotation(ballAngle * 180.0f / b2_pi);
        window.draw(ballShape);

        sf::RectangleShape player1Shape(sf::Vector2f(1.0 * 30.0f, 2.0 * 30.0f));
        player1Shape.setOrigin(0.5 * 30.0f, 1.0 * 30.0f);
        player1Shape.setPosition(player1Pos.x * 30.0f, player1Pos.y * 30.0f);
        player1Shape.setRotation(player1Angle * 180.0f / b2_pi);
        window.draw(player1Shape);

        sf::RectangleShape player2Shape(sf::Vector2f(1.0 * 30.0f, 2.0 * 30.0f));
        player2Shape.setOrigin(0.5 * 30.0f, 1.0 * 30.0f);
        player2Shape.setPosition(player2Pos.x * 30.0f, player2Pos.y * 30.0f);
        player2Shape.setRotation(player2Angle * 180.0f / b2_pi);
        window.draw(player2Shape);

        DrawWalls(window);

        window.display();
    }

    return 0;
}