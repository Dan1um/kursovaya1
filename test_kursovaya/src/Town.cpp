#include "Town.h"
#include <iostream>

Town::Town(float windowWidth, float windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight)
{
    // === «¿√–”« ¿ ”–Œ¬Õﬂ ===
    if (!backgroundTexture.loadFromFile(
        "assets/textures/level1.png"))
    {
        std::cerr << "Failed to load level1.png\n";
    }

    background.setTexture(backgroundTexture);

    // === Ã¿—ÿ“¿¡ “ŒÀ‹ Œ œŒ ¬€—Œ“≈ ===
    float scale = windowHeight / backgroundTexture.getSize().y;
    background.setScale(scale, scale);

    levelWidth = backgroundTexture.getSize().x * scale;
    levelHeight = windowHeight;

    setupGround();
}

void Town::setupGround() {
    constexpr float GROUND_PIXEL_HEIGHT = 32.f;

    float scale = background.getScale().y;
    float groundHeight = GROUND_PIXEL_HEIGHT * scale;

    groundBounds = {
        0.f,
        windowHeight - groundHeight,
        levelWidth,
        groundHeight
    };
}

void Town::update(const sf::Vector2f& cameraCenter) {
    float parallax = 0.35f;

    float bgX = cameraCenter.x * parallax - windowWidth / 2.f;

    bgX = std::clamp(
        bgX,
        0.f,
        levelWidth - windowWidth
    );

    background.setPosition(bgX, 0.f);
}


void Town::draw(sf::RenderWindow& window) {
    window.draw(background);
}
