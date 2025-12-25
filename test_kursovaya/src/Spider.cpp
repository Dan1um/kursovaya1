#include "Spider.h"

Spider::Spider(const sf::Vector2f& position) : currentFrame(0), frameTime(0.f), frameDuration(0.12f) {
    velocity.x = moveSpeed;
    loadTextures();
    sprite.setScale(-2.f, 2.f);
    sprite.setPosition(position);
}

void Spider::loadTextures() {
    for (int i = 1; i <= 4; ++i) {
        sf::Texture tex;
        tex.loadFromFile("assets/textures/spider/spider" + std::to_string(i) + ".png");
        idleTextures.push_back(tex);
    }
}


void Spider::update(float dt, const sf::FloatRect& groundBounds, const sf::Vector2f& playerPos) {
    if (!alive) return;

    // движение червяка
    sprite.move(velocity * dt);

    // простое движение туда-сюда
    if (sprite.getPosition().x < groundBounds.left + 100.f ||
        sprite.getPosition().x + 32.f * 2 > groundBounds.left + groundBounds.width - 100.f) {
        velocity.x = -velocity.x;
        sprite.setScale((velocity.x < 0 ? 3.f : -3.f), 3.f);
    }

    updateAnimation(dt, playerPos);
}

void Spider::updateAnimation(float deltaTime, const sf::Vector2f&) {
    frameTime += deltaTime;

    if (frameTime >= frameDuration) {
        frameTime = 0.f;
        currentFrame++;
    }
    else {
        if (currentFrame >= (int)idleTextures.size())
            currentFrame = 0;
        sprite.setTexture(idleTextures[currentFrame]);
    }
}

void Spider::draw(sf::RenderWindow& window) {
    if (alive)
        window.draw(sprite);
}

void Spider::takeDamage(int dmg) {
    hp -= dmg;
    if (hp <= 0)
        alive = false;
}

sf::FloatRect Spider::getBounds() const {
    sf::FloatRect box = sprite.getGlobalBounds();
    box.left += 15.f;     // немного сузим слева
    box.width -= 30.f;    // сузим справа
    box.top += 10.f;      // чуть выше, чтобы совпадало с телом
    box.height -= 15.f;
    return box;
}