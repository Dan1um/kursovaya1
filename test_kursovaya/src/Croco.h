#pragma once
#include <SFML/Graphics.hpp>

class Croco {
public:
    Croco(const sf::Vector2f& pos);

    void update(float dt, const sf::Vector2f& playerPos, const sf::FloatRect& groundBounds);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::FloatRect getHitbox() const;

    int getAttackDamage() const { return 1; }
    void takeDamage(int dmg);
    bool isAlive() const { return alive; }
    bool isHitActive() const { return hitActive; }

private:
    enum class State {
        Idle,
        Attack,
        Death
    };

    void updateAnimation(float dt);
    void updateHpBar();

    sf::Sprite sprite;

    sf::Texture idleTex;
    sf::Texture attackTex;
    sf::Texture deathTex;

    State state = State::Idle;

    int frame = 0;
    float frameTimer = 0.f;
    float frameTime = 0.12f;

    int idleFrames = 0;
    int attackFrames = 4;
    int deathFrames = 7;

    float frameWidth = 64.f;
    float frameHeight = 64.f;

    float moveSpeed = 80.f;
    float attackRange = 80.f;
    float attackCooldown = 1.2f;
    float attackTimer = 0.f;

    bool alive = true;
    bool hitActive = false;
    bool facingRight = false;

    int hp = 5;
    int maxHp = 5;
    float invulTimer = 0.f;
    float invulDuration = 0.4f;

    sf::RectangleShape hpBack;
    sf::RectangleShape hpFront;
};
