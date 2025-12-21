#pragma once
#include <SFML/Graphics.hpp>

enum class PlateReward {
    HealSmall,
    HealBig,
    DoubleJump,
    Dash
};

class Plate {
public:
    Plate(sf::Vector2f pos, PlateReward r, int amount = 1);

    void update(float dt, const sf::FloatRect& playerBounds);
    void draw(sf::RenderWindow& w);
    int getPrice() const { return price; }
    bool isActivated() const { return activated; }
    bool isConsumed() const { return consumed; }
    PlateReward getReward() const { return reward; }
    bool canBuy(int coins) const;
    int consume();
    void resetActivation();

private:
    sf::RectangleShape shape;
    sf::RectangleShape progressBar;

    sf::Sprite icon;
    PlateReward reward;
    float holdTime = 0.f;
    float requiredTime = 1.2f;

    sf::Sprite smallIcon;
    sf::Sprite bigIcon;
    sf::Texture smallTex;
    sf::Texture bigTex;

    int usesLeft;
    int price;

    bool activated = false;
    bool consumed = false;

    int rewardAmount;
};

