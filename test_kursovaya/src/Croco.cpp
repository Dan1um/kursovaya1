    #include "Croco.h"
    #include <iostream>
    #include <cmath>

    Croco::Croco(const sf::Vector2f& pos) {
        if (!textureRun.loadFromFile("assets/textures/croco mob run.png")) std::cerr << "croco run load fail\n";
        if (!textureAttack.loadFromFile("assets/textures/croco mob attack.png")) std::cerr << "croco attack load fail\n";

        sprite.setTexture(textureRun);
        frameWidth = 32.f;
        frameHeight = 32.f;
        runFrameCount = (int)(textureRun.getSize().x / frameWidth);
        attackFrameCount = (int)(textureAttack.getSize().x / frameWidth);

        sprite.setTextureRect(sf::IntRect(0, 0, (int)frameWidth, (int)frameHeight));
        sprite.setScale(3.f, 3.f);
        sprite.setPosition(pos.x, pos.y + 100.f);

        hpBack.setSize({ 60.f,6.f });
        hpBack.setFillColor(sf::Color(50, 50, 50));
        hpFront.setSize({ 60.f,6.f });
        hpFront.setFillColor(sf::Color::Red);
    }

    void Croco::update(float dt, const sf::Vector2f& playerPos, const sf::FloatRect& groundBounds) {
        if (!alive) return;

        attackTimer += dt;
        if (invulTimer > 0.f) invulTimer -= dt;

        sf::Vector2f pos = sprite.getPosition();
        float dx = playerPos.x - pos.x;

        // decide
        if (!attacking) {
            if (std::abs(dx) <= attackRange && attackTimer >= attackCooldown) {
                attacking = true;
                attackTimer = 0.f;
                currentFrame = 0;
                hitActive = false;
            }
            else {
                if (std::abs(dx) > 5.f) pos.x += (dx > 0 ? 1.f : -1.f) * moveSpeed * dt;
            }
        }

        // apply X, gravity and snap to ground
        sprite.setPosition(pos.x, sprite.getPosition().y);
        sprite.move(0.f, 1000.f * dt);
        sf::FloatRect bounds = sprite.getGlobalBounds();
        if (bounds.intersects(groundBounds))
            sprite.setPosition(bounds.left, groundBounds.top - bounds.height);

        // animation
        animationTimer += dt;
        if (animationTimer >= animationSpeed) {
            animationTimer = 0.f;
            currentFrame++;

            if (attacking) {
                sprite.setTexture(textureAttack);
                // use attackFrameCount for bounds checking
                if (currentFrame >= attackFrameCount) {
                    // end attack cleanly
                    currentFrame = 0;
                    attacking = false;
                    hitActive = false;
                    sprite.setTexture(textureRun);
                }
                else {
                    hitActive = (currentFrame == attackFrameCount / 2);
                }
                sprite.setTextureRect(sf::IntRect((int)(currentFrame * frameWidth), 0, (int)frameWidth, (int)frameHeight));
            }
            else {
                sprite.setTexture(textureRun);
                currentFrame = currentFrame % runFrameCount;
                sprite.setTextureRect(sf::IntRect((int)(currentFrame * frameWidth), 0, (int)frameWidth, (int)frameHeight));
            }
        }

        // mirror: croco art faces left by default — make it look toward player
        facingRight = (dx > 0);
        float sx = std::abs(sprite.getScale().x);
        if (facingRight) {
            sprite.setScale(-sx, sprite.getScale().y);
            sprite.setOrigin(frameWidth, 0.f);
        }
        else {
            sprite.setScale(sx, sprite.getScale().y);
            sprite.setOrigin(0.f, 0.f);
        }

        updateHpBar();
    }

    sf::FloatRect Croco::getHitbox() const {
        if (!hitActive) return {};
        sf::FloatRect b = sprite.getGlobalBounds();
        float w = b.width * 0.5f;
        float h = b.height * 0.45f;
        float y = b.top + b.height * 0.35f;
        float x = facingRight ? (b.left + b.width - w) : (b.left - w);
        return { x, y, w, h };
    }

    void Croco::takeDamage(int dmg) {
        if (invulTimer > 0.f || !alive) return;
        hp -= dmg;
        if (hp <= 0) { hp = 0; alive = false; }
        invulTimer = invulDuration;
        updateHpBar();
    }

    void Croco::updateHpBar() {
        float r = (float)hp / (float)maxHp;
        hpFront.setSize({ 60.f * r, 6.f });
        sf::FloatRect b = sprite.getGlobalBounds();
        sf::Vector2f pos(b.left + (b.width - 60.f) / 2.f, b.top - 12.f);
        hpBack.setPosition(pos);
        hpFront.setPosition(pos);
    }

    void Croco::draw(sf::RenderWindow& window) const {
        if (!alive) return;
        window.draw(sprite);
        window.draw(hpBack);
        window.draw(hpFront);
    }
