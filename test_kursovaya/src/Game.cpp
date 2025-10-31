#include "Game.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode(1280, 720), "Platformer")
    , croco(sf::Vector2f(800.f, 400.f))
    , worm(sf::Vector2f(500.f, 575.f))
{
    window.setFramerateLimit(60);
    camera = window.getDefaultView();

    ground.setSize({ 1280.f, 50.f });
    ground.setPosition(0.f, 670.f);
    ground.setFillColor(sf::Color(120, 80, 40));

    groundBounds = ground.getGlobalBounds();
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
    if (!player.isAlive()) return;

    player.update(dt, gravity, groundBounds);
    croco.update(dt, player.getPosition(), groundBounds);
    worm.update(dt, groundBounds);

    // ===============================
    //   ”–ŒÕ Œ“  –Œ Œƒ»À¿
    // ===============================
    if (croco.isAlive() && croco.isHitActive()) {
        if (player.getBounds().intersects(croco.getHitbox())) {
            player.takeDamage(croco.getAttackDamage());
        }
    }

    // ===============================
    //   ”–ŒÕ Œ“ ◊≈–¬ﬂ / —Ã≈–“‹ Œ“ œ–€∆ ¿
    // ===============================
    if (worm.isAlive() && player.getBounds().intersects(worm.getBounds())) {
        sf::FloatRect pB = player.getBounds();
        sf::FloatRect wB = worm.getBounds();

        bool fromTop = (pB.top + pB.height * 0.7f < wB.top);
        if (fromTop && player.getVelocity().y > 0) {
            worm.takeDamage(1);
            player.bounce(300.f);
        }
        else {
            player.takeDamage(1);
        }
    }

    // ===============================
    //   ÀŒ√» ¿ —Ã≈–“» œ≈–—ŒÕ¿∆¿
    // ===============================
    if (!player.isAlive()) {
        std::cout << "Player died!\n";
    }
}

void Game::render() {
    window.clear(sf::Color(70, 180, 255));
    window.setView(camera);

    window.draw(ground);
    worm.draw(window);
    croco.draw(window);
    player.draw(window);

    window.display();
}
