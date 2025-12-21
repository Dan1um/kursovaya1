#include "Plate.h"

Plate::Plate(sf::Vector2f pos, PlateReward r, int amount)
    : reward(r), rewardAmount(amount)
{
    switch (reward) {
    case PlateReward::HealSmall: price = 3; usesLeft = 3; break;
    case PlateReward::HealBig:   price = 5; usesLeft = 3; break;
    case PlateReward::DoubleJump: price = 1; usesLeft = 1; break;
    case PlateReward::Dash:       price = 1; usesLeft = 1; break;
    }

    shape.setSize({ 64.f, 16.f });
    shape.setFillColor(sf::Color(120, 120, 120));
    shape.setPosition(pos);

    progressBar.setSize({ 0.f, 6.f });
    progressBar.setFillColor(sf::Color::Green);
    progressBar.setPosition(pos.x, pos.y - 10.f);

    smallTex.loadFromFile("assets/textures/shop/icons/common_heal.png");
    bigTex.loadFromFile("assets/textures/shop/icons/big_heal.png");

    smallIcon.setTexture(smallTex);
    bigIcon.setTexture(bigTex);

    smallIcon.setScale(1.f, 1.f);
    bigIcon.setScale(1.f, 1.f);

    smallIcon.setPosition(275.f, 625.f);
    bigIcon.setPosition(355.f, 625.f);

}

void Plate::update(float dt, const sf::FloatRect& playerBounds) {
    if (consumed) return;

    if (shape.getGlobalBounds().intersects(playerBounds)) {
        holdTime += dt;
        float ratio = std::min(holdTime / requiredTime, 1.f);
        progressBar.setSize({ 64.f * ratio, 6.f });

        if (holdTime >= requiredTime) {
            activated = true;
        }
    }
    else {
        holdTime = 0.f;
        progressBar.setSize({ 0.f, 6.f });
        activated = false;
    }
}

void Plate::resetActivation() {
    activated = false;
    holdTime = 0.f;
    progressBar.setSize({ 0.f, 6.f });
}


int Plate::consume() {
    if (consumed || usesLeft <= 0)
        return 0;

    usesLeft--;

    if (usesLeft <= 0)
        consumed = true;

    activated = false;
    holdTime = 0.f;
    progressBar.setSize({ 0.f, 6.f });

    return rewardAmount;
}


bool Plate::canBuy(int coins) const{
    return coins >= price && !consumed;
}

void Plate::draw(sf::RenderWindow& w) {
    if (!consumed) {
        w.draw(shape);
        w.draw(progressBar);
    }
    if (!consumed) {
        w.draw(shape);
        w.draw(progressBar);

        w.draw(smallIcon);
        w.draw(bigIcon);
    }

}
