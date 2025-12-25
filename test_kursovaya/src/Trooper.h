#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Projectile.h"
#include "Inventory.h"

class Trooper {
public:
    Trooper();

    void update(float dt, float gravity, const sf::FloatRect& groundBounds, std::vector<Bullet>& bullets);
    void draw(sf::RenderWindow& w);

    // --- Совместимость с Knight ---
    bool isAlive() const { return hp > 0; }
    bool isHitActive() const { return false; } // ближняя атака отключена
    sf::FloatRect getHitbox() const { return {}; } // пустая зона удара
    int getAttackDamage() const { return 1; }
    void applyKnockback(float strength, bool fromRight);

    void takeDamage(int dmg);
    void bounce(float strength = 250.f);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    sf::Vector2f getVelocity() const { return velocity; }
    void setPosition(const sf::Vector2f& p) { sprite.setPosition(p); }

    void unlockDoubleJump() { doubleJumpUnlocked = true; maxJumps = 2; }
    void unlockDash() { dashUnlocked = true; }

    void heal(int amount);
private:
    enum class State { Idle, Run, Jump, Shoot };

    sf::Sprite sprite;
    sf::Texture idleTextures[6];
    sf::Texture runTextures[14];
    sf::Texture jumpTextures[4];
    sf::Texture shootTextures[3];
    sf::Texture bulletTextures[2];

    // abilities
    bool doubleJumpUnlocked = false;
    bool dashUnlocked = false;

    // dash
    bool dashing = false;
    float dashTime = 0.f;
    float dashCooldown = 0.f;

    // constants
    float dashDuration = 0.15f;
    float dashSpeed = 800.f;
    float dashCooldownTime = 0.6f;

    int frame = 0;
    float frameTime = 0.f;
    float animSpeed = 0.1f;
    State currentState = State::Idle;

    bool facingRight = true;
    bool onGround = true;

    sf::Vector2f velocity{ 0,0 };
    float moveSpeed = 220.f;
    float jumpStrength = 520.f;
    int jumpCount = 0;
    int maxJumps = 1;

    int frameWidth = 0;
    int frameHeight = 0;

    int maxHp = 5;
    int hp = 5;
    float invulTimer = 0.f;
    float invulDuration = 0.4f;

    Inventory inventory;

    // UI
    sf::RectangleShape hpBack;
    sf::RectangleShape hpFront;

    // дальняя атака
    float shootCooldown = 0.6f;
    float shootTimer = 0.f;

    // --- Дистанционная атака ---
    void shoot(std::vector<Bullet>& projectiles);

    int potionsSmall = 0;
    int potionsBig = 0;

    void setState(State s);
    void updateAnimation(float dt, std::vector<Bullet>& bullets);
    void updateHpBar();
};
