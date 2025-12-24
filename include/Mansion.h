#ifndef MANSION_H
#define MANSION_H

#include "Game.h"
#include "Renderer.h"
#include <vector>

class Mansion {
public:
    Mansion();
    
    void initialize();
    
    std::vector<Room> getRooms() const { return rooms; }
    std::vector<Door> getDoors() const { return doors; }
    std::vector<HidingSpot> getHidingSpots() const { return hidingSpots; }
    
    bool isPlayerInRoom(const Vector3& playerPos, int roomIndex) const;
    bool canPlayerMoveTo(const Vector3& from, const Vector3& to) const;
    
    HidingSpot* getNearestHidingSpot(const Vector3& playerPos, float maxDistance);
    
    Vector3 getRandomPatrolPoint() const;
    std::vector<Vector3> getMonsterPatrolPoints() const;
    
private:
    void createRooms();
    void createDoors();
    void createHidingSpots();
    
    bool checkCollision(const Vector3& pos, const Vector3& roomPos, const Vector3& roomSize) const;
    
    std::vector<Room> rooms;
    std::vector<Door> doors;
    std::vector<HidingSpot> hidingSpots;
    
    Vector3 mansionSize;
};

#endif // MANSION_H
