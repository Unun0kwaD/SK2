#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Player.h"
#include "Walls.h"
#include "Ball.h"
#include "Gate.h"
#include "garabge/game_state.cpp"


int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Football", sf::Style::Close);
    window.setFramerateLimit(60);

    GameState game;
    game.startNewGame();
    game.addPlayer(WINDOW_WIDTH/3.0f, WINDOW_HEIGHT/2.0f, sf::Color::Red, "Player 1","1");
    game.addPlayer( 2.0f*WINDOW_WIDTH/3.0f, WINDOW_HEIGHT/2.0f, sf::Color::Blue, "Player 2","2");
    
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
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                game.removePlayer("1");
                
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                game.addPlayer(WINDOW_WIDTH/3.0f, WINDOW_HEIGHT/2.0f, sf::Color::Red, "Player 1","1");
                
            }
             if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                game.updatePlayerPosition("1",0.0f,20.0f);
         }
        window.clear(sf::Color(10, 200, 10));
    game.Step();
    game.Draw(window);
    window.display();
    }
    // // Physics variables
    // float timeStep = 1.0f / 60.0f;
    // int32 velocityIterations = 6;
    // int32 positionIterations = 6;

    // // Create the physics world
    // b2World world({0.0f,0.0f});

    // // Create gates
    // Gate gate1(false);
    // Gate gate2(true);

    // // Create players
    // Player player1(world, sf::Vector2f(WINDOW_WIDTH/3, WINDOW_HEIGHT/2), sf::Color::Red, "Player 1","");
    // Player player2(world, sf::Vector2f(2*WINDOW_WIDTH/3, WINDOW_HEIGHT/2), sf::Color::Blue, "Player 2","");
    
    // // Create walls
    // Walls walls(world, window.getSize().x, window.getSize().y);

    // // Create ball
    // Ball ball(world, sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), BALL_RADIUS);

    // // Set up the game loop
    // sf::Clock clock;
    // while (window.isOpen()) {
    //     // Handle events
    //     sf::Event event;
    //     while (window.pollEvent(event)) {
    //         switch (event.type) {
    //         case sf::Event::Closed:
    //             window.close();
    //             break;
    //         }
    //     }

    //     // Clear the window
    //     window.clear(sf::Color(10, 200, 10));

    //     // Handle keyboard input

    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    //         player1.ApplyForce(b2Vec2(0.0f, -PLAYER_SPEED));
    //     }
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    //         player1.ApplyForce(b2Vec2(0.0f, PLAYER_SPEED));
    //     }
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    //         player1.ApplyForce(b2Vec2(-PLAYER_SPEED, 0.0f));
    //     }
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    //         player1.ApplyForce(b2Vec2(PLAYER_SPEED, 0.0f));
    //     }

    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    //         player2.ApplyForce(b2Vec2(0.0f, -PLAYER_SPEED));
    //     }
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    //         player2.ApplyForce(b2Vec2(0.0f, PLAYER_SPEED));
    //     }
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    //         player2.ApplyForce(b2Vec2(-PLAYER_SPEED, 0.0f));
    //     }
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    //         player2.ApplyForce(b2Vec2(PLAYER_SPEED, 0.0f));
    //     }

    //     // Update physics

    //     world.Step(timeStep, velocityIterations, positionIterations);

    //     // Check for collisions
    //     gate1.CheckCollision(ball);
    //     gate2.CheckCollision(ball);

    //     // Draw objects to the screen
    //     gate1.Draw(window);
    //     gate2.Draw(window);
    //     ball.Draw(window);
    //     player1.Draw(window);
    //     player2.Draw(window);
    //     walls.Draw(window);

    //     // Update the window
    //     window.display();
    // }

    return 0;
}