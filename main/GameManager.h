#pragma once

#include "Maze.h"    // Include các lớp
#include "Player.h" // mà nó quản lý
#include <SFML/Graphics.hpp>
#include <memory> // Cho std::unique_ptr

class GameManager {
public:
    // Kích thước LOGIC (đường đi 2x2)
    static constexpr int W = 40;
    static constexpr int H = 25;
    static constexpr int cellSize = 60;

private:
    sf::RenderWindow& window;
    sf::Font& font;
    Maze maze;
    std::unique_ptr<Player> player;

    int lives;
    float gameTimer;
    int vision;
    std::vector<std::vector<bool>> explored;

    // THAY ĐỔI: Giờ đây mảng này có nghĩa là "Bẫy đã bị kích hoạt/lò xo"
    std::vector<std::vector<bool>> trapsRevealed;

    sf::Text livesText, timerText, messageText;

    sf::Clock deltaClock;

    // === THÊM CÁC BIẾN CHO BẪY NHẤP NHÁY ===
    sf::Clock redClock;
    sf::Clock yellowClock;
    bool redActive;
    bool yellowActive;

    // === THÊM CÁC BIẾN CHO HỆ THỐNG BẤT TỬ ===
    bool isInvincible;
    sf::Clock invincibleClock;
    bool playerVisible;
    // Đặt thời gian bất tử là 2.5 giây
    const float invincibilityDuration = 2.5f;

    bool gameOver, win;

    sf::View gameView;
    sf::View hudView;

    // Các hàm private (hàm nội bộ)
    void update(float delta);
    void render();
    void updateExplored(int pcx, int pcy);

    // THAY ĐỔI: Xóa hàm revealTraps
    // void revealTraps(int pcx, int pcy);

    void checkTrapCollision(int pcx, int pcy);
    void checkGameConditions(int pcx, int pcy);
    void drawMaze();
    void drawGameOverMessage();
    void resetGame();

public:
    GameManager(sf::RenderWindow& win, sf::Font& f);
    void run(); // Hàm công khai duy nhất để bắt đầu game
};
