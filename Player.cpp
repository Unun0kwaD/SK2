#include "Player.h"
#include <iostream>

Player::Player(b2World& world, sf::Vector2f position, sf::Color color,std::string name) {
    // Create the physics body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x / SCALE, position.y / SCALE);

    m_body = world.CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = RADIUS / SCALE;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 0.3f;
    fixtureDef.friction = 0.3f;

    m_body->CreateFixture(&fixtureDef);

    // Create the graphics shape
    m_shape.setRadius(RADIUS);
    m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());
    m_shape.setFillColor(color);

    if (!m_font.loadFromFile("resources/arial.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }

    m_name.setFont(m_font);
    m_name.setString(name);
    m_name.setCharacterSize(16);
    m_name.setFillColor(sf::Color::White);
    //m_name.setOrigin(m_name.getLocalBounds().width / 2, m_name.getLocalBounds().height / 2);
    m_name.setOrigin(m_name.getLocalBounds().width / 2, m_shape.getRadius() + m_name.getLocalBounds().height);

}

void Player::ApplyForce(b2Vec2 force) {
    m_body->ApplyForceToCenter(force, true);
}

void Player::Draw(sf::RenderWindow& window) {
    b2Vec2 position = m_body->GetPosition();
    m_shape.setPosition(position.x * SCALE, position.y * SCALE);
    m_shape.setRotation(m_body->GetAngle() * 180.0f / b2_pi);
    m_name.setPosition(m_shape.getPosition());
    window.draw(m_shape);
    window.draw(m_name);
}
std::string Player::GetName()
{
    return name;
}
