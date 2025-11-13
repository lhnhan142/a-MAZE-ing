#include <SFML/Graphics.hpp>
#include "GameManager.h" // Include lớp quản lý game
#include "Menu.h"      // Include hàm menu
#include <iostream>    // Cho std::cout

// Hàm playGame cũ giờ chỉ có nhiệm vụ tạo và chạy GameManager
void playGame(sf::RenderWindow& window, sf::Font& font) {
    GameManager game(window, font);
    game.run();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Maze Game");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("../font/arial.ttf")) {
        std::cout << "Place arial.ttf in folder\n";
        return 0;
    }

    std::vector<std::string> mainMenu = { "New Game","Exit" };
    while (window.isOpen()) {
        // Gọi hàm showMenu từ Menu.h
        int choice = showMenu(window, font, mainMenu);

        if (choice == 0) {
            playGame(window, font); // Chạy game
        }
        else break; // Thoát (choice == 1 hoặc -1)
    }

    return 0;
}