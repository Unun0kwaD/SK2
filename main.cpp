#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Player.h"
#include "Walls.h"
#include "Ball.h"

#define SCALE 30.0f
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BALL_RADIUS 10.0f
        float timeStep = 1.0f / 60.0f;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Football", sf::Style::Close);
    window.setFramerateLimit(60);

    // Create the physics world
    b2Vec2 gravity(0.0f, 0.0f);
    b2World world(gravity);

    // Create player 1
    Player player1(world, sf::Vector2f(100.0f, 100.0f), sf::Color::Red, "Player 1");

    // Create player 2
    Player player2(world, sf::Vector2f(400.0f, 100.0f), sf::Color::Blue, "Player 2");

    // Create walls
    Walls walls(world, window.getSize().x, window.getSize().y);

    // Create ball
    Ball ball(world, sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), BALL_RADIUS);

    // Set up the game loop
    sf::Clock clock;
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            }
        }

        // Clear the window
        window.clear(sf::Color(45, 95, 15));

        // Handle keyboard input
        const float player1_speed = 25.0f;
        const float player2_speed = 25.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player1.ApplyForce(b2Vec2(0.0f, -player1_speed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player1.ApplyForce(b2Vec2(0.0f, player1_speed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player1.ApplyForce(b2Vec2(-player1_speed, 0.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player1.ApplyForce(b2Vec2(player1_speed, 0.0f));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            player2.ApplyForce(b2Vec2(0.0f, -player2_speed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            player2.ApplyForce(b2Vec2(0.0f, player2_speed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player2.ApplyForce(b2Vec2(-player2_speed, 0.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player2.ApplyForce(b2Vec2(player2_speed, 0.0f));
        }

        // Update physics

        world.Step(timeStep, velocityIterations, positionIterations);

        // Keep players within window bounds
        //player1.ClampToWindowBounds(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        //player2.ClampToWindowBounds(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

        // Draw objects to the screen
        ball.Draw(window);
        player1.Draw(window);
        player2.Draw(window);
        walls.Draw(window);

    // Update the window
    window.display();
    }

    return 0;
}