#pragma once
#include <SFML/Graphics.hpp>

class Croco {
public:
    Croco(const sf::Vector2f& pos);

    void update(float dt, const sf::Vector2f& playerPos, const sf::FloatRect& groundBounds);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    bool isAlive() const { return alive; }

    bool isHitActive() const { return hitActive; }
    sf::FloatRect getHitbox() const;

    int getAttackDamage() const { return 1; }
    void takeDamage(int dmg);

    void setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos);
        position = pos;
    }

private:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Texture textureRun;
    sf::Texture textureAttack;

    float frameWidth = 32.f;
    float frameHeight = 32.f;
    int runFrameCount = 1;
    int attackFrameCount = 1;
    int currentFrame = 0;
    float animationTimer = 0.f;
    float animationSpeed = 0.12f;

    bool facingRight = true;
    bool attacking = false;
    bool hitActive = false;

    float moveSpeed = 80.f;
    float attackRange = 60.f;
    float attackCooldown = 1.0f;
    float attackTimer = 0.f;

    int hp = 3;
    int maxHp = 3;
    bool alive = true;

    float invulTimer = 0.f;
    const float invulDuration = 0.25f;

    sf::RectangleShape hpBack;
    sf::RectangleShape hpFront;

    void updateHpBar();
};
