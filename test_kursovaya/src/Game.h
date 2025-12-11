#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Croco.h"
#include "Swamp.h"
#include "Worm.h"
#include "Wizard.h"
#include "Fireball.h"
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
	Swamp swamp;
	Wizard wizard;
	std::vector<Fireball> fireballs;
	sf::FloatRect groundBounds;
	float gravity = 900.f;
};