#ifndef GATE_HPP
#define GATE_HPP

#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Ball.h"

class Gate {
public:
    Gate(bool isLeft);
    void Draw(sf::RenderWindow& window);
    bool CheckCollision(Ball& ball);
    void IncrementScore();

private:
    sf::RectangleShape m_shape;
    sf::Text m_scoreText;
    sf::Font m_font;
    int m_score;
};

#endif // GATE_HPP
