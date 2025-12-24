#include "Monster.h"
#include <cmath>
#include <random>
#include <algorithm>

Monster::Monster(Vector3 startPos)
    : position(startPos), velocity(0, 0, 0),
      lastKnownPlayerPos(0, 0, 0),
      state(MonsterState::PATROL), previousState(MonsterState::PATROL),
      moveSpeed(3.0f), chaseSpeed(6.0f),
      detectionRadius(15.0f), attackRadius(2.0f),
      hearingRadius(20.0f), visionAngle(60.0f),
      searchTimer(0.0f), searchDuration(10.0f), alertness(0.0f),
      currentPatrolIndex(0), patrolWaitTime(3.0f), patrolWaitTimer(0.0f) {
    
    std::random_device rd;
    rng.seed(rd());
}

void Monster::update(float deltaTime, const Vector3& playerPos, bool playerHiding) {
    updateState(playerPos, playerHiding, deltaTime);
    
    switch (state) {
        case MonsterState::PATROL:
            patrol(deltaTime);
            break;
        case MonsterState::SEARCH:
            search(deltaTime, lastKnownPlayerPos);
            break;
        case MonsterState::CHASE:
            chase(deltaTime, playerPos);
            break;
        case MonsterState::ATTACK:
            attack(deltaTime);
            break;
        case MonsterState::IDLE:
            break;
    }
    
    // Update position
    position = position + velocity * deltaTime;
    
    // Decay alertness
    alertness = std::max(0.0f, alertness - 0.1f * deltaTime);
}

void Monster::updateState(const Vector3& playerPos, bool playerHiding, float deltaTime) {
    bool canSee = canSeePlayer(playerPos, playerHiding);
    bool canHear = canHearPlayer(playerPos, 5.0f); // Threshold speed for hearing
    float distToPlayer = getDistanceToPlayer(playerPos);
    
    // Increase alertness if can see or hear player
    if (canSee || canHear) {
        alertness = std::min(1.0f, alertness + 0.5f * deltaTime);
        lastKnownPlayerPos = playerPos;
    }
    
    // State transitions
    switch (state) {
        case MonsterState::PATROL:
            if (canSee && !playerHiding) {
                state = MonsterState::CHASE;
                searchTimer = 0;
            } else if (canHear) {
                state = MonsterState::SEARCH;
                searchTimer = 0;
            }
            break;
            
        case MonsterState::SEARCH:
            searchTimer += deltaTime;
            if (canSee && !playerHiding) {
                state = MonsterState::CHASE;
                searchTimer = 0;
            } else if (searchTimer > searchDuration) {
                state = MonsterState::PATROL;
                alertness = 0;
            }
            break;
            
        case MonsterState::CHASE:
            if (!canSee && !canHear) {
                state = MonsterState::SEARCH;
                searchTimer = 0;
            } else if (distToPlayer < attackRadius) {
                state = MonsterState::ATTACK;
            }
            break;
            
        case MonsterState::ATTACK:
            if (distToPlayer > attackRadius * 1.5f) {
                state = MonsterState::CHASE;
            }
            break;
            
        default:
            break;
    }
}

void Monster::patrol(float deltaTime) {
    if (patrolPoints.empty()) return;
    
    Vector3 targetPos = patrolPoints[currentPatrolIndex];
    Vector3 dirToTarget = targetPos - position;
    float distToTarget = dirToTarget.length();
    
    if (distToTarget < 2.0f) {
        // Reached patrol point, wait
        patrolWaitTimer += deltaTime;
        velocity = Vector3(0, 0, 0);
        
        if (patrolWaitTimer > patrolWaitTime) {
            patrolWaitTimer = 0;
            currentPatrolIndex = (currentPatrolIndex + 1) % patrolPoints.size();
        }
    } else {
        // Move towards patrol point
        Vector3 direction = dirToTarget.normalize();
        velocity = direction * moveSpeed;
    }
}

void Monster::search(float deltaTime, const Vector3& lastKnownPos) {
    Vector3 dirToLastKnown = lastKnownPos - position;
    float distToLastKnown = dirToLastKnown.length();
    
    if (distToLastKnown < 2.0f) {
        // Reached last known position, look around
        velocity = Vector3(0, 0, 0);
    } else {
        // Move towards last known position
        Vector3 direction = dirToLastKnown.normalize();
        velocity = direction * moveSpeed;
    }
}

void Monster::chase(float deltaTime, const Vector3& playerPos) {
    Vector3 dirToPlayer = playerPos - position;
    Vector3 direction = dirToPlayer.normalize();
    velocity = direction * chaseSpeed;
    lastKnownPlayerPos = playerPos;
}

void Monster::attack(float deltaTime) {
    // Attack animation/behavior would go here
    velocity = Vector3(0, 0, 0);
}

bool Monster::canSeePlayer(const Vector3& playerPos, bool playerHiding) {
    if (playerHiding) return false;
    
    float distance = getDistanceToPlayer(playerPos);
    if (distance > detectionRadius) return false;
    
    // Simple line of sight check
    Vector3 dirToPlayer = (playerPos - position).normalize();
    
    // Check angle (simplified - would need proper forward vector)
    float angle = acos(dirToPlayer.z) * 180.0f / M_PI;
    
    return angle < visionAngle;
}

bool Monster::canHearPlayer(const Vector3& playerPos, float playerSpeed) {
    float distance = getDistanceToPlayer(playerPos);
    
    // Can hear if player is running and within hearing radius
    if (playerSpeed > 4.0f && distance < hearingRadius) {
        return true;
    }
    
    // Can hear if player is close regardless of speed
    if (distance < hearingRadius * 0.3f) {
        return true;
    }
    
    return false;
}

float Monster::getDistanceToPlayer(const Vector3& playerPos) const {
    return (playerPos - position).length();
}

Vector3 Monster::findPath(const Vector3& target) {
    // Simple direct path - in full game would use A* pathfinding
    Vector3 direction = (target - position).normalize();
    return direction;
}
