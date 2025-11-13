#include "GameManager.h"
#include "Menu.h"
#include <iostream>
#include <algorithm>
#include <memory>

GameManager::GameManager(sf::RenderWindow& win, sf::Font& f)
    : window(win), font(f), maze(W, H),
    lives(3), gameTimer(300.f), vision(3),
    // THAY ĐỔI: Xóa redActive
    gameOver(false), win(false)
{
    maze.generate();
    maze.generateTraps(0.05, 0.05); // (Bẫy Đỏ 5%, Bẫy Vàng 5%)

    sf::Vector2f startPos((maze.entrance.first * 2) * cellSize, (maze.entrance.second * 2) * cellSize);

    sf::Vector2i frameSize(64, 64);
    int frameCount = 8;
    player = std::make_unique<Player>("../data/texture/skeleton.png", startPos, cellSize, frameSize, frameCount);

    explored.resize(H, std::vector<bool>(W, false));
    // Khởi tạo mảng "bẫy đã kích hoạt"
    trapsRevealed.resize(H, std::vector<bool>(W, false));

    livesText.setFont(font); livesText.setCharacterSize(30);
    livesText.setPosition(10, 10); livesText.setFillColor(sf::Color::White);

    timerText.setFont(font); timerText.setCharacterSize(30);
    timerText.setPosition(window.getSize().x - 150, 10); timerText.setFillColor(sf::Color::White);

    messageText.setFont(font); messageText.setCharacterSize(40);
    messageText.setStyle(sf::Text::Bold);

    gameView.setSize(window.getSize().x, window.getSize().y);
    hudView = window.getDefaultView();
}

void GameManager::run() {
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
                return;
            }
            else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) {
                return; // Trở về menu
            }
        }

        float delta = deltaClock.restart().asSeconds();
        update(delta);
        render();

        if (gameOver) {
            drawGameOverMessage();
            window.display();

            std::vector<std::string> options = { "Play Again","Return to Menu" };
            int choice = showMenu(window, font, options);

            if (choice == 0) resetGame();
            else return;
        }
    }
}

void GameManager::update(float delta) {
    if (gameOver) return;

    // THAY ĐỔI: Xóa toàn bộ logic redClock

    gameTimer -= delta;
    livesText.setString("Lives: " + std::to_string(lives));
    timerText.setString("Time: " + std::to_string(static_cast<int>(gameTimer)));

    sf::Vector2f newPos = player->handleInput(delta);

    int vcx = int((newPos.x + cellSize / 2.f) / cellSize);
    int vcy = int((newPos.y + cellSize / 2.f) / cellSize);
    int lcx = vcx / 2;
    int lcy = vcy / 2;

    if (lcx >= 0 && lcx < W && lcy >= 0 && lcy < H && !maze.walls[lcy][lcx])
        player->updatePosition(newPos);

    sf::Vector2i visualCellPos = player->getCellPosition(cellSize);
    int pcx_logical = visualCellPos.x / 2;
    int pcy_logical = visualCellPos.y / 2;

    // THAY ĐỔI: Xóa vòng lặp reset trapsRevealed

    if (pcx_logical >= 0 && pcx_logical < W && pcy_logical >= 0 && pcy_logical < H) {
        updateExplored(pcx_logical, pcy_logical);

        // THAY ĐỔI: Xóa lệnh gọi revealTraps()

        // Logic bẫy giờ được xử lý 100% trong hàm này
        checkTrapCollision(pcx_logical, pcy_logical);

        checkGameConditions(pcx_logical, pcy_logical);
    }
}

// THAY ĐỔI: Xóa toàn bộ hàm revealTraps()

void GameManager::updateExplored(int pcx, int pcy) {
    for (int dy = -vision; dy <= vision; dy++)
        for (int dx = -vision; dx <= vision; dx++) {
            int nx2 = pcx + dx, ny2 = pcy + dy;
            if (nx2 >= 0 && nx2 < W && ny2 >= 0 && ny2 < H)
                explored[ny2][nx2] = true;
        }
}

