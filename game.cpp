#include "game.h"
#include "setting.h"
#include "colide.h"
#include <raymath.h>
#include <cmath>

Game::Game() : spawnTimer(0.0f), spawnInterval(3.0f), currentWorld(1), gameWon(false) {
    Image img = GenImageChecked(64, 64, 32, 32, RED, BLACK);
    enemySprite = LoadTextureFromImage(img);
    UnloadImage(img);
    SpawnEnemy(); 
}

Game::~Game() {
    UnloadTexture(enemySprite);
}

void Game::ChangeWorld(int newWorld) {
    currentWorld = newWorld;
    enemies.clear(); 
    
    if (currentWorld == 1) {
        spawnInterval = 3.0f; 
    } else if (currentWorld == 2) {
        spawnInterval = 1.5f; 
    }
    player.camera.position = { 0.0f, 2.0f, 4.0f }; 
}

void Game::SpawnEnemy() {
    float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
    float distance = (float)GetRandomValue(15, 30);
    
    Vector3 spawnPos = player.GetPosition();
    spawnPos.x += std::cos(angle) * distance;
    spawnPos.z += std::sin(angle) * distance;
    spawnPos.y = 1.0f;
    
    EnemyType typeToSpawn = EnemyType::SWORD;
    
    if (currentWorld == 1) {
        typeToSpawn = (GetRandomValue(1, 100) > 85) ? EnemyType::BOW : EnemyType::SWORD;
    } else if (currentWorld == 2) {
        int randVal = GetRandomValue(1, 125);
        if (randVal <= 60) typeToSpawn = EnemyType::SWORD;
        else if (randVal <= 120) typeToSpawn = EnemyType::AXE;
        else typeToSpawn = EnemyType::BOSS;
    }

    enemies.push_back(Enemy(spawnPos, typeToSpawn));
}

void Game::Update() {
    if (player.health <= 0 || gameWon) return;

    // Test phím chuyển World
    if (IsKeyPressed(KEY_ONE)) ChangeWorld(1);
    if (IsKeyPressed(KEY_TWO)) ChangeWorld(2);
    if (currentWorld == 1 && player.killed >= 30 && !player.pass) {
        bool finalBossExists = false;
        for (const auto& enemy : enemies) {
            if (enemy.type == EnemyType::FINALBOSS) {
                finalBossExists = true;
                break;
            }
        }
        if (!finalBossExists) {
            enemies.push_back(Enemy({ player.camera.position.x + 10.0f, 1.0f, player.camera.position.z }, EnemyType::FINALBOSS));
        }
    }
    if (currentWorld == 2 && player.killed >= 50 && !player.pass) {
        bool endBossExists = false;
        for (const auto& enemy : enemies) {
            if (enemy.type == EnemyType::ENDBOSS) {
                endBossExists = true;
                break;
            }
        }
        if (!endBossExists) {
            enemies.push_back(Enemy({ player.camera.position.x + 10.0f, 1.0f, player.camera.position.z }, EnemyType::ENDBOSS));
        }
    }
    player.Update();
    
    spawnTimer += GetFrameTime();
    if (spawnTimer >= spawnInterval) {
        SpawnEnemy();
        spawnTimer = 0.0f;
    }
    
    if (player.aura) {
        for (auto& enemy : enemies) {
            if (Collide::CheckSphereCollision(player.GetPosition(), player.radius + 100.0f, enemy.position, enemy.radius)) {
                enemy.TakeDamage(player.auraDamage);
            }
        }
    }

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        it->Update(player.GetPosition());

        if (Collide::CheckSphereCollision(player.GetPosition(), player.radius, it->position, it->radius)) {
            if (GetTime() - it->lastAttackTime >= it->attackCooldown) {
                player.TakeDamage(it->damage);
                it->lastAttackTime = GetTime();
            }
        }

        if (player.isAttacking) {
            if (Collide::CheckSphereCollision(player.GetPosition(), player.radius + 1.5f, it->position, it->radius)) {
                it->TakeDamage(player.damage);
            }
        }

        if (it->health <= 0) {
            bool defeatedFinalBoss = it->type == EnemyType::FINALBOSS;
            bool defeatedEndBoss = it->type == EnemyType::ENDBOSS;
            player.xp += it->xp;
            player.LevelUp();
            it = enemies.erase(it);
            player.killed++;
            player.Healing(5);
            if (defeatedFinalBoss) player.pass = true;
            if (defeatedEndBoss) player.pass = true;
        } else {
            ++it;
        }
    }
    if (player.pass) {
        if (currentWorld == 1) {
            ChangeWorld(2);
            player.pass = false;
        } else {
            gameWon = true;
        }
    }
}

void Game::Draw() {
    BeginDrawing();
    
    Color skyColor = (currentWorld == 1) ? SKYBLUE : DARKPURPLE;
    Color groundColor = (currentWorld == 1) ? DARKGREEN : MAROON;
    ClearBackground(skyColor);
    
    BeginMode3D(player.camera);
        DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 400.0f, 400.0f }, groundColor);
        for (auto& enemy : enemies) {
            enemy.Draw(player.camera, enemySprite);
        }
    EndMode3D();
    
    // Hiển thị thông số UI
    DrawCircle(Setting::screenWidth / 2, Setting::screenHeight / 2, 4, BLACK);
    DrawText(TextFormat("WORLD: %d", currentWorld), Setting::screenWidth - 150, 20, 25, WHITE);
    DrawText(TextFormat("Player HP: %d", player.health), 20, 20, 30, (player.health > 30) ? GREEN : RED);
    DrawText(TextFormat("Player Energy: %.0f", player.energy), 20, 40, 30, BLUE);
    DrawText(TextFormat("Kills: %d", player.killed), 20, 60, 30, YELLOW);
    DrawText(TextFormat("Level: %d", player.lvl), 20, 100, 25, ORANGE);
    DrawText(TextFormat("XP: %d", player.xp), 20, 140, 25, PURPLE);
    
    if (player.isCooldown) {
        DrawText("Aura: COOLDOWN", 20, 200, 20, GRAY);
    } else {
        DrawText("Aura: READY (Press R)", 20, 200, 20, BLUE);
    }
    
    for (const auto& enemy : enemies) {
        Vector3 toEnemy = Vector3Subtract(enemy.position, player.camera.position);
        Vector3 forward = Vector3Subtract(player.camera.target, player.camera.position);
        if (Vector3DotProduct(toEnemy, forward) > 0.0f) {
            Vector2 screenPos = GetWorldToScreen(enemy.position, player.camera);
            if (screenPos.x > 0 && screenPos.x < Setting::screenWidth && screenPos.y > 0 && screenPos.y < Setting::screenHeight) {
                DrawText(TextFormat("%d", enemy.health), (int)screenPos.x - 10, (int)screenPos.y - 40, 20, ORANGE);
            }
        }
    }

    if (player.health <= 0) {
        DrawText("GAME OVER!", Setting::screenWidth / 2 - 120, Setting::screenHeight / 2 - 40, 40, RED);
    }
    if (gameWon) {
        DrawText("YOU WIN!", Setting::screenWidth / 2 - 100, Setting::screenHeight / 2 - 40, 40, GREEN);
    }

    EndDrawing();
}

bool Game::IsRunning() {
    return !WindowShouldClose();
}