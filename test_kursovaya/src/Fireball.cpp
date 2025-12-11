#include "Fireball.h"

Fireball::Fireball(sf::Vector2f position, float dir)
    : speed(300.f), direction(dir), alive(true),
    currentFrame(0), frameTimer(0.f), frameDuration(0.1f)
{
    loadTextures();
    sprite.setTexture(textures[0]);
    sprite.setPosition(position);
    sprite.setScale(2.f * direction, 2.f);
}

void Fireball::loadTextures() {
    for (int i = 1; i <= 3; ++i) {
        sf::Texture tex;
        tex.loadFromFile("assets/textures/fireball-sprites/fireball" + std::to_string(i) + ".png");
        textures.push_back(tex);
    }
}

void Fireball::update(float deltaTime) {
    if (!alive) return;

    frameTimer += deltaTime;
    if (frameTimer >= frameDuration) {
        frameTimer = 0.f;
        currentFrame = (currentFrame + 1) % textures.size();
        sprite.setTexture(textures[currentFrame]);
    }

    sprite.move(speed * direction * deltaTime, 0);

    if (sprite.getPosition().x < -100 || sprite.getPosition().x > 2000)
        alive = false;
}

void Fireball::draw(sf::RenderWindow& window) {
    if (alive)
        window.draw(sprite);
}

sf::FloatRect Fireball::getBounds() const {
    sf::FloatRect b = sprite.getGlobalBounds();
    float w = b.width * 0.5f;
    float h = b.height * 0.5f;
    float x = b.left + b.width * 0.25f;
    float y = b.top + b.height * 0.25f;
    return { x, y, w, h };
}

