#include "game.h"
#include "setting.h"
#include <raylib.h>
int main() {
    InitWindow(Setting::screenWidth, Setting::screenHeight, "GAME");
    SetTargetFPS(60);
    DisableCursor();
    Game game; 
    while (game.IsRunning()) {
        game.Update();
        game.Draw();
    }
    EnableCursor();
    CloseWindow();
    return 0;
}