#pragma once
#include <raylib.h>

enum class EnemyType { SWORD, AXE, BOW, BOSS, FINALBOSS, ENDBOSS }; 

class Enemy {
public:
    Vector3 position;
    float speed;
    float radius; 
    int damage;
    float attackCooldown;
    float lastAttackTime;
    int health;
    int xp;
    EnemyType type;
    Color color;

    Enemy(Vector3 startPos, EnemyType eType);
    void Update(Vector3 playerPos);
    void Draw(Camera3D camera, Texture2D sprite);
    void TakeDamage(int damageAmount);
};