#include "sprites.h"
#include <raymath.h>

Enemy::Enemy(Vector3 startPos, EnemyType eType) : position(startPos), lastAttackTime(0.0f), type(eType) {
    switch (type) {
        case EnemyType::SWORD: // Quái nhỏ, nhanh
            speed = 5.0f; radius = 0.6f; damage = 5; health = 100; xp = 20;  attackCooldown = 0.5f; color = GREEN;
            break;
        case EnemyType::AXE: // Quái to, chậm
            speed = 2.0f; radius = 3.0f; damage = 40; health = 200; xp = 80; attackCooldown = 1.0f; color = DARKBROWN;
            break;
        case EnemyType::BOW: // Boss khu vực
            speed = 3.0f; radius = 1.5f; damage = 50; health = 100; xp = 100; attackCooldown = 5.0f; color = PURPLE;
            break;
        case EnemyType::BOSS:
            speed = 1.0f; radius = 5.0f; damage = 100; health = 500; xp = 200; attackCooldown = 2.0f; color = RED;
            break;
        case EnemyType::FINALBOSS:
            speed = 2.0f; radius = 6.0f; damage = 200; health = 1000; xp = 1000; attackCooldown = 1.5f; color = MAROON;
            break;
        case EnemyType::ENDBOSS:
            speed = 1.5f; radius = 7.0f; damage = 300; health = 1500; xp = 2000; attackCooldown = 1.0f; color = DARKPURPLE;
            break;
    }
}

void Enemy::Update(Vector3 playerPos) {
    if (health <= 0) return; 
    Vector3 direction = Vector3Subtract(playerPos, position);
    direction.y = 0.0f; 
    
    if (Vector3Length(direction) > 0.1f) {
        direction = Vector3Normalize(direction);
        position.x += direction.x * speed * GetFrameTime();
        position.z += direction.z * speed * GetFrameTime();
    }
}

void Enemy::Draw(Camera3D camera, Texture2D sprite) {
    if (health > 0) {
        // Vẽ kích thước theo radius tương ứng
        DrawCube(position, radius * 2, radius * 2, radius * 2, color);
    }
}

void Enemy::TakeDamage(int damageAmount) {
    health -= damageAmount;
    if (health < 0) health = 0;
}