#include "Maze.h"
#include <ctime>       // Cho time(0)
#include <functional>  // Cho std::function
#include <algorithm>   // Cho std::shuffle

Maze::Maze(int width, int height) : w(width), h(height) {
    walls.resize(h, std::vector<bool>(w, true));
    traps.resize(h, std::vector<int>(w, 0));
}

void Maze::generate() {
    std::vector<std::vector<bool>> visited(h, std::vector<bool>(w, false));
    std::mt19937 gen(time(0));

    std::function<void(int, int)> dfs = [&](int x, int y) {
        visited[y][x] = true; walls[y][x] = false;
        std::vector<std::pair<int, int>> dirs = { {0,1},{0,-1},{1,0},{-1,0} };
        std::shuffle(dirs.begin(), dirs.end(), gen);
        for (auto& d : dirs) {
            int nx = x + d.first * 2, ny = y + d.second * 2;
            if (nx >= 0 && nx < w && ny >= 0 && ny < h && !visited[ny][nx]) {
                walls[y + d.second][x + d.first] = false;
                dfs(nx, ny);
            }
        }
        };
    dfs(1, 1);
    entrance = { 1,1 };
    exit = { w - 2,h - 2 };
}

void Maze::generateTraps(double redP, double yellowP) {
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> dis(0, 1);

    // Thêm bộ tạo hướng ngẫu nhiên (0 = Ngang, 1 = Dọc)
    std::uniform_int_distribution<> dirDis(0, 1);

    // === BƯỚC 1: Đặt bẫy đỏ (đặt riêng lẻ) ===
    // Lặp qua toàn bộ mê cung
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (!walls[y][x]) { // Nếu là đường đi
                if (dis(gen) < redP) // Kiểm tra xác suất bẫy đỏ
                    traps[y][x] = 1; // Đặt bẫy đỏ
            }
        }
    }

    // === BƯỚC 2: Đặt bẫy vàng (luôn theo chuỗi 3 ô) ===
    // Lặp lại toàn bộ mê cung một lần nữa
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            // Chỉ đặt bẫy nếu:
            // 1. Là đường đi (!walls)
            // 2. Chưa có bẫy đỏ (traps == 0)
            if (!walls[y][x] && traps[y][x] == 0) {

                // Kiểm tra xác suất bẫy vàng
                if (dis(gen) < yellowP) {

                    int direction = dirDis(gen); // Chọn hướng ngẫu nhiên

                    if (direction == 0) { // Đặt theo chiều Ngang (x, x+1, x+2)

                        // Kiểm tra xem 2 ô bên phải có hợp lệ không
                        if (x + 2 < w && // Nằm trong biên
                            !walls[y][x + 1] && traps[y][x + 1] == 0 && // Ô (x+1) là đường đi & không có bẫy
                            !walls[y][x + 2] && traps[y][x + 2] == 0)   // Ô (x+2) là đường đi & không có bẫy
                        {
                            // Nếu hợp lệ, đặt 3 bẫy vàng
                            traps[y][x] = 2;
                            traps[y][x + 1] = 2;
                            traps[y][x + 2] = 2;
                        }
                    }
                    else { // Đặt theo chiều Dọc (y, y+1, y+2)

                        // Kiểm tra xem 2 ô bên dưới có hợp lệ không
                        if (y + 2 < h && // Nằm trong biên
                            !walls[y + 1][x] && traps[y + 1][x] == 0 && // Ô (y+1) là đường đi & không có bẫy
                            !walls[y + 2][x] && traps[y + 2][x] == 0)   // Ô (y+2) là đường đi & không có bẫy
                        {
                            // Nếu hợp lệ, đặt 3 bẫy vàng
                            traps[y][x] = 2;
                            traps[y + 1][x] = 2;
                            traps[y + 2][x] = 2;
                        }
                    }
                }
            }
        }
    }

    // === BƯỚC 3: Dọn dẹp lối vào/ra ===
    // Luôn đảm bảo lối vào và lối ra an toàn
    traps[entrance.second][entrance.first] = 0;
    traps[exit.second][exit.first] = 0;
}
