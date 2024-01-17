#include "Gate.h"

Gate::Gate(bool isLeft)
    : m_score(0)
{
    if (isLeft) {
        m_shape.setPosition(0, WINDOW_HEIGHT/2-GATE_HEIGHT/2);
    } else {
        m_shape.setPosition(WINDOW_WIDTH-GATE_WIDTH, WINDOW_HEIGHT/2-GATE_HEIGHT/2);
    }
    
    m_shape.setSize(sf::Vector2f(GATE_WIDTH, GATE_HEIGHT));
    m_shape.setFillColor(sf::Color::White);

    if (!m_font.loadFromFile("resources/arial.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(30);
    m_scoreText.setFillColor(sf::Color::Black);
    m_scoreText.setPosition(sf::Vector2f(m_shape.getPosition().x + (isLeft ? GATE_WIDTH + 20 : -50), m_shape.getPosition().y  + 20));
    m_scoreText.setString(std::to_string(m_score));
}

void Gate::Draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
    window.draw(m_scoreText);
}

bool Gate::CheckCollision(Ball& ball)
{

    // Check if ball is colliding with the gate
    sf::FloatRect ballBounds = ball.GetShape().getGlobalBounds();
    sf::FloatRect gateBounds = m_shape.getGlobalBounds();
    if (ballBounds.intersects(gateBounds)) {
        // If so, reset ball position and increment score
        ball.reset();
        IncrementScore();
        return true;
    }
    return false;
}

void Gate::IncrementScore()
{
    m_score++;
    m_scoreText.setString(std::to_string(m_score));
}
