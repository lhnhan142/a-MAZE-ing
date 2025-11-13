#pragma once // Đảm bảo tệp này chỉ được include 1 lần

#include <vector>
#include <string>
#include <random>
#include <SFML/System/Vector2.hpp> // Chỉ include phần cần thiết

class Maze {
public:
    int w, h;
    std::vector<std::vector<bool>> walls;
    std::vector<std::vector<int>> traps; // 0=none,1=red,2=yellow
    std::pair<int, int> entrance, exit;

    // Khai báo hàm
    Maze(int width, int height);
    void generate();
    void generateTraps(double redP = 0.025, double yellowP = 0.015);
};