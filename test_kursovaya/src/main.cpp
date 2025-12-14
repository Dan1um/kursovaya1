#include <SFML/Graphics.hpp>
#include "Game.h"
#include "PlayerClass.h"

int main() {
    sf::RenderWindow menu(sf::VideoMode(600, 400), "Choose Class");
    menu.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("assets/fonts/arial.ttf");

    // ----------  ÕŒœ » ----------
    sf::RectangleShape knightBtn({ 200, 60 });
    sf::RectangleShape trooperBtn({ 200, 60 });
    sf::RectangleShape monkBtn({ 200, 60 });

    knightBtn.setPosition(200, 80);
    trooperBtn.setPosition(200, 160);
    monkBtn.setPosition(200, 240);

    knightBtn.setFillColor(sf::Color(100, 100, 200));
    trooperBtn.setFillColor(sf::Color(100, 200, 100));
    monkBtn.setFillColor(sf::Color(200, 100, 100));

    sf::Text knightText("Knight", font, 24);
    sf::Text trooperText("Trooper", font, 24);
    sf::Text monkText("Monk", font, 24);

    knightText.setPosition(260, 95);
    trooperText.setPosition(250, 175);
    monkText.setPosition(265, 255);

    PlayerClass chosenClass;
    bool selected = false;

    // ---------- ÷» À Ã≈Õﬁ ----------
    while (menu.isOpen()) {
        sf::Event e;
        while (menu.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                menu.close();

            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2f mouse(
                    (float)e.mouseButton.x,
                    (float)e.mouseButton.y
                );

                if (knightBtn.getGlobalBounds().contains(mouse)) {
                    chosenClass = PlayerClass::Knight;
                    selected = true;
                }
                else if (trooperBtn.getGlobalBounds().contains(mouse)) {
                    chosenClass = PlayerClass::Trooper;
                    selected = true;
                }
                else if (monkBtn.getGlobalBounds().contains(mouse)) {
                    chosenClass = PlayerClass::Monk;
                    selected = true;
                }

                if (selected)
                    menu.close();
            }
        }

        menu.clear(sf::Color(30, 30, 30));
        menu.draw(knightBtn);
        menu.draw(trooperBtn);
        menu.draw(monkBtn);
        menu.draw(knightText);
        menu.draw(trooperText);
        menu.draw(monkText);
        menu.display();
    }

    // ---------- «¿œ”—  »√–€ ----------
    if (selected) {
        Game game(chosenClass);
        game.run();
    }

    return 0;
}
