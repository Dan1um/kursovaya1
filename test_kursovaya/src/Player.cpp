#include "Player.h"
#include <iostream>

Player::Player() {
    textureRun.loadFromFile("assets/textures/player.png");
    textureJump.loadFromFile("assets/textures/jump.png");
    textureAttack.loadFromFile("assets/textures/atack.png");

    sprite.setTexture(textureRun);
    runFrameCount = 8;
    jumpFrameCount = 4;
    attackFrameCount = 6;
    frameWidth = textureRun.getSize().x / runFrameCount;
    frameHeight = textureRun.getSize().y;
    sprite.setTextureRect({ 0, 0, (int)frameWidth, (int)frameHeight });
    sprite.setScale(2.f, 2.f);
    sprite.setPosition(200.f, 400.f);

    hp = maxHp = 5;
    hpBack.setSize({ 100, 8 });
    hpBack.setFillColor(sf::Color(40, 40, 40));
    hpFront.setSize({ 100, 8 });
    hpFront.setFillColor(sf::Color::Green);
}

void Player::update(float dt, float gravity, const sf::FloatRect& groundBounds) {
    if (hp <= 0) return;

    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool jump = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool attack = sf::Keyboard::isKeyPressed(sf::Keyboard::J);

    if (attackCooldownTimer > 0) attackCooldownTimer -= dt;
    if (invulTimer > 0) invulTimer -= dt;

    if (!attacking) velocity.x = 0;
    if (!attacking) {
        if (left) { velocity.x = -moveSpeed; facingRight = false; }
        if (right) { velocity.x = moveSpeed; facingRight = true; }
        if (onGround && jump) { velocity.y = -jumpStrength; onGround = false; setState(State::Jump); }
        if (onGround && attack && attackCooldownTimer <= 0) { setState(State::Attack); attacking = true; hitActive = false; frame = 0; attackCooldownTimer = attackCooldown; }
    }

    velocity.y += gravity * dt;
    sprite.move(velocity * dt);

    // ground check
    sf::FloatRect b = sprite.getGlobalBounds();
    if (b.intersects(groundBounds)) {
        sprite.setPosition(sprite.getPosition().x, groundBounds.top - b.height);
        velocity.y = 0;
        onGround = true;
    }
    else onGround = false;

    if (onGround && !attacking) {
        if (std::abs(velocity.x) < 0.1f) setState(State::Idle);
        else setState(State::Run);
    }

    // update animation
    updateAnimation(dt);

    // mirror
    float sx = std::abs(sprite.getScale().x);
    sprite.setScale(facingRight ? sx : -sx, sprite.getScale().y);
    sprite.setOrigin(facingRight ? 0.f : frameWidth, 0.f);

    updateHpBar();
}

void Player::updateAnimation(float dt) {
    frameTime += dt;
    if (frameTime < animSpeed) return;
    frameTime = 0;

    if (currentState == State::Idle) {
        sprite.setTexture(textureRun);
        sprite.setTextureRect({ 0, 0, (int)frameWidth, (int)frameHeight });
        return;
    }

    if (currentState == State::Run) {
        sprite.setTexture(textureRun);
        frame = (frame + 1) % runFrameCount;
        sprite.setTextureRect({ (int)(frame * frameWidth), 0, (int)frameWidth, (int)frameHeight });
    }
    else if (currentState == State::Jump) {
        sprite.setTexture(textureJump);
        frame = std::min(frame + 1, jumpFrameCount - 1);
        sprite.setTextureRect({ (int)(frame * (textureJump.getSize().x / jumpFrameCount)), 0, (int)frameWidth, (int)frameHeight });
    }
    else if (currentState == State::Attack) {
        sprite.setTexture(textureAttack);
        frame++;
        int texW = textureAttack.getSize().x / attackFrameCount;
        if (frame >= attackFrameCount) {
            attacking = false;
            hitActive = false;
            frame = 0;
            setState(onGround ? State::Idle : State::Jump);
        }
        else {
            sprite.setTextureRect({ frame * texW, 0, texW, (int)frameHeight });
            hitActive = (frame == attackFrameCount / 2);
        }
    }
}

void Player::setState(State s) {
    if (currentState == s) return;
    currentState = s;
    frame = 0;
    frameTime = 0;
    hitActive = false;
}

sf::FloatRect Player::getHitbox() const {
    if (!hitActive) return {};
    sf::FloatRect b = sprite.getGlobalBounds();
    float w = b.width * 0.4f;
    float h = b.height * 0.5f;
    float x = facingRight ? (b.left + b.width * 0.6f) : (b.left - w * 0.2f);
    float y = b.top + b.height * 0.2f;
    return { x, y, w, h };
}

void Player::takeDamage(int dmg) {
    if (invulTimer > 0) return;
    hp -= dmg;
    if (hp <= 0) hp = 0;
    invulTimer = invulDuration;
}

void Player::bounce(float s) { velocity.y = -s; onGround = false; }

void Player::updateHpBar() {
    float ratio = (float)hp / maxHp;
    hpFront.setSize({ 100 * ratio, 8 });
    sf::Vector2f pos(sprite.getPosition().x - 10, sprite.getPosition().y - 20);
    hpBack.setPosition(pos);
    hpFront.setPosition(pos);
}

void Player::draw(sf::RenderWindow& w) {
    if (hp > 0) { w.draw(sprite); w.draw(hpBack); w.draw(hpFront); }
}
