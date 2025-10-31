#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Croco.h"
#include "Worm.h"
#include <vector>

class Game {
public:
    Game();
    void run();
private:
    void processEvent();
    void update(float dt);
    void render();

    sf::RenderWindow window;
    sf::View camera;
    sf::RectangleShape ground;
    Player player;
    Croco croco;
    Worm worm;
    sf::FloatRect groundBounds;
    float gravity = 900.f;
};
