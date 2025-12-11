#pragma once
#include <SFML/Graphics.hpp>

class Worm {
public:
    Worm(const sf::Vector2f& pos);

    void update(float dt, const sf::FloatRect& groundBounds);
    void draw(sf::RenderWindow& window);
    void takeDamage(int dmg);
    bool isAlive() const { return alive; }
    sf::FloatRect getBounds() const;

    void setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos);
        position = pos;
    }

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;

    float frameTime = 0.f;
    int frame = 0;
    int maxFrames = 4;
    float animSpeed = 0.15f;

    bool alive = true;
    int hp = 1;

    float moveSpeed = 40.f;
    sf::Vector2f velocity;
};
