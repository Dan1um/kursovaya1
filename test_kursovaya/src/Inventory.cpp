#include "Inventory.h"
#include <SFML/Window/Keyboard.hpp>

void Inventory::loadTextures() {
    if (texturesLoaded) return;

    smallTex.loadFromFile("assets/textures/shop/icons/common_heal.png");
    bigTex.loadFromFile("assets/textures/shop/icons/big_heal.png");

    smallIcon.setTexture(smallTex);
    bigIcon.setTexture(bigTex);

    smallIcon.setScale(2.f, 2.f);
    bigIcon.setScale(2.f, 2.f);

    font.loadFromFile("assets/fonts/arial.ttf");

    texturesLoaded = true;

    smallText.setFont(font);
    bigText.setFont(font);
    smallText.setCharacterSize(16);
    bigText.setCharacterSize(16);
    smallText.setFillColor(sf::Color::White);
    bigText.setFillColor(sf::Color::White);
}

bool Inventory::useSmall() {
    if (timer > 0 || small <= 0) return false;
    small--;
    timer = useCooldown;
    return true;
}

bool Inventory::useBig() {
    if (timer > 0 || big <= 0) return false;
    big--;
    timer = useCooldown;
    return true;
}

void Inventory::update(float dt) {
    if (timer > 0) timer -= dt;

    smallText.setString("x" + std::to_string(small));
    bigText.setString("x" + std::to_string(big));
}

void Inventory::setView(const sf::View& view) {
    sf::Vector2f topLeft = view.getCenter() - view.getSize() / 2.f;

    smallIcon.setPosition(topLeft.x + 20.f, topLeft.y + 20.f);
    bigIcon.setPosition(topLeft.x + 20.f, topLeft.y + 80.f);

    smallText.setPosition(topLeft.x + 65.f, topLeft.y + 22.f);
    bigText.setPosition(topLeft.x + 65.f, topLeft.y + 82.f);
}


void Inventory::draw(sf::RenderWindow& window) {
    loadTextures();

    window.draw(smallIcon);
    window.draw(bigIcon);
    smallText.setString("x" + std::to_string(small));
    bigText.setString("x" + std::to_string(big));

    window.draw(smallText);
    window.draw(bigText);
}
