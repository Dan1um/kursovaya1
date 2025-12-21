#include "Monk.h"

Monk::Monk() {
    for (int i = 0; i < runFrameCount; ++i)
        runTextures[i].loadFromFile("assets/textures/monk/walk/walk" + std::to_string(i + 1) + ".png");
    for (int i = 0; i < idleFrameCount; ++i)
        idleTextures[i].loadFromFile("assets/textures/monk/idle/idle" + std::to_string(i + 1) + ".png");
    for (int i = 0; i < jumpFrameCount; ++i)
        jumpTextures[i].loadFromFile("assets/textures/monk/jump/jump" + std::to_string(i + 1) + ".png");
    for (int i = 0; i < attackFrameCount; ++i)
        attackTextures[i].loadFromFile("assets/textures/monk/attack/attack" + std::to_string(i + 1) + ".png");
    for (int i = 0; i < kickFrameCount; ++i)
        kickTextures[i].loadFromFile("assets/textures/monk/kick/kick" + std::to_string(i + 1) + ".png");
    for (int i = 0; i < crouchFrameCount; ++i)
        crouchTextures[i].loadFromFile("assets/textures/monk/crouch/crouch-kick" + std::to_string(i + 1) + ".png");
    for (int i = 0; i < flyFrameCount; ++i)
        flyTextures[i].loadFromFile("assets/textures/monk/fly/flying-kick" + std::to_string(i + 1) + ".png");

    sprite.setTexture(runTextures[0]);
    frameWidth = runTextures[0].getSize().x;
    frameHeight = runTextures[0].getSize().y;
    sprite.setScale(2.f, 2.f);
    sprite.setPosition(200.f, 400.f);

    hp = maxHp = 5;
    hpBack.setSize({ 100, 8 });
    hpBack.setFillColor(sf::Color(40, 40, 40));
    hpFront.setSize({ 100, 8 });
    hpFront.setFillColor(sf::Color::Green);
}

void Monk::update(float dt, float gravity, const sf::FloatRect& groundBounds) {
    if (hp <= 0) return;

    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool jump = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool attack = sf::Keyboard::isKeyPressed(sf::Keyboard::J);
    bool kick = sf::Keyboard::isKeyPressed(sf::Keyboard::K);
    bool crouch = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    bool fly = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);

    if (attackCooldownTimer > 0) attackCooldownTimer -= dt;
    if (invulTimer > 0) invulTimer -= dt;

    // движение
    if (attacking) velocity.x = 0;
    else {
        velocity.x = 0;
        if (left) { velocity.x = -moveSpeed; facingRight = false; movingRight = false; }
        if (right) { velocity.x = moveSpeed; facingRight = true; movingRight = true; }

        // прыжок
        if (onGround && jump) {
            velocity.y = -jumpStrength;
            onGround = false;
            setState(State::Jump);
        }

        // атака
        if (onGround && attack && attackCooldownTimer <= 0) {
            setState(State::Attack);
            attacking = true;
            hitActive = false;
            frame = 0;
            attackCooldownTimer = attackCooldown;
        }
        // kick
        if (onGround && kick && attackCooldownTimer <= 0) {
            setState(State::Kick);
            attacking = true;
            hitActive = false;
            frame = 0;
            attackCooldownTimer = attackCooldown;
        }
        // crouch_kick
        if (onGround && crouch && attackCooldownTimer <= 0) {
            setState(State::Crouch);
            attacking = true;
            hitActive = false;
            frame = 0;
            attackCooldownTimer = attackCooldown;
        }
        // fly_kick
        if (!onGround && fly && attackCooldownTimer <= 0) {
            setState(State::Fly);
            attacking = true;
            hitActive = false;
            frame = 0;
            attackCooldownTimer = attackCooldown;
        }
    }

    velocity.y += gravity * dt; sprite.move(velocity * dt);
    // ground check
    sf::FloatRect b = sprite.getGlobalBounds();
    if (b.intersects(groundBounds)) {
        sprite.setPosition(sprite.getPosition().x, groundBounds.top - b.height);
        velocity.y = 0; onGround = true;
    }
    else onGround = false;

    // --- —мена состо€ний ---
    if (onGround && !attacking) {
        if (std::abs(velocity.x) < 0.1f) setState(State::Idle);
        else setState(State::Run);
    }
    
    updateAnimation(dt);

    float sx = std::abs(sprite.getScale().x);
    sprite.setScale(facingRight ? sx : -sx, sprite.getScale().y);
    sprite.setOrigin(facingRight ? 0.f : frameWidth, 0.f);

    
    updateHpBar();
}

