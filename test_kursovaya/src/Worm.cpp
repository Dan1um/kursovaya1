#include "Worm.h"

Worm::Worm(const sf::Vector2f& pos) {
    texture.loadFromFile("assets/textures/worm.png");
    sprite.setTexture(texture);
    sprite.setPosition(pos);
    sprite.setScale(3.f, 3.f);
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    velocity.x = moveSpeed;
}

void Worm::update(float dt, const sf::FloatRect& groundBounds) {
    if (!alive) return;

    // движение червяка
    sprite.move(velocity * dt);

    // простое движение туда-сюда
    if (sprite.getPosition().x < groundBounds.left + 100.f ||
        sprite.getPosition().x + 32.f * 2 > groundBounds.left + groundBounds.width - 100.f) {
        velocity.x = -velocity.x;
        sprite.setScale((velocity.x < 0 ? 2.f : -2.f), 2.f);
    }

    // анимация
    frameTime += dt;
    if (frameTime >= animSpeed) {
        frameTime = 0.f;
        frame = (frame + 1) % maxFrames;
        sprite.setTextureRect(sf::IntRect(32 * frame, 0, 32, 32));
    }
}

void Worm::draw(sf::RenderWindow& window) {
    if (alive)
        window.draw(sprite);
}

void Worm::takeDamage(int dmg) {
    hp -= dmg;
    if (hp <= 0)
        alive = false;
}

sf::FloatRect Worm::getBounds() const {
    return sprite.getGlobalBounds();
}