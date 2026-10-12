#include "setting.h"

Setting::Setting() {
    InitWindow(screenWidth, screenHeight, "Game");
    SetTargetFPS(60);
    DisableCursor(); 
}

Setting::~Setting() {
    CloseWindow();
}