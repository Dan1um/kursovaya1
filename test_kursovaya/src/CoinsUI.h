#pragma once
#include <SFML/Graphics.hpp>

class CoinsUI {
public:
    void add(int n);
    bool canSpend(int price) const;
    bool spend(int price);
    void set(int value);

    void update(float dt);
    void setView(const sf::View& view);
    void draw(sf::RenderWindow& w);

    void flashRed();
private:
    int coins = 0;

    sf::Texture frames[9];
    sf::Sprite sprite;

    sf::Font font;
    sf::Text text;

    int frame = 0;
    float animTimer = 0.f;
    float animSpeed = 0.08f;

    float flashTimer = 0.f;
    sf::Color normalColor = sf::Color::White;
    sf::Color flashColor = sf::Color::Red;

    void load();
};

