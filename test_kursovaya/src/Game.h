#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "PlayerClass.h"
#include "Knight.h"
#include "Trooper.h"
#include "Monk.h"

#include "Croco.h"
#include "Worm.h"
#include "Swamp.h"
#include "Wizard.h"
#include "Fireball.h"
#include "Projectile.h"

class Game {
public:
    Game(PlayerClass chosenClass);
    void run();

private:
    void processEvent();
    void update(float dt);
    void render();

    sf::RenderWindow window;
    sf::View camera;

    PlayerClass playerClass;

    // игроки (используется только один)
    Knight knight;
    Trooper trooper;
    Monk monk;

    // враги и уровень
    Croco croco;
    Worm worm;
    Swamp swamp;
    Wizard wizard;

    std::vector<Fireball> fireballs;
    std::vector<Bullet> bullets;

    sf::FloatRect groundBounds;
    float gravity = 900.f;
};
