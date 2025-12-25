#include "CoinsUI.h"

void CoinsUI::load() {
    static bool loaded = false;
    if (loaded) return;

    for (int i = 0; i < 9; ++i)
        frames[i].loadFromFile(
            "assets/textures/shop/icons/coin/goldCoin" + std::to_string(i+1) + ".png"
        );

    sprite.setTexture(frames[0]);
    sprite.setScale(2.f, 2.f);

    font.loadFromFile("assets/fonts/arial.ttf");
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);

    loaded = true;
}

void CoinsUI::add(int n) {
    coins += n;
}

void CoinsUI::set(int n) {
    coins = n;
}

bool CoinsUI::spend(int price) {
    if (coins < price) {
        flashRed();
        return false;
    }
    coins -= price;
    return true;
}

void CoinsUI::flashRed() {
    flashTimer = 0.3f;
}

void CoinsUI::update(float dt) {
    animTimer += dt;
    if (animTimer >= animSpeed) {
        animTimer = 0.f;
        frame = (frame + 1) % 9;
        sprite.setTexture(frames[frame]);
    }
    // таймер подсветки
    if (flashTimer > 0.f) {
        flashTimer -= dt;
        text.setFillColor(flashColor);
    }
    else {
        text.setFillColor(normalColor);
    }

    text.setString("x" + std::to_string(coins));
}

void CoinsUI::setView(const sf::View& view) {
    sf::Vector2f tl = view.getCenter() - view.getSize() / 2.f;
    sprite.setPosition(tl.x + 20.f, tl.y + 150.f);
    text.setPosition(tl.x + 70.f, tl.y + 155.f);
}

void CoinsUI::draw(sf::RenderWindow& w) {
    load();
    w.draw(sprite);
    w.draw(text);
}
