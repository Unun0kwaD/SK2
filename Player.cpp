#include "Player.h"
#include <iostream>

Player::Player(b2World& world, sf::Vector2f position, sf::Color color,std::string name,std::string in_id,sf::Font& m_font) {
    // Create the physics body
    createBody(&world,position);
    this->name=name;
    // Create the graphics shape
    m_shape.setRadius(RADIUS);
    m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());
    m_shape.setFillColor(color);
    this->id=in_id;
    // if (in_id=="")
    //     id=name.append(std::to_string(rand()));
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
float unify_speed(float a){
    if (a>0.0f) return 1.0f*PLAYER_SPEED;
    if (a<0.0f) return -1.0f*PLAYER_SPEED;
    return 0.0f;
}

void Player::ApplyForce(float x,float y) {
    m_body->ApplyForceToCenter(b2Vec2(unify_speed(x),unify_speed(y)),true);
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
std::string Player::get_id() {
    return id;
}
float Player::get_x(){
    return m_body->GetPosition().x;
}
float Player::get_y(){
    return m_body->GetPosition().y;
}
void Player::setInitPosition(sf::Vector2f position){
    init_position=position;
}
void Player::moveToInitPosition(){
    this->moveToPosition(init_position);
}
void Player::moveToPosition(sf::Vector2f position){
    b2World* worldptr = m_body->GetWorld();
    m_body->GetWorld()->DestroyBody(m_body);
    createBody(worldptr,position);
}
void Player::createBody(b2World* world,sf::Vector2f position){
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    this->setInitPosition(position);
    bodyDef.position.Set(position.x / SCALE, position.y / SCALE);

    m_body = world->CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = RADIUS / SCALE;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 0.3f;
    fixtureDef.friction = 0.3f;

    m_body->CreateFixture(&fixtureDef);
}
void Player::RemoveBody(){
    b2World* worldptr = m_body->GetWorld();
    m_body->GetWorld()->DestroyBody(m_body);
    std::cout<<"Player is dead"<<std::endl;
}