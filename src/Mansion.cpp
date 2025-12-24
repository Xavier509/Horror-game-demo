#include "Mansion.h"
#include <cmath>

Mansion::Mansion() : mansionSize(100.0f, 10.0f, 100.0f) {
}

void Mansion::initialize() {
    createRooms();
    createDoors();
    createHidingSpots();
}

void Mansion::createRooms() {
    rooms.clear();
    
    // Entrance Hall
    Room entrance;
    entrance.name = "Entrance Hall";
    entrance.position = Vector3(10.0f, 0.0f, 10.0f);
    entrance.size = Vector3(15.0f, 5.0f, 15.0f);
    rooms.push_back(entrance);
    
    // Study
    Room study;
    study.name = "Study";
    study.position = Vector3(30.0f, 0.0f, 15.0f);
    study.size = Vector3(12.0f, 5.0f, 10.0f);
    rooms.push_back(study);
    
    // Library
    Room library;
    library.name = "Library";
    library.position = Vector3(25.0f, 0.0f, 30.0f);
    library.size = Vector3(10.0f, 5.0f, 12.0f);
    rooms.push_back(library);
    
    // Bedroom
    Room bedroom;
    bedroom.name = "Master Bedroom";
    bedroom.position = Vector3(45.0f, 0.0f, 35.0f);
    bedroom.size = Vector3(14.0f, 5.0f, 14.0f);
    rooms.push_back(bedroom);
    
    // Kitchen
    Room kitchen;
    kitchen.name = "Kitchen";
    kitchen.position = Vector3(10.0f, 0.0f, 35.0f);
    kitchen.size = Vector3(12.0f, 5.0f, 10.0f);
    rooms.push_back(kitchen);
    
    // Dining Room
    Room dining;
    dining.name = "Dining Room";
    dining.position = Vector3(15.0f, 0.0f, 50.0f);
    dining.size = Vector3(16.0f, 5.0f, 12.0f);
    rooms.push_back(dining);
    
    // Basement (Lab)
    Room basement;
    basement.name = "Basement Laboratory";
    basement.position = Vector3(20.0f, -5.0f, 55.0f);
    basement.size = Vector3(20.0f, 4.0f, 15.0f);
    rooms.push_back(basement);
    
    // Hallway 1
    Room hallway1;
    hallway1.name = "Hallway";
    hallway1.position = Vector3(18.0f, 0.0f, 20.0f);
    hallway1.size = Vector3(6.0f, 5.0f, 15.0f);
    rooms.push_back(hallway1);
    
    // Hallway 2
    Room hallway2;
    hallway2.name = "Upstairs Hallway";
    hallway2.position = Vector3(35.0f, 0.0f, 25.0f);
    hallway2.size = Vector3(8.0f, 5.0f, 18.0f);
    rooms.push_back(hallway2);
}

void Mansion::createDoors() {
    doors.clear();
    
    // Door from entrance to hallway
    Door door1;
    door1.position = Vector3(17.0f, 1.5f, 15.0f);
    door1.isOpen = true;
    door1.connectsRooms[0] = 0;
    door1.connectsRooms[1] = 7;
    doors.push_back(door1);
    
    // Door from hallway to study
    Door door2;
    door2.position = Vector3(25.0f, 1.5f, 17.0f);
    door2.isOpen = false; // Locked initially
    door2.connectsRooms[0] = 7;
    door2.connectsRooms[1] = 1;
    doors.push_back(door2);
    
    // Door from hallway to library
    Door door3;
    door3.position = Vector3(20.0f, 1.5f, 27.0f);
    door3.isOpen = true;
    door3.connectsRooms[0] = 7;
    door3.connectsRooms[1] = 2;
    doors.push_back(door3);
    
    // Door to basement
    Door door4;
    door4.position = Vector3(20.0f, 0.5f, 48.0f);
    door4.isOpen = false; // Locked initially
    door4.connectsRooms[0] = 5;
    door4.connectsRooms[1] = 6;
    doors.push_back(door4);
    
    // Door from hallway to bedroom
    Door door5;
    door5.position = Vector3(38.0f, 1.5f, 35.0f);
    door5.isOpen = true;
    door5.connectsRooms[0] = 8;
    door5.connectsRooms[1] = 3;
    doors.push_back(door5);
}

