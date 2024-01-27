#ifndef GATE_HPP
#define GATE_HPP

#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Ball.h"

class Gate {
public:
    Gate();
    Gate(bool isLeft,sf::Font& m_font);
    void Draw(sf::RenderWindow& window);
    bool CheckCollision(Ball& ball);
    void IncrementScore();
    int get_score();
    void reset();
    void set_score(int score);
private:
    sf::RectangleShape m_shape;
    sf::Text m_scoreText;
    sf::Font m_font;
    int m_score;
};

#endif // GATE_HPP
