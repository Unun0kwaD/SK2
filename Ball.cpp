#include "Ball.h"

Ball::Ball(b2World& world, const sf::Vector2f& position, float radius)
{
    // Create Box2D body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x / SCALE, position.y / SCALE);

    m_body = world.CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = radius / SCALE;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.6f;

    m_body->CreateFixture(&fixtureDef);

    // Create graphics shape
    m_shape.setRadius(radius);
    m_shape.setOrigin(radius, radius);
    m_shape.setFillColor(sf::Color(255, 255, 255));
}

void Ball::Update()
{
    // Update graphics shape position and rotation
    m_shape.setPosition(m_body->GetPosition().x * SCALE, m_body->GetPosition().y * SCALE);
    m_shape.setRotation(m_body->GetAngle() * 180.0f / b2_pi);
}

void Ball::Draw(sf::RenderWindow& window)
{
    this->Update();
    window.draw(m_shape);
}
