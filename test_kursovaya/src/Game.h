#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "PlayerClass.h"
#include "Knight.h"
#include "Trooper.h"
#include "Monk.h"

#include "Croco.h"
#include "Spider.h"
#include "Town.h"
#include "Wizard.h"
#include "Fireball.h"
#include "Projectile.h"

#include "CoinsUI.h"

#include "Merchant.h"
#include "Plate.h"

#include "Inventory.h"

class Game {
public:
    Game(PlayerClass chosenClass);
    void run();
    Town town;

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
    Inventory inventory;
    CoinsUI coinsUI;

    float potionCooldown = 0.25f;
    float potionTimer = 0.f;

    // враги и уровень
    Croco croco;
    Spider spider;
    
    Wizard wizard;

    bool crocoRewarded = false;
    bool wizardRewarded = false;

    std::vector<Plate> plates;
    Merchant merchant;

    int coins = 0;

    std::vector<Fireball> fireballs;
    std::vector<Bullet> bullets;

    sf::FloatRect groundBounds;
    float gravity = 900.f;
};
