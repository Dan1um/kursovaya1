#include "Croco.h"
#include <cmath>
#include <iostream>

Croco::Croco(const sf::Vector2f& pos) {
    idleTex.loadFromFile("assets/textures/ghoul/ghoulIdle.png");
    attackTex.loadFromFile("assets/textures/ghoul/ghoulAttack.png");
    deathTex.loadFromFile("assets/textures/ghoul/ghoulDeath.png");

    sprite.setTexture(idleTex);

    idleFrames = idleTex.getSize().x / frameWidth;

    sprite.setTextureRect({ 0, 0, (int)frameWidth, (int)frameHeight });
    sprite.setScale(2.5f, 2.5f);
    sprite.setPosition(pos);

    hpBack.setSize({ 60.f, 6.f });
    hpBack.setFillColor(sf::Color(40, 40, 40));
    hpFront.setSize({ 60.f, 6.f });
    hpFront.setFillColor(sf::Color::Red);
}

void Croco::update(float dt, const sf::Vector2f& playerPos, const sf::FloatRect& groundBounds) {
    if (!alive && state != State::Death)
        return;

    if (invulTimer > 0.f)
        invulTimer -= dt;

    attackTimer += dt;

    sf::Vector2f pos = sprite.getPosition();
    float dx = playerPos.x - pos.x;
    facingRight = dx > 0.f;

    if (state == State::Idle && alive) {
        if (std::abs(dx) <= attackRange && attackTimer >= attackCooldown) {
            state = State::Attack;
            frame = 0;
            frameTimer = 0.f;
            attackTimer = 0.f;
            hitActive = false;
        }
        else {
            pos.x += (dx > 0 ? 1.f : -1.f) * moveSpeed * dt;
        }
    }

    // gravity
    sprite.move(0.f, 1000.f * dt);
    sprite.setPosition(pos.x, sprite.getPosition().y);

    sf::FloatRect b = sprite.getGlobalBounds();
    if (b.intersects(groundBounds)) {
        sprite.setPosition(b.left, groundBounds.top - b.height);
    }

    updateAnimation(dt);
    updateHpBar();

    float sx = std::abs(sprite.getScale().x);
    sprite.setScale(facingRight ? -sx : sx, sprite.getScale().y);
    sprite.setOrigin(facingRight ? frameWidth : 0.f, 0.f);
}

void Croco::updateAnimation(float dt) {
    frameTimer += dt;
    if (frameTimer < frameTime)
        return;

    frameTimer = 0.f;
    frame++;

    if (state == State::Idle) {
        frame %= idleFrames;
        sprite.setTexture(idleTex);
    }
    else if (state == State::Attack) {
        sprite.setTexture(attackTex);

        hitActive = (frame == 2);

        if (frame >= attackFrames) {
            frame = 0;
            state = State::Idle;
            hitActive = false;
        }
    }
    else if (state == State::Death) {
        sprite.setTexture(deathTex);
        if (frame >= deathFrames) {
            frame = deathFrames - 1; // застываем на последнем кадре
        }
    }

    sprite.setTextureRect({
        (int)(frame * frameWidth),
        0,
        (int)frameWidth,
        (int)frameHeight
        });
}

sf::FloatRect Croco::getHitbox() const {
    if (!hitActive)
        return {};

    sf::FloatRect b = sprite.getGlobalBounds();
    float w = b.width * 0.4f;
    float h = b.height * 0.4f;

    float x = facingRight ? (b.left + b.width - w) : b.left;
    float y = b.top + b.height * 0.35f;

    return { x, y, w, h };
}

void Croco::takeDamage(int dmg) {
    if (invulTimer > 0.f || state == State::Death)
        return;

    hp -= dmg;
    invulTimer = invulDuration;

    if (hp <= 0) {
        hp = 0;
        alive = false;
        state = State::Death;
        frame = 0;
        frameTimer = 0.f;
    }
}

void Croco::updateHpBar() {
    if (state == State::Death)
        return;

    float r = (float)hp / (float)maxHp;
    hpFront.setSize({ 60.f * r, 6.f });

    sf::FloatRect b = sprite.getGlobalBounds();
    sf::Vector2f p(b.left + (b.width - 60.f) / 2.f, b.top - 12.f);
    hpBack.setPosition(p);
    hpFront.setPosition(p);
}

void Croco::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    if (state != State::Death) {
        window.draw(hpBack);
        window.draw(hpFront);
    }
}