// THAY ĐỔI: Viết lại hoàn toàn logic bẫy gai
void GameManager::checkTrapCollision(int pcx, int pcy) {

    // Logic Bẫy Vàng "???" (Bẫy tàng hình chết ngay)
    if (maze.traps[pcy][pcx] == 2) {
        lives = 0;
        gameOver = true;
        win = false;
        messageText.setString("???");
        messageText.setFillColor(sf::Color::Cyan);
        return;
    }

    // Logic Bẫy Đỏ "Gai" (Bẫy loại 1)
    if (maze.traps[pcy][pcx] == 1) {
        // Kiểm tra xem bẫy này đã bị kích hoạt (lò xo) chưa
        if (trapsRevealed[pcy][pcx] == false) {
            // Đây là LẦN ĐẦU TIÊN dẫm lên
            trapsRevealed[pcy][pcx] = true; // Kích hoạt bẫy

            lives--; // Mất 1 mạng

            if (lives <= 0) {
                gameOver = true;
                win = false;
                messageText.setString("YOU DIED!");
                messageText.setFillColor(sf::Color::Red);
            }
            else {
                // Văng người chơi về điểm bắt đầu
                sf::Vector2f startPos((maze.entrance.first * 2) * cellSize, (maze.entrance.second * 2) * cellSize);
                player->reset(startPos);
            }
        }
        // Nếu trapsRevealed[pcy][pcx] == true (đã bị kích hoạt rồi)
        // thì không làm gì cả. Bẫy giờ đã vô hại.
    }
}


void GameManager::checkGameConditions(int pcx, int pcy) {
    if (gameOver) return;
    if (pcx == maze.exit.first && pcy == maze.exit.second) {
        gameOver = true; win = true;
        messageText.setString("YOU ESCAPED!");
        messageText.setFillColor(sf::Color::Green);
    }
    else if (gameTimer <= 0.f) {
        gameOver = true; win = false;
        messageText.setString("TIME'S UP!");
        messageText.setFillColor(sf::Color::Yellow);
    }
}

void GameManager::render() {
    window.clear(sf::Color::Black);

    sf::Vector2f pPos = player->position;
    float viewX = pPos.x + cellSize / 2.f;
    float viewY = pPos.y + cellSize / 2.f;

    viewX = std::max(window.getSize().x / 2.f, std::min(viewX, (W * 2) * cellSize - window.getSize().x / 2.f));
    viewY = std::max(window.getSize().y / 2.f, std::min(viewY, (H * 2) * cellSize - window.getSize().y / 2.f));
    gameView.setCenter(viewX, viewY);

    window.setView(gameView);
    drawMaze();
    player->draw(window);

    window.setView(hudView);
    window.draw(livesText);
    window.draw(timerText);

    if (!gameOver) window.display();
}

void GameManager::drawMaze() {
    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            sf::Color cellColor;
            if (explored[y][x]) {
                if (maze.walls[y][x]) cellColor = sf::Color::Black;
                else cellColor = sf::Color::White;
            }
            else cellColor = sf::Color(80, 80, 80);

            // Vẽ nền (trắng, đen, hoặc xám)
            for (int dy = 0; dy < 2; dy++)
                for (int dx = 0; dx < 2; dx++) {
                    cell.setPosition((x * 2 + dx) * cellSize, (y * 2 + dy) * cellSize);
                    cell.setFillColor(cellColor);
                    window.draw(cell);
                }

            // THAY ĐỔI: Vẽ bẫy gai ĐÃ BỊ KÍCH HOẠT
            // Bẫy loại 2 (vàng) sẽ không bao giờ được vẽ
            if (trapsRevealed[y][x] && maze.traps[y][x] == 1) {
                // Luôn vẽ màu đỏ (vì nó đã "bật lên" vĩnh viễn)
                sf::Color trapColor = sf::Color(255, 50, 50);
                for (int dy = 0; dy < 2; dy++)
                    for (int dx = 0; dx < 2; dx++) {
                        cell.setPosition((x * 2 + dx) * cellSize, (y * 2 + dy) * cellSize);
                        cell.setFillColor(trapColor);
                        window.draw(cell);
                    }
            }
        }
    }
}

void GameManager::drawGameOverMessage() {
    window.setView(hudView);
    sf::FloatRect rect = messageText.getLocalBounds();
    messageText.setOrigin(rect.width / 2.f, rect.height / 2.f);
    messageText.setPosition(hudView.getCenter());
    window.draw(messageText);
}

void GameManager::resetGame() {
    maze.generate();
    maze.generateTraps(0.05, 0.05);

    sf::Vector2f startPos((maze.entrance.first * 2) * cellSize, (maze.entrance.second * 2) * cellSize);

    sf::Vector2i frameSize(64, 64);
    int frameCount =8;
    player = std::make_unique<Player>("../data/texture/skeleton.png", startPos, cellSize, frameSize, frameCount);

    lives = 3;
    gameTimer = 300.f;
    gameOver = false;
    win = false;

    for (auto& row : explored)
        std::fill(row.begin(), row.end(), false);

    // QUAN TRỌNG: Reset lại bẫy đã kích hoạt khi chơi lại
    for (auto& row : trapsRevealed)
        std::fill(row.begin(), row.end(), false);

    deltaClock.restart();
    // THAY ĐỔI: Xóa redClock.restart()
}