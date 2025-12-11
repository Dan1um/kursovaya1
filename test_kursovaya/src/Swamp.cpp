#include "Swamp.h"
#include <iostream>

Swamp::Swamp(float levelWidth, float windowWidth, float windowHeight)
    : levelWidth(levelWidth), windowWidth(windowWidth), windowHeight(windowHeight)
{
    // === ‘ќЌ ===
    if (!backgroundTexture.loadFromFile("D:/vs projects/test_kursovaya/test_kursovaya/assets/textures/swamp_background.png"))
        std::cerr << "Failed to load background.png\n";

    background.setTexture(backgroundTexture);
    // раст€гиваем фон под размер окна
    float scaleX = windowWidth / backgroundTexture.getSize().x;
    float scaleY = windowHeight / backgroundTexture.getSize().y;
    background.setScale(scaleX, scaleY);

    // === “ј…Ћџ ===
    if (!tilesetTexture.loadFromFile("D:/vs projects/test_kursovaya/test_kursovaya/assets/textures/tileset-swamp.png"))
        std::cerr << "Failed to load tileset-sliced.png\n";

    loadTiles();

    // === ƒ≈–≈¬№я ===
    if (!treeTexture.loadFromFile("D:/vs projects/test_kursovaya/test_kursovaya/assets/textures/tree-3.png"))
        std::cerr << "Failed to load tree-3.png\n";

    sf::Sprite tree;
    tree.setTexture(treeTexture);
    tree.setScale(2.2f, 2.2f);
    tree.setPosition(600.f, 400.f);
    trees.push_back(tree);

    sf::Sprite tree2 = tree;
    tree2.setPosition(1600.f, 420.f);
    trees.push_back(tree2);

    // === «≈ћЋя ===
    groundBounds = { 0.f, 640.f, levelWidth, 64.f };
}

void Swamp::loadTiles() {
    const int tileWidth = 64;
    const int tileHeight = 32;
    const int scale = 2;
    const int groundY = 640;

    sf::Sprite tile;
    tile.setTexture(tilesetTexture);
    tile.setTextureRect({ 0, 0, tileWidth, tileHeight });
    tile.setScale(scale, scale);

    const int tilesCount = static_cast<int>(levelWidth / (tileWidth * scale));
    for (int i = 0; i < tilesCount; ++i) {
        tile.setPosition(i * tileWidth * scale, groundY);
        tiles.push_back(tile);
    }
}

void Swamp::update(const sf::Vector2f& playerPos) {
    // ‘он теперь центрируетс€ относительно игрока и не уходит влево
    float bgX = playerPos.x - windowWidth / 2.f;
    if (bgX < 0) bgX = 0;
    if (bgX > levelWidth - windowWidth) bgX = levelWidth - windowWidth;
    background.setPosition(bgX, 0.f);
}

void Swamp::draw(sf::RenderWindow& window) {
    // ‘он
    window.draw(background);

    // ƒеревь€
    for (auto& tree : trees)
        window.draw(tree);

    // «емл€
    for (auto& tile : tiles)
        window.draw(tile);
}
