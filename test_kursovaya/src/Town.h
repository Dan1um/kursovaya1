#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Town {
public:
    Town(float windowWidth = 1280.f, float windowHeight = 720.f);

    void update(const sf::Vector2f& cameraCenter);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getGroundBounds() const { return groundBounds; }
    float getLevelWidth() const { return levelWidth; }
private:
    // === ‘ŒÕ ===
    sf::Texture backgroundTexture;
    sf::Sprite background;

    float levelWidth;
    float levelHeight;
    float windowWidth;
    float windowHeight;

    // === «≈ÃÀﬂ /  ŒÀÀ»«»ﬂ ===
    sf::FloatRect groundBounds;

    void setupGround();
};
