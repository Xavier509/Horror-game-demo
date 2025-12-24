#ifndef MONSTER_H
#define MONSTER_H

#include "Game.h"
#include <random>

enum class MonsterState {
    PATROL,
    SEARCH,
    CHASE,
    ATTACK,
    IDLE
};

class Monster {
public:
    Monster(Vector3 startPos);
    
    void update(float deltaTime, const Vector3& playerPos, bool playerHiding);
    
    Vector3 getPosition() const { return position; }
    MonsterState getState() const { return state; }
    
    bool canSeePlayer(const Vector3& playerPos, bool playerHiding);
    bool canHearPlayer(const Vector3& playerPos, float playerSpeed);
    
    float getDistanceToPlayer(const Vector3& playerPos) const;
    
    void setPatrolPoints(const std::vector<Vector3>& points) { patrolPoints = points; }
    
private:
    void patrol(float deltaTime);
    void search(float deltaTime, const Vector3& lastKnownPos);
    void chase(float deltaTime, const Vector3& playerPos);
    void attack(float deltaTime);
    
    void updateState(const Vector3& playerPos, bool playerHiding, float deltaTime);
    Vector3 findPath(const Vector3& target);
    
    Vector3 position;
    Vector3 velocity;
    Vector3 lastKnownPlayerPos;
    
    MonsterState state;
    MonsterState previousState;
    
    float moveSpeed;
    float chaseSpeed;
    float detectionRadius;
    float attackRadius;
    float hearingRadius;
    float visionAngle;
    
    float searchTimer;
    float searchDuration;
    float alertness;
    
    std::vector<Vector3> patrolPoints;
    int currentPatrolIndex;
    float patrolWaitTime;
    float patrolWaitTimer;
    
    std::mt19937 rng;
};

#endif // MONSTER_H
