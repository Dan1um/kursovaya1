#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "Inventory.h"

class Knight {
public:
    Knight();

    void update(float dt, float gravity, const sf::FloatRect& groundBounds);
    void draw(sf::RenderWindow& window);

    // combat
    bool isHitActive() const { return hitActive; }
    sf::FloatRect getHitbox() const;
    int getAttackDamage() const { return 1; }

    void handleTileCollisions(const std::vector<sf::FloatRect>& tiles);

    void takeDamage(int dmg);
    sf::Vector2f getPosition() const { return sprite.getPosition(); }

    sf::FloatRect getBounds() const;

    sf::Vector2f getVelocity() const { return velocity; }  
    void bounce(float strength = 250.f);
    void applyKnockback(float strength, bool fromRight);
    bool isAlive() const { return hp > 0; }

    void setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos);
        position = pos;
    }

    void unlockDoubleJump() { doubleJumpUnlocked = true; maxJumps = 2; }
    void unlockDash() { dashUnlocked = true; }

    void heal(int amount);

private:
    enum class State { Idle, Run, Jump, Attack };

    void resolveTileCollisionsX(const std::vector<sf::FloatRect>& tiles);
    void resolveTileCollisionsY(const std::vector<sf::FloatRect>& tiles);

    void setState(State s);
    void updateAnimation(float dt);
    void updateHpBar();

    sf::Sprite sprite;
    sf::Vector2f position;
    std::array<sf::Texture, 4> idleTextures;
    std::array<sf::Texture, 6> runTextures;
    std::array<sf::Texture, 4> jumpTextures;
    std::array<sf::Texture, 5> attackTextures;

    // frame params (current texture)
    float frameWidth = 64.f;
    float frameHeight = 64.f;

    // animation counters
    int frame = 0;
    float frameTime = 0.f;
    float animSpeed = 0.10f;

    int runFrameCount = 6;
    int jumpFrameCount = 4;
    int attackFrameCount = 5;
    int idleFrameCount = 4;

    // abilities
    bool doubleJumpUnlocked = false;
    bool dashUnlocked = false;

    // dash
    bool dashing = false;
    float dashTime = 0.f;
    float dashCooldown = 0.f;

    // constants
    float dashDuration = 0.25f;
    float dashSpeed = 800.f;
    float dashCooldownTime = 0.6f;

    int currentFrame = 0;
    // movement
    sf::Vector2f velocity{ 0.f,0.f };
    bool onGround = false;
    bool facingRight = true;
    float moveSpeed = 220.f;
    float jumpStrength = 520.f;
    bool movingRight = true;
    int jumpCount = 0;
    int maxJumps = 1;

    // state
    State currentState = State::Idle;
    bool attacking = false;
    bool hitActive = false;
    float attackCooldown = 0.5f;
    float attackCooldownTimer = 0.f;

    // HP
    int maxHp = 5;
    int hp = 5;
    float invulTimer = 0.f;
    float invulDuration = 0.5f;
    sf::RectangleShape hpBack;
    sf::RectangleShape hpFront;

    Inventory inventory;

    int potionsSmall = 0;
    int potionsBig = 0;

};