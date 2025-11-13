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
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            if (!walls[y][x]) {
                if (dis(gen) < redP) traps[y][x] = 1;
                else if (dis(gen) < yellowP) traps[y][x] = 2;
            }
        }
    traps[entrance.second][entrance.first] = 0;
    traps[exit.second][exit.first] = 0;
}