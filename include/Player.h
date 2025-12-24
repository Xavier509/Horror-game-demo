#ifndef PLAYER_H
#define PLAYER_H

#include "Game.h"
#include <cmath>

class Player {
public:
    Player(Vector3 startPos);
    
    void update(float deltaTime);
    void handleInput(const InputHandler& input, float deltaTime, ControlMode mode);
    
    Vector3 getPosition() const { return position; }
    void setPosition(const Vector3& pos) { position = pos; }
    
    Vector3 getVelocity() const { return velocity; }
    
    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }
    
    void setYaw(float y) { yaw = y; }
    void setPitch(float p) { pitch = std::max(-89.0f, std::min(89.0f, p)); }
    
    Vector3 getForward() const;
    Vector3 getRight() const;
    
    bool isHiding() const { return hiding; }
    void setHiding(bool h) { hiding = h; }
    
    float getStamina() const { return stamina; }
    bool canRun() const { return stamina > 10.0f; }
    
    float getHealth() const { return health; }
    void takeDamage(float amount);
    void heal(float amount);
    
    bool isAlive() const { return health > 0; }
    
private:
    Vector3 position;
    Vector3 velocity;
    float yaw;
    float pitch;
    
    float moveSpeed;
    float sprintSpeed;
    float mouseSensitivity;
    
    float stamina;
    float maxStamina;
    float staminaRegenRate;
    float staminaDrainRate;
    
    float health;
    float maxHealth;
    
    bool hiding;
    bool isSprinting;
    
    const float GRAVITY = -20.0f;
    const float PLAYER_HEIGHT = 1.8f;
};

#endif // PLAYER_H
