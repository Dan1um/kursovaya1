#include "Trooper.h"
#include <SFML/Window.hpp>

Trooper::Trooper() {
    for (int i = 0; i < 6; ++i)
        idleTextures[i].loadFromFile("assets/textures/trooper/idle/idle" + std::to_string(i + 1) + ".png");
    for (int i = 0; i < 14; ++i)
        runTextures[i].loadFromFile("assets/textures/trooper/run/run" + std::to_string(i + 1) + ".png");
    for (int i = 0; i < 4; ++i)
        jumpTextures[i].loadFromFile("assets/textures/trooper/jump/jump" + std::to_string(i + 1) + ".png");
    for (int i = 0; i < 3; ++i)
        shootTextures[i].loadFromFile("assets/textures/trooper/shoot/shoot" + std::to_string(i + 1) + ".png");

    sprite.setTexture(idleTextures[0]);
    frameWidth = idleTextures[0].getSize().x;
    frameHeight = idleTextures[0].getSize().y;

    sprite.setScale(2.f, 2.f);
    sprite.setPosition(200.f, 400.f);

    hpBack.setSize({ 100, 8 });
    hpBack.setFillColor(sf::Color(40, 40, 40));
    hpFront.setSize({ 100, 8 });
    hpFront.setFillColor(sf::Color::Green);
}

void Trooper::update(float dt, float gravity, const sf::FloatRect& groundBounds, std::vector<Bullet>& bullets) {
    if (!isAlive()) return;

    bool L = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool R = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool J = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool Shoot = sf::Keyboard::isKeyPressed(sf::Keyboard::J);

    if (invulTimer > 0) invulTimer -= dt;
    if (shootTimer > 0) shootTimer -= dt;

    // движение
    velocity.x = 0;
    if (L) { velocity.x = -moveSpeed; facingRight = false; }
    if (R) { velocity.x = moveSpeed; facingRight = true; }

    if (onGround && J) {
        velocity.y = -jumpStrength;
        onGround = false;
        setState(State::Jump);
    }

    // атака
    if (Shoot && shootTimer <= 0 && onGround) {
        setState(State::Shoot);
        frame = 0;
        shootTimer = shootCooldown;
    }

    velocity.y += gravity * dt;
    sprite.move(velocity * dt);

    // ground
    sf::FloatRect b = sprite.getGlobalBounds();
    if (b.intersects(groundBounds)) {
        sprite.setPosition(sprite.getPosition().x, groundBounds.top - b.height);
        velocity.y = 0;
        onGround = true;
    }
    else onGround = false;

    if (onGround && currentState != State::Shoot) {
        if (std::abs(velocity.x) < 0.1f) setState(State::Idle);
        else setState(State::Run);
    }

    updateAnimation(dt, bullets);

    float sx = std::abs(sprite.getScale().x);
    sprite.setScale(facingRight ? sx : -sx, sprite.getScale().y);
    sprite.setOrigin(facingRight ? 0 : frameWidth, 0);

    updateHpBar();
}

void Trooper::updateAnimation(float dt, std::vector<Bullet>& bullets) {
    frameTime += dt;
    if (frameTime < animSpeed) return;
    frameTime = 0;

    switch (currentState) {
    case State::Idle:
        frame = (frame + 1) % 6;
        sprite.setTexture(idleTextures[frame]);
        break;

    case State::Run:
        frame = (frame + 1) % 14;
        sprite.setTexture(runTextures[frame]);
        break;

    case State::Jump:
        frame = std::min(frame + 1, 3);
        sprite.setTexture(jumpTextures[frame]);
        break;

    case State::Shoot:
        frame++;

        if (frame == 1) {
            shoot(bullets);
        }

        if (frame >= 3) {
            // анимация закончилась — выходим
            setState(onGround ? State::Idle : State::Jump);
        }
        else {
            sprite.setTexture(shootTextures[frame]);
        }
        break;  
    }
}

void Trooper::setState(State s) {
    if (currentState == s) return;
    currentState = s;
    frame = 0;
    frameTime = 0;
}

void Trooper::updateHpBar() {
    float ratio = (float)hp / maxHp;
    hpFront.setSize({ 100 * ratio, 8 });
    sf::Vector2f pos(sprite.getPosition().x - 10, sprite.getPosition().y - 20);
    hpBack.setPosition(pos);
    hpFront.setPosition(pos);
}

sf::FloatRect Trooper::getBounds() const {
    sf::FloatRect b = sprite.getGlobalBounds();
    b.left += b.width * 0.25f;
    b.width *= 0.5f;
    b.top += b.height * 0.2f;
    b.height *= 0.8f;
    return b;
}

void Trooper::takeDamage(int dmg) {
    if (invulTimer > 0) return;

    hp -= dmg;
    if (hp <= 0) hp = 0;
    invulTimer = invulDuration;

    // исправленный отскок — противоположно направлению взгляда
    applyKnockback(600.f, facingRight);
}

void Trooper::applyKnockback(float strength, bool fromRight) {
    velocity.x = fromRight ? -strength : strength;
    velocity.y = -strength * 0.35f;
    onGround = false;
}

void Trooper::bounce(float s) {
    velocity.y = -s;
    onGround = false;
}

void Trooper::shoot(std::vector<Bullet>& bullets) {
    sf::FloatRect b = sprite.getGlobalBounds();
    sf::Vector2f pos = { b.left + b.width * (facingRight ? 0.8f : 0.2f),
                         b.top + b.height * 0.30f};

    float dir = facingRight ? 1.f : -1.f;

    bullets.emplace_back(pos, dir);
}

void Trooper::heal(int amount) {
    hp = std::min(hp + amount, maxHp);
}

void Trooper::draw(sf::RenderWindow& w) {
    if (hp > 0) {
        w.draw(sprite);
        w.draw(hpBack);
        w.draw(hpFront);
    }
}

