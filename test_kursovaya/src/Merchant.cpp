#include "Merchant.h"

Merchant::Merchant(sf::Vector2f pos) {
    for (int i = 0; i < idleFrameCount; ++i)
        idleTextures[i].loadFromFile("assets/textures/shop/merchant/merchant" + std::to_string(i + 1) + ".png");
    sprite.setTexture(idleTextures[0]);
    frameWidth = idleTextures[0].getSize().x;
    frameHeight = idleTextures[0].getSize().y;
    sprite.setScale(2.f, 2.f);
    sprite.setPosition(370.f, 540.f);
}

void Merchant::updateAnimation(float dt) {
    frameTime += dt;
    if (frameTime < animSpeed) return;
    frameTime = 0;
    frame = (frame + 1) % idleFrameCount;
    sprite.setTexture(idleTextures[frame]);
}

void Merchant::update(float dt) {
    updateAnimation(dt);
}

void Merchant::draw(sf::RenderWindow& w) {
    w.draw(sprite);
}
