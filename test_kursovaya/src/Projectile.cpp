#include "Projectile.h"

Bullet::Bullet(sf::Vector2f position, float dir)
    : speed(300.f), direction(dir), alive(true),
    currentFrame(0), frameTimer(0.f), frameDuration(0.1f)
{
    loadTextures();
    sprite.setTexture(textures[0]);
    sprite.setPosition(position);
    sprite.setScale(2.f * direction, 2.f);
}

void Bullet::loadTextures() {
    for (int i = 1; i <= 2; ++i) {
        sf::Texture tex;
        tex.loadFromFile("assets/textures/trooper/fire/fire" + std::to_string(i) + ".png");
        textures.push_back(tex);
    }
}

void Bullet::update(float deltaTime) {
    if (!alive) return;

    frameTimer += deltaTime;
    if (frameTimer >= frameDuration) {
        frameTimer = 0.f;
        currentFrame = (currentFrame + 1) % textures.size();
        sprite.setTexture(textures[currentFrame]);
    }

    sprite.move(speed * direction * deltaTime, 0);

    if (sprite.getPosition().x < 0 || sprite.getPosition().x > 10000)
        alive = false;
}

void Bullet::draw(sf::RenderWindow& window) {
    if (alive)
        window.draw(sprite);
}

sf::FloatRect Bullet::getBounds() const {
    sf::FloatRect b = sprite.getGlobalBounds();
    float w = b.width * 0.5f;
    float h = b.height * 0.5f;
    float x = b.left + b.width * 0.25f;
    float y = b.top + b.height * 0.25f;
    return { x, y, w, h };
}

