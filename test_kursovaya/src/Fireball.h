#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Fireball {
public:
    Fireball(sf::Vector2f position, float direction);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isAlive() const { return alive; }

private:
    sf::Sprite sprite;
    std::vector<sf::Texture> textures;
    float speed;
    float direction;
    bool alive;
    int currentFrame;
    float frameTimer;
    float frameDuration;

    void loadTextures();
};
