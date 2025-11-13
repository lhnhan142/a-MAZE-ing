#pragma once

#include <SFML/Graphics.hpp>

class Player {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
    float speed;

    // THAY ĐỔI: Sử dụng Vector2i cho kích thước frame (ví dụ: 32x48)
    sf::Vector2i frameSize;

    // Biến mới cho animation
    sf::Clock animationClock;
    float animationSpeed;
    int numFrames;      // Số frame mỗi hàng (sẽ là 4)
    int currentFrame;   // Frame hiện tại (0-3)
    int spriteRow;      // Hàng animation (0=Down, 1=Up, 2=Right, 3=Left)

    // THAY ĐỔI: Constructor đã được cập nhật
    Player(const std::string& textureFile, sf::Vector2f startPos, float cellSize, sf::Vector2i sSize, int fCount);

    // Các hàm này giữ nguyên
    sf::Vector2f handleInput(float delta);
    void updatePosition(sf::Vector2f newPos);
    void reset(sf::Vector2f startPos);
    sf::Vector2i getCellPosition(int cellSize);
    void draw(sf::RenderWindow& window);
};