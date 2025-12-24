#ifndef RENDERER_H
#define RENDERER_H

#include "Game.h"
#include <vector>

struct Room {
    Vector3 position;
    Vector3 size;
    std::string name;
};

struct Door {
    Vector3 position;
    bool isOpen;
    int connectsRooms[2];
};

struct HidingSpot {
    Vector3 position;
    float radius;
    std::string type; // "closet", "under_bed", "cabinet"
};

class Renderer {
public:
    Renderer(int width, int height);
    
    void initialize();
    void beginFrame();
    void endFrame();
    
    void renderMansion(const std::vector<Room>& rooms, const std::vector<Door>& doors);
    void renderPlayer(const Player& player);
    void renderMonster(const Monster& monster, const Vector3& playerPos);
    void renderTasks(const std::vector<Task>& tasks);
    void renderHidingSpots(const std::vector<HidingSpot>& spots);
    
    void renderHUD(const Player& player, const TaskSystem& taskSystem, const Monster& monster);
    void renderCrosshair();
    
    void setCamera(const Vector3& position, float yaw, float pitch);
    
    void renderText(const std::string& text, int x, int y, float r = 1.0f, float g = 1.0f, float b = 1.0f);
    
private:
    void drawCube(const Vector3& pos, const Vector3& size, float r, float g, float b, float a = 1.0f);
    void drawFloor(float size);
    void drawWalls(const Room& room);
    void drawDoor(const Door& door);
    void drawMonster(const Vector3& pos, float scale = 1.0f);
    void drawTaskMarker(const Vector3& pos, bool completed);
    
    void setupLighting();
    void setupPerspective();
    void setup2D();
    
    int screenWidth;
    int screenHeight;
    
    float ambientLight;
    bool fogEnabled;
};

#endif // RENDERER_H
