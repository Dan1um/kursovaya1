#include "Game.h"
#include <iostream>
Game::Game() : window(sf::VideoMode(1280, 720), "Platformer")
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
	while (window.isOpen()){
		processEvent();
		float dt = clock.restart().asSeconds();
		update(dt); render();
	}
}
void Game::processEvent() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)\
			window.close();
	}
}
void Game::update(float dt) {
	if (!player.isAlive()) return; // === Обновляем игрока и врагов ===
	player.update(dt, gravity, groundBounds);
	croco.update(dt, player.getPosition(), groundBounds);
	worm.update(dt, groundBounds);
	wizard.update(dt, player.getPosition(), fireballs);
	// ===============================
	// // АТАКА ИГРОКА ПО ВРАГАМ //
	// ===============================
	if (player.isHitActive()){
		sf::FloatRect hit = player.getHitbox();
		// по крокодилу
		if (croco.isAlive() && hit.intersects(croco.getBounds())) {
			croco.takeDamage(player.getAttackDamage());
		}
		// по магу
		if (!wizard.isDead() && hit.intersects(wizard.getBounds())) {
			wizard.takeDamage(player.getAttackDamage());
		}
	}
	// === Урон от крокодила ===
	if (croco.isAlive() && croco.isHitActive()) {
		if (player.getBounds().intersects(croco.getHitbox())) {
			player.takeDamage(croco.getAttackDamage());
		}
	}
	// === Урон / смерть от червя ===
	if (worm.isAlive() && player.getBounds().intersects(worm.getBounds())) {
		sf::FloatRect pB = player.getBounds();
		sf::FloatRect wB = worm.getBounds();
		bool fromTop = (pB.top + pB.height * 0.6f < wB.top);
		// усиленная проверка
		if (fromTop && player.getVelocity().y > 100.f) {
			// теперь урон не пройдёт при явном прыжке
			worm.takeDamage(1);
			player.bounce(320.f);
		}
		else {
			player.takeDamage(1);
		}
	}
	// === Камера следует за игроком ===
	sf::Vector2f camCenter = player.getPosition();
	camCenter.y = 360.f;
	// фиксируем по вертикали
	if (camCenter.x < 640.f) camCenter.x = 640.f;
	if (camCenter.x > 5000.f - 640.f) camCenter.x = 5000.f - 640.f;
	camera.setCenter(camCenter);
	// === Обновляем уровень ===
	swamp.update(player.getPosition());
	// === Смерть игрока ===
	if (!player.isAlive()) {
		std::cout << "Player died!\n";
	}
	for (auto& f : fireballs) f.update(dt);
	// Проверка попадания фаерболла в игрока
	for (auto& f : fireballs) {
		if (f.isAlive() && f.getBounds().intersects(player.getBounds())) {
			player.takeDamage(1);
		}
	}
}
void Game::render(){
	window.clear(sf::Color(70, 180, 255));
	window.setView(camera);
	swamp.draw(window);
	worm.draw(window);
	croco.draw(window);
	player.draw(window);
	wizard.draw(window);
	for (auto& f : fireballs) f.draw(window);
	window.display();
}