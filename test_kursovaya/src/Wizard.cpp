#include "Wizard.h"
#include <iostream>

Wizard::Wizard(sf::Vector2f position)
    : fireTimer(0.f), fireCooldown(3.5f), hp(2), // fireCooldown  чтобы стрелял реже
    attacking(false), dying(false), dead(false),
    currentFrame(0), frameTime(0.f), frameDuration(0.12f)
{
    loadTextures();
    if (!idleTextures.empty()) {
        // получим размеры первого кадра для корректного origin
        int fw = idleTextures[0].getSize().x;
        int fh = idleTextures[0].getSize().y;
        // origin по центру по X и по нулю по Y (чтобы HP-bar / позиция легче было рассчитывать)
        sprite.setOrigin(fw / 2.f, 0.f);
    }

    sprite.setPosition(position);
    sprite.setScale(-spriteScale, spriteScale); 
    spriteFacesRightByDefault = false;

    // HP bar
    hpBack.setSize({ 80.f, 6.f });
    hpBack.setFillColor(sf::Color(40, 40, 40));
    hpFront.setSize({ 80.f, 6.f });
    hpFront.setFillColor(sf::Color::Red);
}

void Wizard::loadTextures() {
    for (int i = 1; i <= 4; ++i) {
        sf::Texture tex;
        tex.loadFromFile("assets/textures/idle-sprites/wizard-idle-" + std::to_string(i) + ".png");
        idleTextures.push_back(tex);
    }

    for (int i = 1; i <= 10; ++i) {
        sf::Texture tex;
        tex.loadFromFile("assets/textures/fire-sprites/fire" + std::to_string(i) + ".png");
        fireTextures.push_back(tex);
    }

    for (int i = 1; i <= 9; ++i) {
        sf::Texture tex;
        tex.loadFromFile("assets/textures/enemy-death-sprites/enemy-death" + std::to_string(i) + ".png");
        deathTextures.push_back(tex);
    }
}

void Wizard::update(float deltaTime, const sf::Vector2f& playerPos, std::vector<Fireball>& fireballs) {
    if (dead) return;
   
    sf::FloatRect gb = sprite.getGlobalBounds();
    float spriteCenterX = gb.left + gb.width * 0.5f;
    facingRight = (playerPos.x > spriteCenterX);

    // зеркалим по X относительно origin, учитывая spriteFacesRightByDefault
    if (spriteFacesRightByDefault) {
        sprite.setScale(facingRight ? spriteScale : -spriteScale, spriteScale);
    }
    else {
        // если спрайт в файле смотрит влево по умолчанию — инвертируем
        sprite.setScale(facingRight ? -spriteScale : spriteScale, spriteScale);
    }

    if (!dying) {
        fireTimer += deltaTime;
        if (fireTimer >= fireCooldown) {
            attacking = true;
            fireTimer = 0.f;
            shoot(fireballs);
        }
    }

    updateAnimation(deltaTime, playerPos);

    if (dying && currentFrame >= (int)deathTextures.size() - 1) {
        dead = true;
    }
}

void Wizard::updateAnimation(float deltaTime, const sf::Vector2f&) {
    frameTime += deltaTime;

    if (frameTime >= frameDuration) {
        frameTime = 0.f;
        currentFrame++;
    }

    if (dying) {
        if (currentFrame >= (int)deathTextures.size())
            currentFrame = (int)deathTextures.size() - 1;

        sprite.setTexture(deathTextures[currentFrame]);
    }
    else if (attacking) {
        if (currentFrame >= (int)fireTextures.size()) {
            currentFrame = 0;
            attacking = false;
        }
        sprite.setTexture(fireTextures[currentFrame]);
    }
    else {
        if (currentFrame >= (int)idleTextures.size())
            currentFrame = 0;
        sprite.setTexture(idleTextures[currentFrame]);
    }
}

void Wizard::shoot(std::vector<Fireball>& fireballs) {
    sf::FloatRect gb = sprite.getGlobalBounds();
    sf::Vector2f pos;
    pos.x = gb.left + gb.width * 0.5f;     // центр спрайта по X
    pos.y = gb.top + gb.height * 0.6f;     // ниже центра — на уровне руки/тела

    float direction = facingRight ? 1.f : -1.f;
    // создаём фаербол немного смещённым по X, чтобы не пересекаться со спрайтом
    pos.x += direction * (gb.width * 0.25f);

    fireballs.emplace_back(pos, direction);
}

void Wizard::draw(sf::RenderWindow& window) {
    if (!dead) {
        window.draw(sprite);
        updateHpBar();
        window.draw(hpBack);
        window.draw(hpFront);
    }
}

sf::FloatRect Wizard::getBounds() const {
    sf::FloatRect b = sprite.getGlobalBounds();
    float w = b.width * 0.6f;
    float h = b.height * 0.8f;
    float x = b.left + b.width * 0.2f;
    float y = b.top + b.height * 0.1f;
    return { x, y, w, h };
}

void Wizard::updateHpBar() {
    float ratio = std::max(0.f, (float)hp / 2.f);
    sf::FloatRect gb = sprite.getGlobalBounds();
    float barWidth = 80.f;
    float x = gb.left + gb.width * 0.5f - barWidth * 0.5f; // центрируем над спрайтом
    float y = gb.top - 10.f; // чуть над головой

    hpBack.setSize({ barWidth, 6.f });
    hpBack.setPosition(x, y);
    hpFront.setSize({ barWidth * ratio, 6.f });
    hpFront.setPosition(x, y);
}

void Wizard::takeDamage(int damage) {
    if (dead || dying) return;
    hp -= damage;
    if (hp <= 0) {
        dying = true;
        currentFrame = 0;
    }
}
