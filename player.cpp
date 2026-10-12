#include "player.h"
#include <raymath.h>

Player::Player() {
    camera = { 0 };
    camera.position = { 0.0f, 2.0f, 4.0f }; 
    camera.target = { 0.0f, 2.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    radius = 0.5f;
    speed = 5.0f;
    damage = 25; 
    isAttacking = false;
    ishealing = false; 
    killed = 0;
    yVelocity = 0.0f;
    mainenergy = 100;
    gravity = -20.0f;
    jumpForce = 8.5f;
    auraDamage = 100;
    isGrounded = false;
    mainhealth = 100;
    health = mainhealth;
    energy = mainenergy;
    aura = false;
    cooldownDuration = 25.0;
    lastAttackTime = -25.0;
    isCooldown = false;
    lvl = 1;
    xp = 0;
    level_up = false;
    dash = false;
    pass = false;
}

void Player::Update() {
    float frameTime = GetFrameTime();
    sprint = IsKeyDown(KEY_LEFT_SHIFT) && energy > 0.0f;
    speed = sprint ? 10.0f : 5.0f;
    if (sprint) energy -= frameTime;
    else energy += 10.0f * frameTime;
    if (energy < 0.0f) energy = 0.0f;
    if (energy > mainenergy) energy = mainenergy;
    if (isCooldown) {
        if (GetTime() - lastAttackTime >= cooldownDuration) {
            isCooldown = false;
        }
    }
    aura = false;
    if (IsKeyPressed(KEY_R) && !isCooldown) {
        aura = true;
        lastAttackTime = GetTime();
        isCooldown = true;
    }

    Vector3 movement = { 0.0f, 0.0f, 0.0f };
    if (IsKeyDown(KEY_W)) movement.x += speed * GetFrameTime();
    if (IsKeyDown(KEY_S)) movement.x -= speed * GetFrameTime();
    if (IsKeyDown(KEY_D)) movement.y += speed * GetFrameTime();
    if (IsKeyDown(KEY_A)) movement.y -= speed * GetFrameTime();
    
    Vector2 mouseDelta = GetMouseDelta();
    Vector3 rotation = { 0.0f, 0.0f, 0.0f };
    rotation.x = mouseDelta.x * 0.15f; 
    rotation.y = mouseDelta.y * 0.15f; 
    
    UpdateCameraPro(&camera, movement, rotation, 0.0f);
    if (camera.up.y < 0) camera.up.y = 0.001f;

    if (IsKeyPressed(KEY_Q)) dash = true;
    if (dash && energy >= 20.0f) {
        Vector3 direction = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        Vector3 dashOffset = Vector3Scale(direction, 10.0f);
        camera.position = Vector3Add(camera.position, dashOffset);
        camera.target = Vector3Add(camera.target, dashOffset);
        energy -= 20;
    }
    dash = false;
    
    float playerHeight = 2.0f;
    yVelocity += gravity * GetFrameTime();
    if (isGrounded && IsKeyPressed(KEY_SPACE)) {
        yVelocity = jumpForce;
        isGrounded = false;
    }
    
    float oldY = camera.position.y;
    camera.position.y += yVelocity * GetFrameTime();
    if (camera.position.y <= playerHeight) {
        camera.position.y = playerHeight; 
        yVelocity = 0.0f;
        isGrounded = true;
    }
    
    float deltaY = camera.position.y - oldY;
    camera.target.y += deltaY;
    
    isAttacking = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    
    static float healTimer = 0.0f;
    healTimer += GetFrameTime();
    if (healTimer >= 1.0f) { 
        Healing(1);
        healTimer = 0.0f;
    }
}

Vector3 Player::GetPosition() const {
    return camera.position;
}

void Player::TakeDamage(int damageAmount) {
    health -= damageAmount;
    if (health < 0) health = 0;
}

void Player::Healing(int amount) {
    if (health < mainhealth) {
        health += amount;
        if (health > mainhealth) health = mainhealth;
    }
}
void Player::LevelUp() {
    if (xp >= 100) {
        lvl++;
        mainhealth += 50;
        mainenergy += 50;
        health = mainhealth;
        energy = mainenergy;
        damage += 10;
        auraDamage += 10;
        xp = 0;
    }
}