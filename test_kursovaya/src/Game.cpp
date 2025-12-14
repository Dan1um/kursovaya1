#include "Game.h"
#include <iostream>

Game::Game(PlayerClass chosenClass)
    : window(sf::VideoMode(1280, 720), "Platformer")
    , playerClass(chosenClass)
    , croco(sf::Vector2f(800.f, 400.f))
    , worm(sf::Vector2f(500.f, 575.f))
    , swamp(5000.f, 1280.f, 720.f)
    , wizard(sf::Vector2f(1200.f, 520.f))
{
    window.setFramerateLimit(60);
    camera = window.getDefaultView();
    groundBounds = swamp.getGroundBounds();
    wizard.setFacingRight(false);
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        processEvent();
        float dt = clock.restart().asSeconds();
        update(dt);
        render();
    }
}

void Game::processEvent() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update(float dt) {

    // ================= PLAYER UPDATE =================
    sf::Vector2f playerPos;
    sf::FloatRect playerBounds;
    sf::Vector2f playerVel;
    bool playerAlive = true;

    switch (playerClass) {
    case PlayerClass::Knight:
        playerAlive = knight.isAlive();
        if (!playerAlive) break;
        knight.update(dt, gravity, groundBounds);
        playerPos = knight.getPosition();
        playerBounds = knight.getBounds();
        playerVel = knight.getVelocity();
        break;

    case PlayerClass::Trooper:
        playerAlive = trooper.isAlive();
        if (!playerAlive) break;
        trooper.update(dt, gravity, groundBounds, bullets);
        playerPos = trooper.getPosition();
        playerBounds = trooper.getBounds();
        playerVel = trooper.getVelocity();
        break;

    case PlayerClass::Monk:
        playerAlive = monk.isAlive();
        if (!playerAlive) break;
        monk.update(dt, gravity, groundBounds);
        playerPos = monk.getPosition();
        playerBounds = monk.getBounds();
        playerVel = monk.getVelocity();
        break;
    }

    if (!playerAlive) {
        std::cout << "Player died!\n";
        return;
    }

    // ================= ENEMIES =================
    croco.update(dt, playerPos, groundBounds);
    worm.update(dt, groundBounds);
    wizard.update(dt, playerPos, fireballs);

    // ================= KNIGHT MELEE DAMAGE =================
    if (playerClass == PlayerClass::Knight && knight.isHitActive()) {
        sf::FloatRect hit = knight.getHitbox();

        if (croco.isAlive() && hit.intersects(croco.getBounds()))
            croco.takeDamage(knight.getAttackDamage());

        if (!wizard.isDead() && hit.intersects(wizard.getBounds()))
            wizard.takeDamage(knight.getAttackDamage());
    }

    // ================= MONK MELEE DAMAGE =================
    if (playerClass == PlayerClass::Monk && monk.isHitActive()) {
        sf::FloatRect hit = monk.getHitbox();

        if (croco.isAlive() && hit.intersects(croco.getBounds()))
            croco.takeDamage(monk.getAttackDamage());

        if (!wizard.isDead() && hit.intersects(wizard.getBounds()))
            wizard.takeDamage(monk.getAttackDamage());
    }

    // ================= CROCO DAMAGE =================
    if (croco.isAlive() && croco.isHitActive()) {
        if (playerBounds.intersects(croco.getHitbox())) {
            if (playerClass == PlayerClass::Knight) knight.takeDamage(croco.getAttackDamage());
            if (playerClass == PlayerClass::Trooper) trooper.takeDamage(croco.getAttackDamage());
            if (playerClass == PlayerClass::Monk) monk.takeDamage(croco.getAttackDamage());
        }
    }

    // ================= WORM =================
    if (worm.isAlive() && playerBounds.intersects(worm.getBounds())) {
        bool fromTop = (playerBounds.top + playerBounds.height * 0.6f < worm.getBounds().top);

        if (fromTop && playerVel.y > 100.f) {
            worm.takeDamage(1);
            if (playerClass == PlayerClass::Knight) knight.bounce(320.f);
            if (playerClass == PlayerClass::Trooper) trooper.bounce(320.f);
            if (playerClass == PlayerClass::Monk) monk.bounce(320.f);
        }
        else {
            if (playerClass == PlayerClass::Knight) knight.takeDamage(1);
            if (playerClass == PlayerClass::Trooper) trooper.takeDamage(1);
            if (playerClass == PlayerClass::Monk) monk.takeDamage(1);
        }
    }

    // ================= CAMERA =================
    sf::Vector2f camCenter = playerPos;
    camCenter.y = 360.f;
    camCenter.x = std::clamp(camCenter.x, 640.f, 5000.f - 640.f);
    camera.setCenter(camCenter);

    swamp.update(playerPos);

    // ================= FIREBALLS =================
    for (auto& f : fireballs) f.update(dt);
    for (auto& f : fireballs) {
        if (!f.isAlive()) continue;
        if (f.getBounds().intersects(playerBounds)) {
            if (playerClass == PlayerClass::Knight) knight.takeDamage(1);
            if (playerClass == PlayerClass::Trooper) trooper.takeDamage(1);
            if (playerClass == PlayerClass::Monk) monk.takeDamage(1);
        }
    }

    // ================= BULLETS =================
    for (auto& b : bullets) b.update(dt);
    for (auto& b : bullets) {
        if (!b.isAlive()) continue;

        if (croco.isAlive() && b.getBounds().intersects(croco.getBounds())) {
            croco.takeDamage(1);
            b.kill();
            continue;
        }

        if (!wizard.isDead() && b.getBounds().intersects(wizard.getBounds())) {
            wizard.takeDamage(1);
            b.kill();
        }
    }
}

void Game::render() {
    window.clear(sf::Color(70, 180, 255));
    window.setView(camera);

    swamp.draw(window);
    worm.draw(window);
    croco.draw(window);

    switch (playerClass) {
    case PlayerClass::Knight: knight.draw(window); break;
    case PlayerClass::Trooper: trooper.draw(window); break;
    case PlayerClass::Monk: monk.draw(window); break;
    }

    wizard.draw(window);

    for (auto& f : fireballs) f.draw(window);
    for (auto& b : bullets) b.draw(window);

    window.display();
}
