#pragma once
#include <raylib.h>

class Player {
public:
    Camera3D camera;
    float radius;
    float speed;
    int damage;
    int auraDamage;
    bool isAttacking;
    bool ishealing; 
    int killed;
    int mainhealth;
    int health;
    float energy;
    float yVelocity;  
    float gravity;    
    float jumpForce;  
    bool isGrounded;  
    bool aura;
    double cooldownDuration;
    double lastAttackTime;
    bool isCooldown;
    bool level_up;
    bool dash;
    bool pass;
    bool sprint;
    float mainenergy;
    int lvl;
    int xp;
    Player();
    void Update();
    Vector3 GetPosition() const;
    void TakeDamage(int damage);
    void Healing(int amount); 
    void LevelUp();
};