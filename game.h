#pragma once
#include <raylib.h>
#include <vector>
#include "player.h"
#include "sprites.h"
#include "setting.h"

class Game {
private:
    Player player;
    std::vector<Enemy> enemies; 
    Texture2D enemySprite;      
    
    float spawnTimer;
    float spawnInterval;        
    int currentWorld;
    bool gameWon;

public:
    Game();
    ~Game();
    void Update();
    void Draw();
    bool IsRunning();
    void SpawnEnemy();          
    void ChangeWorld(int newWorld); 
};