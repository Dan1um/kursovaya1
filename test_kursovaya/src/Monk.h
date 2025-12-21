#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "Inventory.h"

class Monk {
public:
    Monk();

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

    //void unlockDoubleJump() { canDoubleJump = true; }
   //void unlockDash() { canDash = true; }

    void heal(int amount);
    sf::FloatRect getFullBounds() const {
        return sprite.getGlobalBounds();
    }


private:
    enum class State { Idle, Run, Jump, Attack, Kick, Crouch, Fly };

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
    std::array<sf::Texture, 6> attackTextures;
    std::array<sf::Texture, 5> kickTextures;
    std::array<sf::Texture, 5> crouchTextures;
    std::array<sf::Texture, 2> flyTextures;

    // frame params (current texture)
    float frameWidth = 64.f;
    float frameHeight = 64.f;

    // animation counters
    int frame = 0;
    float frameTime = 0.f;
    float animSpeed = 0.10f;

    int runFrameCount = 6;
    int jumpFrameCount = 4;
    int attackFrameCount = 6;
    int kickFrameCount = 5;
    int crouchFrameCount = 5;
    int idleFrameCount = 4;
    int flyFrameCount = 2;

    Inventory inventory;

    int currentFrame = 0; // alias for clarity (index)
    // movement
    sf::Vector2f velocity{ 0.f,0.f };
    bool onGround = false;
    bool facingRight = true;
    float moveSpeed = 170.f;
    float jumpStrength = 520.f;
    bool movingRight = true;

    // state
    State currentState = State::Idle;
    bool attacking = false;
    bool hitActive = false;
    float attackCooldown = 0.2f;
    float attackCooldownTimer = 0.f;

    // HP
    int maxHp = 7;
    int hp = 7;
    float invulTimer = 0.f;
    float invulDuration = 0.5f;
    sf::RectangleShape hpBack;
    sf::RectangleShape hpFront;

    int potionsSmall = 0;
    int potionsBig = 0;

};