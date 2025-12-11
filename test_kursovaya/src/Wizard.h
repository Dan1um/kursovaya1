#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Fireball.h"

class Wizard {
public:
    Wizard(sf::Vector2f position);

    void update(float deltaTime, const sf::Vector2f& playerPos, std::vector<Fireball>& fireballs);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    void takeDamage(int damage);
    bool isDead() const { return dead; }
    void setFacingRight(bool right) {
        facingRight = right;
        sprite.setScale(right ? 2.f : -2.f, 2.f);
    }

    void setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos);
        position = pos;
    }

private:
    sf::Sprite sprite;
    sf::Vector2f position;
    std::vector<sf::Texture> idleTextures;
    std::vector<sf::Texture> fireTextures;
    std::vector<sf::Texture> deathTextures;

    float fireTimer;
    float fireCooldown;

    int hp;
    bool facingRight = true;
    bool attacking;
    bool dying;
    bool dead;
    sf::RectangleShape hpBack;
    sf::RectangleShape hpFront;

    float spriteScale = 2.f;
    bool spriteFacesRightByDefault = true;

    int currentFrame;
    float frameTime;
    float frameDuration;

    void loadTextures();
    void updateHpBar();
    void shoot(std::vector<Fireball>& fireballs);
    void updateAnimation(float deltaTime, const sf::Vector2f& playerPos);
};