void Mansion::createHidingSpots() {
    hidingSpots.clear();
    
    // Closet in entrance hall
    HidingSpot closet1;
    closet1.position = Vector3(8.0f, 1.0f, 8.0f);
    closet1.radius = 1.5f;
    closet1.type = "closet";
    hidingSpots.push_back(closet1);
    
    // Under desk in study
    HidingSpot desk;
    desk.position = Vector3(32.0f, 0.5f, 20.0f);
    desk.radius = 1.2f;
    desk.type = "under_desk";
    hidingSpots.push_back(desk);
    
    // Behind bookshelf in library
    HidingSpot bookshelf;
    bookshelf.position = Vector3(23.0f, 1.0f, 33.0f);
    bookshelf.radius = 1.5f;
    bookshelf.type = "behind_furniture";
    hidingSpots.push_back(bookshelf);
    
    // Under bed in bedroom
    HidingSpot bed;
    bed.position = Vector3(47.0f, 0.3f, 37.0f);
    bed.radius = 1.8f;
    bed.type = "under_bed";
    hidingSpots.push_back(bed);
    
    // Closet in bedroom
    HidingSpot closet2;
    closet2.position = Vector3(43.0f, 1.0f, 40.0f);
    closet2.radius = 1.5f;
    closet2.type = "closet";
    hidingSpots.push_back(closet2);
    
    // Behind counter in kitchen
    HidingSpot kitchen_counter;
    kitchen_counter.position = Vector3(12.0f, 0.8f, 37.0f);
    kitchen_counter.radius = 1.3f;
    kitchen_counter.type = "behind_furniture";
    hidingSpots.push_back(kitchen_counter);
    
    // Behind lab equipment in basement
    HidingSpot lab_equip;
    lab_equip.position = Vector3(18.0f, -4.5f, 52.0f);
    lab_equip.radius = 1.4f;
    lab_equip.type = "behind_furniture";
    hidingSpots.push_back(lab_equip);
}

bool Mansion::isPlayerInRoom(const Vector3& playerPos, int roomIndex) const {
    if (roomIndex < 0 || roomIndex >= rooms.size()) {
        return false;
    }
    
    const Room& room = rooms[roomIndex];
    return checkCollision(playerPos, room.position, room.size);
}

bool Mansion::canPlayerMoveTo(const Vector3& from, const Vector3& to) const {
    // Simple wall collision - in full game would use proper collision detection
    // For now, just check if player is within mansion bounds
    return true;
}

HidingSpot* Mansion::getNearestHidingSpot(const Vector3& playerPos, float maxDistance) {
    HidingSpot* nearest = nullptr;
    float nearestDist = maxDistance;
    
    for (auto& spot : hidingSpots) {
        Vector3 diff = spot.position - playerPos;
        float dist = diff.length();
        
        if (dist < nearestDist) {
            nearestDist = dist;
            nearest = &spot;
        }
    }
    
    return nearest;
}

Vector3 Mansion::getRandomPatrolPoint() const {
    if (rooms.empty()) return Vector3(0, 0, 0);
    
    int randomRoom = rand() % rooms.size();
    return rooms[randomRoom].position;
}

std::vector<Vector3> Mansion::getMonsterPatrolPoints() const {
    std::vector<Vector3> points;
    
    // Create patrol route through mansion
    points.push_back(Vector3(10.0f, 1.0f, 10.0f));  // Entrance
    points.push_back(Vector3(18.0f, 1.0f, 20.0f));  // Hallway 1
    points.push_back(Vector3(25.0f, 1.0f, 30.0f));  // Library
    points.push_back(Vector3(35.0f, 1.0f, 25.0f));  // Hallway 2
    points.push_back(Vector3(45.0f, 1.0f, 35.0f));  // Bedroom
    points.push_back(Vector3(35.0f, 1.0f, 35.0f));  // Back to hallway
    points.push_back(Vector3(15.0f, 1.0f, 50.0f));  // Dining room
    points.push_back(Vector3(10.0f, 1.0f, 35.0f));  // Kitchen
    
    return points;
}

bool Mansion::checkCollision(const Vector3& pos, const Vector3& roomPos, const Vector3& roomSize) const {
    float halfW = roomSize.x / 2.0f;
    float halfD = roomSize.z / 2.0f;
    
    return (pos.x >= roomPos.x - halfW && pos.x <= roomPos.x + halfW &&
            pos.z >= roomPos.z - halfD && pos.z <= roomPos.z + halfD);
}
