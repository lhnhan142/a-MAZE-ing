#include "Menu.h"

// Định nghĩa (triển khai) hàm
int showMenu(sf::RenderWindow& window, sf::Font& font, const std::vector<std::string>& options) {
    int selected = 0;
    sf::View originalView = window.getView();
    sf::View menuView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(menuView);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) return -1;
            else if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Up) selected = (selected + options.size() - 1) % options.size();
                else if (e.key.code == sf::Keyboard::Down) selected = (selected + 1) % options.size();
                else if (e.key.code == sf::Keyboard::Enter) {
                    window.setView(originalView);
                    return selected;
                }
            }
        }
        window.clear(sf::Color::Black);
        for (int i = 0; i < options.size(); i++) {
            sf::Text t(options[i], font, 50);
            t.setPosition(window.getSize().x / 2.f - 100, 200 + i * 70);
            t.setFillColor(i == selected ? sf::Color::Yellow : sf::Color::White);
            window.draw(t);
        }
        window.display();
    }
    window.setView(originalView);
    return -1;
}