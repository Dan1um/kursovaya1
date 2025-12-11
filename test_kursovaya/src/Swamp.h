#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Swamp {
public:
    Swamp(float levelWidth = 5000.f, float windowWidth = 1280.f, float windowHeight = 720.f);

    void update(const sf::Vector2f& playerPos);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getGroundBounds() const { return groundBounds; }

private:
    // === ‘ŒÕ ===
    sf::Texture backgroundTexture;
    sf::Sprite background;
    float levelWidth;
    float windowWidth, windowHeight;

    // === “¿…À€ ===
    sf::Texture tilesetTexture;
    std::vector<sf::Sprite> tiles;

    // === ƒ≈–≈¬‹ﬂ ===
    sf::Texture treeTexture;
    std::vector<sf::Sprite> trees;

    sf::FloatRect groundBounds;

    void loadTiles();
};
