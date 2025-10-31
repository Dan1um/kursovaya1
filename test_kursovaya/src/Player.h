#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();

    void update(float dt, float gravity, const sf::FloatRect& groundBounds);
    void draw(sf::RenderWindow& window);

    // combat
    bool isHitActive() const { return hitActive; }
    sf::FloatRect getHitbox() const;
    int getAttackDamage() const { return 1; }

    void takeDamage(int dmg);
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f getVelocity() const { return velocity; }   // <-- нужно для stomp
    void bounce(float strength = 250.f);

    bool isAlive() const { return hp > 0; }

private:
    enum class State { Idle, Run, Jump, Attack };

    void setState(State s);
    void updateAnimation(float dt);
    void updateHpBar();

    sf::Sprite sprite;
    sf::Texture textureRun;
    sf::Texture textureJump;
    sf::Texture textureAttack;

    // frame params (current texture)
    float frameWidth = 64.f;
    float frameHeight = 64.f;

    // animation counters
    int frame = 0;
    float frameTime = 0.f;
    float animSpeed = 0.10f;

    int runFrameCount = 8;
    int jumpFrameCount = 4;
    int attackFrameCount = 6;

    int currentFrame = 0; // alias for clarity (index)
    // movement
    sf::Vector2f velocity{ 0.f,0.f };
    bool onGround = false;
    bool facingRight = true;
    float moveSpeed = 220.f;
    float jumpStrength = 450.f;

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
};
