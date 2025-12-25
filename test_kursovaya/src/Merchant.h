#pragma once
#include <SFML/Graphics.hpp>
#include <array>

class Merchant {
public:
    Merchant(sf::Vector2f pos);
    void update(float dt);
    void draw(sf::RenderWindow& w);

private:
    sf::Sprite sprite;
    sf::Texture texture;
    void updateAnimation(float dt);
    float frameWidth = 64.f;
    float frameHeight = 64.f;
    int idleFrameCount = 5;
    std::array<sf::Texture, 5> idleTextures;
    // animation counters
    int frame = 0;
    float frameTime = 0.f;
    float animSpeed = 0.50f;

};
