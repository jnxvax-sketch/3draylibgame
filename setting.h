#pragma once
#include <raylib.h>

class Setting {
public:
    static constexpr int screenWidth = 1366;
    static constexpr int screenHeight = 768;
    static constexpr int padding = 50;

    Setting();
    ~Setting();
};