#include "Game.h"
#include <iostream>

Game::Game(PlayerClass chosenClass)
    : window(sf::VideoMode(1280, 720), "Platformer")
    , playerClass(chosenClass)
    , merchant(sf::Vector2f(200.f, 640.f - 64.f))
    , croco(sf::Vector2f(900.f, 640.f - 64.f))
    , spider(sf::Vector2f(1400.f, 640.f - 50.f))
    , wizard(sf::Vector2f(2200.f, 640.f - 135.f))
    , town(1280.f, 720.f)

{
    plates.emplace_back(sf::Vector2f(260.f, 625.f), PlateReward::HealSmall);
    plates.emplace_back(sf::Vector2f(340.f, 625.f), PlateReward::HealBig);
    plates.emplace_back(sf::Vector2f(420.f, 625.f), PlateReward::DoubleJump);
    plates.emplace_back(sf::Vector2f(500.f, 625.f), PlateReward::Dash);
    window.setFramerateLimit(60);
    camera = window.getDefaultView();
    groundBounds = town.getGroundBounds();
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

    inventory.update(dt);
    coinsUI.update(dt);
    coinsUI.setView(camera);
    potionTimer -= dt;
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
    spider.update(dt, groundBounds, playerPos);
    wizard.update(dt, playerPos, fireballs);

    // ================= KNIGHT MELEE DAMAGE =================
    if (playerClass == PlayerClass::Knight && knight.isHitActive()) {
        sf::FloatRect hit = knight.getHitbox();

        if (croco.isAlive() && hit.intersects(croco.getBounds()))
            croco.takeDamage(knight.getAttackDamage());

        if (!wizard.isDead() && hit.intersects(wizard.getBounds()))
            wizard.takeDamage(knight.getAttackDamage());
    }
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

    // ================= spider =================
    if (spider.isAlive() && playerBounds.intersects(spider.getBounds())) {
        bool fromTop = (playerBounds.top + playerBounds.height * 0.6f < spider.getBounds().top);

        if (fromTop && playerVel.y > 100.f) {
            spider.takeDamage(1);
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
    camCenter.y = camera.getCenter().y; // Y фиксируем

    float halfW = camera.getSize().x * 0.5f;

    camCenter.x = std::clamp(
        playerPos.x,
        halfW,
        town.getLevelWidth() - halfW
    );

    camera.setCenter(camCenter);
    window.setView(camera);




    for (auto& plate : plates) {
        sf::FloatRect plateBounds =
            (playerClass == PlayerClass::Monk)
            ? monk.getFullBounds()
            : playerBounds;

        plate.update(dt, plateBounds);

        if (!plate.isActivated() || plate.isConsumed())
            continue;

        // ? проверка денег
        if (!coinsUI.spend(plate.getPrice())) {
            coinsUI.flashRed(); // подсветка текста
            plate.resetActivation();
            break;
        }

        // ? списываем деньги ТОЛЬКО через CoinsUI
        coins -= plate.getPrice();
        coinsUI.set(coins);

        int amount = plate.consume();

        switch (plate.getReward()) {
        case PlateReward::HealSmall:
            inventory.addSmall(amount);
            break;

        case PlateReward::HealBig:
            inventory.addBig(amount);
            break;

        case PlateReward::DoubleJump:
            if (playerClass == PlayerClass::Knight) knight.unlockDoubleJump();
            if (playerClass == PlayerClass::Monk) monk.unlockDoubleJump();
            if (playerClass == PlayerClass::Trooper) trooper.unlockDoubleJump();
            break;

        case PlateReward::Dash:
            if (playerClass == PlayerClass::Knight) knight.unlockDash();
            if (playerClass == PlayerClass::Monk) monk.unlockDash();
            if (playerClass == PlayerClass::Trooper) trooper.unlockDash();
            break;
        }

        plate.resetActivation();
        break;

        town.update(camera.getCenter());
    }



    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && potionTimer <= 0.f) {
        if (inventory.useSmall()) {
            if (playerClass == PlayerClass::Knight) knight.heal(1);
            if (playerClass == PlayerClass::Monk) monk.heal(1);
            if (playerClass == PlayerClass::Trooper) trooper.heal(1);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && potionTimer <= 0.f) {
        if (inventory.useBig()) {
            if (playerClass == PlayerClass::Knight) knight.heal(2);
            if (playerClass == PlayerClass::Monk) monk.heal(2);
            if (playerClass == PlayerClass::Trooper) trooper.heal(2);
        }
    }

    //merchant
    merchant.update(dt);

    if (!croco.isAlive() && !crocoRewarded) {
        coins += 4;
        coinsUI.add(4);
        crocoRewarded = true;
    }

    if (wizard.isDead() && !wizardRewarded) {
        coins += 3;
        coinsUI.add(3);
        wizardRewarded = true;
    }

}

void Game::render() {
    window.clear(sf::Color(70, 180, 255));
    window.setView(camera);

    town.draw(window);
    spider.draw(window);
    croco.draw(window);
    inventory.setView(camera);
    inventory.draw(window);
    coinsUI.draw(window);

    switch (playerClass) {
    case PlayerClass::Knight: knight.draw(window); break;
    case PlayerClass::Trooper: trooper.draw(window); break;
    case PlayerClass::Monk: monk.draw(window); break;
    }

    merchant.draw(window);
    for (auto& plate : plates)
        plate.draw(window);

    wizard.draw(window);

    for (auto& f : fireballs) f.draw(window);
    for (auto& b : bullets) b.draw(window);

    window.display();
}
