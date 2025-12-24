#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <memory>
#include <string>

// Forward declarations
class Player;
class Monster;
class TaskSystem;
class Renderer;
class InputHandler;
class Menu;
class AudioManager;
class Mansion;

enum class GameState {
    MAIN_MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    VICTORY
};

enum class ControlMode {
    DESKTOP,
    MOBILE
};

struct Vector3 {
    float x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    
    float length() const {
        return sqrt(x * x + y * y + z * z);
    }
    
    Vector3 normalize() const {
        float len = length();
        if (len > 0) return Vector3(x / len, y / len, z / len);
        return Vector3(0, 0, 0);
    }
    
    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
};

struct Task {
    std::string description;
    Vector3 location;
    float radius;
    bool completed;
    int id;
};

class Game {
public:
    Game();
    ~Game();
    
    bool initialize();
    void run();
    void cleanup();
    
    void setState(GameState state) { currentState = state; }
    GameState getState() const { return currentState; }
    
    void setControlMode(ControlMode mode) { controlMode = mode; }
    ControlMode getControlMode() const { return controlMode; }
    
    SDL_Window* getWindow() { return window; }
    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }
    
private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    
    SDL_Window* window;
    SDL_GLContext glContext;
    
    int screenWidth;
    int screenHeight;
    bool running;
    
    GameState currentState;
    ControlMode controlMode;
    
    std::unique_ptr<Player> player;
    std::unique_ptr<Monster> monster;
    std::unique_ptr<TaskSystem> taskSystem;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<InputHandler> inputHandler;
    std::unique_ptr<Menu> menu;
    std::unique_ptr<AudioManager> audioManager;
    std::unique_ptr<Mansion> mansion;
    
    Uint32 lastTime;
};

#endif // GAME_H
