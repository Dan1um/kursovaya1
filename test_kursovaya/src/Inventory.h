#pragma once
#include <SFML/Graphics.hpp>

class Inventory {
public:
    void addSmall(int n) { small += n; }
    void addBig(int n) { big += n; }

    bool useSmall();
    bool useBig();
    void setView(const sf::View& view);
    void update(float dt);
    void draw(sf::RenderWindow& w);

private:
    int small = 0;
    int big = 0;

    float useCooldown = 0.3f;
    float timer = 0.f;

    sf::Sprite smallIcon;
    sf::Sprite bigIcon;
    sf::Texture smallTex;
    sf::Texture bigTex;

    sf::Font font;
    sf::Text smallText;
    sf::Text bigText;

    bool texturesLoaded = false;
    
    void loadTextures();
};
