#pragma once
#include <SFML/Graphics.hpp>

class Spider {
public:
    Spider(const sf::Vector2f& position);

    void update(float dt, const sf::FloatRect& groundBounds, const sf::Vector2f& playerPos);
    void draw(sf::RenderWindow& window);
    void takeDamage(int dmg);
    bool isAlive() const { return alive; }
    sf::FloatRect getBounds() const;

    void setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos);
        position = pos;
    }

private:
    void loadTextures();

    sf::Sprite sprite;
    sf::Vector2f position;
    std::vector<sf::Texture> idleTextures;

    int currentFrame;
    float frameTime;
    float frameDuration;

    void updateAnimation(float deltaTime, const sf::Vector2f& playerPos);

    int frame = 0;
    int maxFrames = 4;
    float animSpeed = 0.15f;

    bool alive = true;
    int hp = 1;

    float moveSpeed = 40.f;
    sf::Vector2f velocity;
};
