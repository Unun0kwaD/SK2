#include "Player.h"

Player::Player(b2World& world, float x, float y, bool isFacingRight) : facingRight(isFacingRight), onGround(false) {
    // Create body
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.type = b2_dynamicBody;

    b2PolygonShape shape;
    shape.SetAsBox(PLAYER_WIDTH / 2.0f, PLAYER_HEIGHT / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.3f;

    body = world.CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);
    body->SetFixedRotation(true);
    body->SetUserData(this);

    // Load texture
    texture.loadFromFile("player.png");
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setScale(PLAYER_WIDTH / texture.getSize().x, PLAYER_HEIGHT / texture.getSize().y);
}

void Player::MoveLeft() {
    body->SetLinearVelocity(b2Vec2(-PLAYER_SPEED, body->GetLinearVelocity().y));
    if (facingRight) {
        sprite.setScale(-PLAYER_WIDTH / texture.getSize().x, PLAYER_HEIGHT / texture.getSize().y);
        facingRight = false;
    }
}

void Player::MoveRight() {
    body->SetLinearVelocity(b2Vec2(PLAYER_SPEED, body->GetLinearVelocity().y));
    if (!facingRight) {
        sprite.setScale(PLAYER_WIDTH / texture.getSize().x, PLAYER_HEIGHT / texture.getSize().y);
        facingRight = true;
    }
}

void Player::Jump() {
    if (onGround) {
        body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -PLAYER_JUMP_FORCE), true);
        onGround = false;
    }
}

void Player::Stop() {
    body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
}

void Player::Draw(sf::RenderWindow& window) {
    sprite.setPosition(body->GetPosition().x, body->GetPosition().y);
    window.draw(sprite);
}

void BeginContact(b2Contact* contact) {
    void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
}
    
void EndContact(b2Contact* contact) {
    void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
    if (bodyUserDataA && bodyUserDataB) {
        Player* playerA = static_cast<Player*>(bodyUserDataA);
        Player* playerB = static_cast<Player*>(bodyUserDataB);

        playerA->onGround = playerA->onGround || playerB->onGround;
        playerB->onGround = playerA->onGround || playerB->onGround;
    }
}
