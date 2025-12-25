#include <SFML/Graphics.hpp>
#include "Game.h"
#include "PlayerClass.h"

int main() {
    sf::RenderWindow menu(sf::VideoMode(600, 400), "Choose Class");
    menu.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("assets/fonts/arial.ttf");

    // ---------- ‘ŒÕ ----------
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("assets/textures/title-screen.png")) {
        return -1;
    }

    sf::Sprite bgSprite(bgTexture);

    float scaleX = 600.f / bgTexture.getSize().x;
    float scaleY = 400.f / bgTexture.getSize().y;
    bgSprite.setScale(scaleX, scaleY);

    // ----------  ÕŒœ » ----------
    sf::Vector2f btnSize(160.f, 40.f);

    sf::RectangleShape knightBtn(btnSize);
    sf::RectangleShape trooperBtn(btnSize);
    sf::RectangleShape monkBtn(btnSize);

    float centerX = 600.f / 2.f - btnSize.x / 2.f;
    float startY = 260.f;      // ÌËÊÂ ˆÂÌÚ‡ ˝Í‡Ì‡
    float spacing = 50.f;

    knightBtn.setPosition(centerX, startY);
    trooperBtn.setPosition(centerX, startY + spacing);
    monkBtn.setPosition(centerX, startY + spacing * 2);

    knightBtn.setFillColor(sf::Color(100, 100, 200));
    trooperBtn.setFillColor(sf::Color(100, 200, 100));
    monkBtn.setFillColor(sf::Color(200, 100, 100));

    sf::Text knightText("Knight", font, 18);
    sf::Text trooperText("Trooper", font, 18);
    sf::Text monkText("Monk", font, 18);

    auto centerText = [](sf::Text& text, sf::RectangleShape& btn) {
        sf::FloatRect t = text.getLocalBounds();
        text.setOrigin(t.left + t.width / 2.f, t.top + t.height / 2.f);
        text.setPosition(
            btn.getPosition().x + btn.getSize().x / 2.f,
            btn.getPosition().y + btn.getSize().y / 2.f
        );
    };

    centerText(knightText, knightBtn);
    centerText(trooperText, trooperBtn);
    centerText(monkText, monkBtn);

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

        menu.clear();
        menu.draw(bgSprite);
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
