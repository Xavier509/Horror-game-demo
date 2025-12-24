#include "Player.h"
#include "InputHandler.h"
#include <cmath>
#include <algorithm>

Player::Player(Vector3 startPos)
    : position(startPos), velocity(0, 0, 0),
      yaw(0.0f), pitch(0.0f),
      moveSpeed(5.0f), sprintSpeed(8.0f),
      mouseSensitivity(0.1f),
      stamina(100.0f), maxStamina(100.0f),
      staminaRegenRate(15.0f), staminaDrainRate(20.0f),
      health(100.0f), maxHealth(100.0f),
      hiding(false), isSprinting(false) {
}

void Player::update(float deltaTime) {
    // Apply gravity
    velocity.y += GRAVITY * deltaTime;
    
    // Update position
    position = position + velocity * deltaTime;
    
    // Ground collision (simple)
    if (position.y < PLAYER_HEIGHT) {
        position.y = PLAYER_HEIGHT;
        velocity.y = 0;
    }
    
    // Regenerate or drain stamina
    if (isSprinting && !hiding) {
        stamina = std::max(0.0f, stamina - staminaDrainRate * deltaTime);
        if (stamina <= 0) {
            isSprinting = false;
        }
    } else {
        stamina = std::min(maxStamina, stamina + staminaRegenRate * deltaTime);
    }
    
    // Slow down velocity (friction)
    velocity.x *= 0.8f;
    velocity.z *= 0.8f;
}

void Player::handleInput(const InputHandler& input, float deltaTime, ControlMode mode) {
    if (hiding) {
        // Can't move while hiding
        velocity.x = 0;
        velocity.z = 0;
        return;
    }
    
    if (mode == ControlMode::DESKTOP) {
        // Movement
        Vector3 moveDir(0, 0, 0);
        
        if (input.isKeyPressed(SDLK_w)) moveDir.z += 1.0f;
        if (input.isKeyPressed(SDLK_s)) moveDir.z -= 1.0f;
        if (input.isKeyPressed(SDLK_a)) moveDir.x -= 1.0f;
        if (input.isKeyPressed(SDLK_d)) moveDir.x += 1.0f;
        
        // Sprint
        isSprinting = input.isKeyPressed(SDLK_LSHIFT) && canRun();
        float currentSpeed = isSprinting ? sprintSpeed : moveSpeed;
        
        // Transform movement to world space
        if (moveDir.length() > 0) {
            moveDir = moveDir.normalize();
            Vector3 forward = getForward();
            Vector3 right = getRight();
            
            Vector3 movement = (forward * moveDir.z + right * moveDir.x) * currentSpeed;
            velocity.x = movement.x;
            velocity.z = movement.z;
        }
        
        // Mouse look
        int deltaX, deltaY;
        input.getMouseDelta(deltaX, deltaY);
        
        yaw += deltaX * mouseSensitivity;
        pitch -= deltaY * mouseSensitivity;
        pitch = std::max(-89.0f, std::min(89.0f, pitch));
        
        // Wrap yaw
        while (yaw > 360.0f) yaw -= 360.0f;
        while (yaw < 0.0f) yaw += 360.0f;
        
    } else { // Mobile touch controls
        auto touches = input.getActiveTouches();
        
        // Left side of screen: movement joystick
        // Right side of screen: look
        for (const auto& touch : touches) {
            if (touch.startX < 400) { // Movement area
                float dx = touch.x - touch.startX;
                float dy = touch.y - touch.startY;
                float length = sqrt(dx*dx + dy*dy);
                
                if (length > 10.0f) {
                    dx /= length;
                    dy /= length;
                    
                    Vector3 forward = getForward();
                    Vector3 right = getRight();
                    Vector3 movement = (forward * dy + right * dx) * moveSpeed;
                    velocity.x = movement.x;
                    velocity.z = movement.z;
                }
            } else { // Look area
                float dx = touch.x - touch.startX;
                float dy = touch.y - touch.startY;
                
                yaw += dx * mouseSensitivity * 0.5f;
                pitch -= dy * mouseSensitivity * 0.5f;
                pitch = std::max(-89.0f, std::min(89.0f, pitch));
            }
        }
    }
}

Vector3 Player::getForward() const {
    float yawRad = yaw * M_PI / 180.0f;
    float pitchRad = pitch * M_PI / 180.0f;
    
    return Vector3(
        cos(pitchRad) * sin(yawRad),
        sin(pitchRad),
        cos(pitchRad) * cos(yawRad)
    );
}

Vector3 Player::getRight() const {
    float yawRad = yaw * M_PI / 180.0f;
    return Vector3(
        cos(yawRad),
        0,
        -sin(yawRad)
    );
}

void Player::takeDamage(float amount) {
    health = std::max(0.0f, health - amount);
}

void Player::heal(float amount) {
    health = std::min(maxHealth, health + amount);
}