void Monk::updateAnimation(float dt) {
    frameTime += dt;
    if (frameTime < animSpeed) return;
    frameTime = 0;

    switch (currentState) {
    case State::Idle:
        frame = (frame + 1) % idleFrameCount;
        sprite.setTexture(idleTextures[frame]);
        break;

    case State::Run:
        frame = (frame + 1) % runFrameCount;
        sprite.setTexture(runTextures[frame]);
        break;

    case State::Jump:
        frame = std::min(frame + 1, jumpFrameCount - 1);
        sprite.setTexture(jumpTextures[frame]);
        break;

    case State::Attack:
        frame++;
        if (frame >= attackFrameCount) {
            attacking = false;
            hitActive = false;
            frame = 0;
            setState(onGround ? State::Idle : State::Jump);
        }
        else {
            sprite.setTexture(attackTextures[frame]);
            hitActive = (frame == attackFrameCount / 2);
        }
        break;
    case State::Kick:
        frame++;
        if (frame >= kickFrameCount) {
            attacking = false;
            hitActive = false;
            frame = 0;
            setState(onGround ? State::Idle : State::Jump);
        }
        else {
            sprite.setTexture(kickTextures[frame]);
            hitActive = (frame == kickFrameCount / 2);
        }
        break;
    case State::Crouch:
        frame++;
        if (frame >= crouchFrameCount) {
            attacking = false;
            hitActive = false;
            frame = 0;
            setState(onGround ? State::Idle : State::Jump);
        }
        else {
            sprite.setTexture(crouchTextures[frame]);
            hitActive = (frame == crouchFrameCount / 2);
        }
        break;
    case State::Fly:
        frame++;
        if (frame >= flyFrameCount) {
            attacking = false;
            hitActive = false;
            frame = 0;
            setState(onGround ? State::Idle : State::Jump);
        }
        else {
            sprite.setTexture(flyTextures[frame]);
            hitActive = (frame == flyFrameCount / 2);
        }
        break;
    }
}

void Monk::setState(State s) {
    if (currentState == s) return;
    currentState = s;
    frame = 0;
    frameTime = 0;
    hitActive = false;
}

// --- ”меньшенный хитбокс дл€ коллизий ---
sf::FloatRect Monk::getBounds() const {
    sf::FloatRect b = sprite.getGlobalBounds();
    float shrinkX = b.width * 0.35f;
    float shrinkY = b.height * 0.25f;
    b.left += shrinkX / 2.f;
    b.top += shrinkY / 2.f;
    b.width -= shrinkX;
    b.height -= shrinkY;
    return b;
}

sf::FloatRect Monk::getHitbox() const {
    if (!hitActive) return {};
    sf::FloatRect b = sprite.getGlobalBounds();
    float w = b.width * 0.6f;
    float h = b.height * 0.6f;
    float x = b.left + b.width * 0.2f;
    float y = b.top + b.height * 0.3f;
    return { x, y, w, h };
}

void Monk::takeDamage(int dmg) {
    if (invulTimer > 0) return;

    hp -= dmg;
    if (hp <= 0) hp = 0;
    invulTimer = invulDuration;

    // исправленный отскок Ч противоположно направлению взгл€да
    applyKnockback(600.f, facingRight);
}

void Monk::applyKnockback(float strength, bool fromRight) {
    velocity.x = fromRight ? -strength : strength;
    velocity.y = -strength * 0.35f;
    onGround = false;
}

void Monk::bounce(float s) {
    velocity.y = -s;
    onGround = false;
}

void Monk::updateHpBar() {
    float ratio = (float)hp / maxHp;
    hpFront.setSize({ 100 * ratio, 8 });
    sf::Vector2f pos(sprite.getPosition().x + 10, sprite.getPosition().y - 10);
    hpBack.setPosition(pos);
    hpFront.setPosition(pos);
}

void Monk::draw(sf::RenderWindow& w) {
    if (hp > 0) {
        w.draw(sprite);
        w.draw(hpBack);
        w.draw(hpFront);
    }
}

void Monk::resolveTileCollisionsX(const std::vector<sf::FloatRect>& tiles) {
    sf::FloatRect p = getBounds();
    for (const auto& tile : tiles) {
        if (p.intersects(tile)) {
            if (velocity.x > 0)
                sprite.setPosition(tile.left - p.width, sprite.getPosition().y);
            else if (velocity.x < 0)
                sprite.setPosition(tile.left + tile.width, sprite.getPosition().y);
            velocity.x = 0;
            break;
        }
    }
}

void Monk::resolveTileCollisionsY(const std::vector<sf::FloatRect>& tiles) {
    sf::FloatRect p = getBounds();
    onGround = false;

    for (const auto& tile : tiles) {
        if (p.intersects(tile)) {
            if (velocity.y > 0) {
                sprite.setPosition(sprite.getPosition().x, tile.top - p.height);
                onGround = true;
            }
            else if (velocity.y < 0) {
                sprite.setPosition(sprite.getPosition().x, tile.top + tile.height);
            }
            velocity.y = 0;
            break;
        }
    }
}

void Monk::heal(int amount) {
    hp = std::min(hp + amount, maxHp);
}

void Monk::handleTileCollisions(const std::vector<sf::FloatRect>& tiles)
{
    sf::FloatRect playerBox = getBounds();

    for (const auto& tile : tiles)
    {
        if (!playerBox.intersects(tile)) continue;

        float dxLeft = std::abs((playerBox.left + playerBox.width) - tile.left);
        float dxRight = std::abs(playerBox.left - (tile.left + tile.width));
        float dyTop = std::abs((playerBox.top + playerBox.height) - tile.top);
        float dyBottom = std::abs(playerBox.top - (tile.top + tile.height));

        if (std::min(dxLeft, dxRight) < std::min(dyTop, dyBottom))
        {
            // горизонтальный удар
            if (dxLeft < dxRight)
                sprite.move(-dxLeft, 0);
            else
                sprite.move(dxRight, 0);

            velocity.x = 0;
        }
        else
        {
            // вертикальный удар
            if (dyTop < dyBottom)
            {
                // приземление на тайл
                sprite.move(0, -dyTop);
                velocity.y = 0;
                onGround = true;
            }
            else
            {
                sprite.move(0, dyBottom);
                velocity.y = 0;
            }
        }

        playerBox = getBounds(); // обновить после перемещени€
    }
}