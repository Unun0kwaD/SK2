#include "Walls.h"

Walls::Walls(b2World& world, float windowWidth, float windowHeight) {
    // Create top wall
    b2BodyDef topDef;
    topDef.position.Set(windowWidth / SCALE / 2, 0.0f);
    m_bodies[0] = world.CreateBody(&topDef);

    b2PolygonShape topShape;
    topShape.SetAsBox(windowWidth / SCALE / 2, WALL_THICKNESS / SCALE);

    b2FixtureDef topFixtureDef;
    topFixtureDef.shape = &topShape;

    m_bodies[0]->CreateFixture(&topFixtureDef);

    // Create left wall
    b2BodyDef leftDef;
    leftDef.position.Set(0.0f, windowHeight / SCALE / 2);
    m_bodies[1] = world.CreateBody(&leftDef);

    b2PolygonShape leftShape;
    leftShape.SetAsBox(WALL_THICKNESS / SCALE, windowHeight / SCALE / 2);

    b2FixtureDef leftFixtureDef;
    leftFixtureDef.shape = &leftShape;

    m_bodies[1]->CreateFixture(&leftFixtureDef);

    // Create right wall
    b2BodyDef rightDef;
    rightDef.position.Set(windowWidth / SCALE , windowHeight / SCALE / 2);
    m_bodies[2] = world.CreateBody(&rightDef);

    b2PolygonShape rightShape;
    rightShape.SetAsBox(WALL_THICKNESS/ SCALE, windowHeight / SCALE / 2);

    b2FixtureDef rightFixtureDef;
    rightFixtureDef.shape = &rightShape;

    m_bodies[2]->CreateFixture(&rightFixtureDef);

    // Create bottom wall
    b2BodyDef bottomDef;
    bottomDef.position.Set(windowWidth / SCALE / 2, windowHeight / SCALE);
    m_bodies[3] = world.CreateBody(&bottomDef);

    b2PolygonShape bottomShape;
    bottomShape.SetAsBox(windowWidth / SCALE / 2, WALL_THICKNESS/ SCALE);

    b2FixtureDef bottomFixtureDef;
    bottomFixtureDef.shape = &bottomShape;

    m_bodies[3]->CreateFixture(&bottomFixtureDef);

    // Create graphics shapes
    for (int i = 0; i < 4; i++) {
        m_shapes[i].setSize(sf::Vector2f(
            (i == 0 || i == 3) ? windowWidth : WALL_THICKNESS*2,
            (i == 1 || i == 2) ? windowHeight : WALL_THICKNESS*2
        ));
        m_shapes[i].setOrigin(m_shapes[i].getSize() / 2.0f);
        m_shapes[i].setFillColor(sf::Color(128, 128, 128));
    }

}

void Walls::Draw(sf::RenderWindow& window) {
    for (int i = 0; i < 4; i++) {
        b2Vec2 position = m_bodies[i]->GetPosition();
        m_shapes[i].setPosition(position.x * SCALE, position.y * SCALE);
        m_shapes[i].setRotation(m_bodies[i]->GetAngle() * 180.0f / b2_pi);
        window.draw(m_shapes[i]);
    }
}