#include "Player.h"
#include <iostream> 

Player::Player(const std::string& textureFile, sf::Vector2f startPos, float cellSize, sf::Vector2i sSize, int fCount)
    : speed(150.f),
    frameSize(sSize),
    numFrames(fCount),      // numFrames giờ là 9 (từ GameManager.cpp)
    position(startPos),
    animationSpeed(0.1f), 
    currentFrame(0),
    spriteRow(2)            // Bắt đầu ở hàng 2 (hướng Xuống)
{
    if (!texture.loadFromFile(textureFile)) {
        std::cout << "Loi: Khong the tai texture file: " << textureFile << std::endl;
    }
    sprite.setTexture(texture);

    // Bắt đầu ở hàng 2 (Xuống), cột 0
    sprite.setTextureRect(sf::IntRect(0, frameSize.y * 2, frameSize.x, frameSize.y));

    // Scale sprite để vừa với cellSize (tính theo chiều rộng)
    sprite.setScale(cellSize / (float)frameSize.x, cellSize / (float)frameSize.x);

    sprite.setPosition(position);
}

sf::Vector2f Player::handleInput(float delta) {
    sf::Vector2f newPos = position;
    bool isMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { // Đi Lên
        newPos.y -= speed * delta;
        spriteRow = 0; // Hàng 0
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { // Đi Xuống
        newPos.y += speed * delta;
        spriteRow = 2; // Hàng 2
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { // Đi Trái
        newPos.x -= speed * delta;
        spriteRow = 1; // Hàng 1
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { // Đi Phải
        newPos.x += speed * delta;
        spriteRow = 3; // Hàng 3
        isMoving = true;
    }

    if (isMoving) {
        if (animationClock.getElapsedTime().asSeconds() > animationSpeed) {
            currentFrame = (currentFrame + 1) % numFrames; // Lặp lại (0 đến 8)
            animationClock.restart();
        }
    }
    else {
        currentFrame = 0;
    }

    sprite.setTextureRect(sf::IntRect(
        currentFrame * frameSize.x, // Cột
        spriteRow * frameSize.y,    // Hàng
        frameSize.x,
        frameSize.y
    ));

    return newPos;
}

void Player::updatePosition(sf::Vector2f newPos) {
    position = newPos;
    sprite.setPosition(position);
}

void Player::reset(sf::Vector2f startPos) {
    position = startPos;
    sprite.setPosition(position);
    currentFrame = 0;
    spriteRow = 2; // Reset về hướng Xuống (Hàng 2)
    // Đặt lại texture rect cho đúng
    sprite.setTextureRect(sf::IntRect(0, frameSize.y * 2, frameSize.x, frameSize.y));
}

sf::Vector2i Player::getCellPosition(int cellSize) {
    float scaledWidth = (float)frameSize.x * sprite.getScale().x;
    float scaledHeight = (float)frameSize.y * sprite.getScale().y;

    return sf::Vector2i(
        int((position.x + scaledWidth / 2.f) / cellSize),
        int((position.y + scaledHeight / 2.f) / cellSize)
    );
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite); 
}