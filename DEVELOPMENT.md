# Development Guide - Mansion Horror

Complete technical documentation for developers.

## Architecture Overview

### Design Pattern
The game uses an **Entity-Component-like** architecture, similar to Unreal Engine's Actor/Component system:

- **Game** - Main controller (like GameMode)
- **Player** - Player character controller (like PlayerController + Pawn)
- **Monster** - AI-controlled enemy (like AIController + Pawn)
- **TaskSystem** - Quest/objective manager
- **Mansion** - Level data and collision
- **Renderer** - Graphics subsystem
- **InputHandler** - Input abstraction layer
- **Menu** - UI/UMG equivalent
- **AudioManager** - Sound system

### Core Game Loop

```
Initialize → Main Menu → Playing → Update Loop → Render → Victory/Game Over
                ↑______________|
```

**Update Loop:**
1. Handle input events
2. Update player position/state
3. Update monster AI
4. Check task completion
5. Handle collisions
6. Update audio
7. Render frame

## Key Systems

### 1. Player System (Player.h/cpp)

**Movement:**
- WASD input → direction vector
- Transform by yaw rotation
- Apply to velocity
- Update position with delta time
- Simple gravity simulation

**Key Properties:**
- `position` - World position
- `yaw`, `pitch` - Camera rotation
- `stamina` - Sprint resource
- `health` - Player HP
- `hiding` - Is player hidden?

**Stamina System:**
```cpp
if (sprinting) {
    stamina -= drainRate * deltaTime;
} else {
    stamina += regenRate * deltaTime;
}
```

### 2. Monster AI (Monster.h/cpp)

**State Machine:**
```
PATROL ──sees/hears player──> CHASE ──loses player──> SEARCH
  ↑                              ↓                       ↓
  └──────────search timeout──────┴───────────────────────┘
```

**Detection Systems:**

**Vision:**
```cpp
bool canSeePlayer(playerPos, playerHiding) {
    if (playerHiding) return false;
    if (distance > detectionRadius) return false;
    if (angle > visionAngle) return false;
    return true;
}
```

**Hearing:**
```cpp
bool canHearPlayer(playerPos, playerSpeed) {
    return (playerSpeed > threshold && distance < hearingRadius) ||
           (distance < hearingRadius * 0.3);
}
```

**Patrol:**
- Follows predefined waypoints
- Waits at each point
- Cycles through patrol route

**Chase:**
- Moves directly toward player
- Uses `chaseSpeed` (faster than patrol)
- Updates last known position

**Search:**
- Moves to last known player position
- Has timeout duration
- Returns to patrol after timeout

**Key Tuning Parameters:**
```cpp
float moveSpeed = 3.0f;        // Patrol speed
float chaseSpeed = 6.0f;       // Chase speed (adjust difficulty)
float detectionRadius = 15.0f; // How far monster can see
float hearingRadius = 20.0f;   // How far monster can hear
float visionAngle = 60.0f;     // Field of view (degrees)
float searchDuration = 10.0f;  // How long to search
```

### 3. Task System (TaskSystem.h/cpp)

**Task Structure:**
```cpp
struct Task {
    string description;      // What to do
    Vector3 location;        // Where to do it
    float radius;            // Interaction range
    bool completed;          // Is it done?
    int id;                  // Unique identifier
};
```

**Task Flow:**
1. Player approaches task location
2. Press F within `interactionRadius`
3. Task marked complete
4. Next task becomes active
5. All tasks done → Victory

**Adding New Tasks:**
```cpp
void TaskSystem::initialize() {
    Task newTask;
    newTask.id = 9;
    newTask.description = "Find the secret passage";
    newTask.location = Vector3(x, y, z);
    newTask.radius = 2.5f;
    newTask.completed = false;
    tasks.push_back(newTask);
}
```

### 4. Mansion Layout (Mansion.h/cpp)

**Room System:**
```cpp
struct Room {
    Vector3 position;   // Center point
    Vector3 size;       // Width, height, depth
    string name;        // Room identifier
};
```

**Current Rooms:**
1. Entrance Hall (spawn point)
2. Study (locked initially)
3. Library
4. Master Bedroom
5. Kitchen
6. Dining Room
7. Basement Laboratory
8. Hallway 1
9. Hallway 2

**Adding New Rooms:**
```cpp
void Mansion::createRooms() {
    Room newRoom;
    newRoom.name = "Secret Chamber";
    newRoom.position = Vector3(60.0f, 0.0f, 60.0f);
    newRoom.size = Vector3(10.0f, 5.0f, 10.0f);
    rooms.push_back(newRoom);
}
```

**Hiding Spots:**
```cpp
struct HidingSpot {
    Vector3 position;
    float radius;
    string type;  // "closet", "under_bed", "cabinet"
};
```

### 5. Rendering System (Renderer.h/cpp)

**Graphics Pipeline:**
```
Setup Camera → Render 3D Scene → Render HUD → Swap Buffers
```

**3D Rendering:**
- OpenGL 2.1 (compatibility)
- Basic lighting (single point light)
- Fog for atmosphere
- Simple cube primitives

**HUD Elements:**
- Health bar (top left)
- Stamina bar (below health)
- Task counter
- Current objective text
- Danger warning
- Hiding indicator
- Crosshair

**Drawing Functions:**
```cpp
drawCube()      - Generic box primitive
drawFloor()     - Checkered floor pattern
drawWalls()     - Room walls
drawMonster()   - Monster representation
drawTaskMarker() - Yellow/green objective markers
```

### 6. Input System (InputHandler.h/cpp)

**Desktop Input:**
- Keyboard state tracking
- Mouse delta for camera
- "Just pressed" detection
- Relative mouse mode

**Mobile Touch:**
```cpp
struct Touch {
    int id;           // Touch identifier
    float x, y;       // Current position
    float startX, startY;  // Initial position
    bool active;      // Is touch active?
};
```

**Touch Zones:**
- Left half: Movement joystick
- Right half: Look control

### 7. Menu System (Menu.h/cpp)

**Menu Types:**
```cpp
enum MenuType {
    MAIN_MENU,
    PAUSE_MENU,
    CONTROL_SELECT,
    GAME_OVER_MENU,
    VICTORY_MENU
};
```

**Button System:**
```cpp
struct MenuButton {
    string text;
    int x, y, width, height;
    function<void()> onClick;
    bool hovered;
};
```

**Menu Flow:**
```
Main Menu → Control Select → Game Playing
                                   ↓
                         (ESC) ← Pause Menu
                                   ↓
                         Game Over / Victory
```

## Extending the Game

### Adding a New Room

1. **Define in Mansion.cpp:**
```cpp
Room secretRoom;
secretRoom.name = "Secret Laboratory";
secretRoom.position = Vector3(80.0f, 0.0f, 80.0f);
secretRoom.size = Vector3(15.0f, 5.0f, 15.0f);
rooms.push_back(secretRoom);
```

2. **Add doors/connections:**
```cpp
Door secretDoor;
secretDoor.position = Vector3(75.0f, 1.5f, 75.0f);
secretDoor.isOpen = false;  // Initially locked
secretDoor.connectsRooms[0] = existingRoomIndex;
secretDoor.connectsRooms[1] = newRoomIndex;
doors.push_back(secretDoor);
```

3. **Add hiding spots:**
```cpp
HidingSpot secretHiding;
secretHiding.position = Vector3(82.0f, 1.0f, 82.0f);
secretHiding.radius = 1.5f;
secretHiding.type = "closet";
hidingSpots.push_back(secretHiding);
```

4. **Update patrol points if needed**

### Adding a New Task

1. **In TaskSystem.cpp:**
```cpp
Task findSerum;
findSerum.id = 9;
findSerum.description = "Find the antidote serum";
findSerum.location = Vector3(85.0f, 1.0f, 85.0f);
findSerum.radius = 2.5f;
findSerum.completed = false;
tasks.push_back(findSerum);
```

2. **Add visual marker (automatic)**
3. **Test interaction range**

### Modifying Monster Difficulty

**Make Monster Easier:**
```cpp
moveSpeed = 2.5f;        // Slower patrol
chaseSpeed = 5.0f;       // Slower chase
detectionRadius = 12.0f; // Shorter sight
hearingRadius = 15.0f;   // Worse hearing
```

**Make Monster Harder:**
```cpp
moveSpeed = 3.5f;        // Faster patrol
chaseSpeed = 7.5f;       // Faster chase
detectionRadius = 20.0f; // Better sight
hearingRadius = 25.0f;   // Better hearing
searchDuration = 15.0f;  // Searches longer
```

**Make Monster Smarter:**
- Add more patrol points
- Decrease patrol wait time
- Implement pathfinding (A*)
- Add memory of player patterns
- Multiple search locations

### Adding Sound Effects

Currently AudioManager is a placeholder. To add real audio:

1. **Install SDL_mixer:**
```bash
sudo apt-get install libsdl2-mixer-dev
```

2. **Update CMakeLists.txt:**
```cmake
find_package(SDL2_mixer REQUIRED)
target_link_libraries(MansionHorror ${SDL2_MIXER_LIBRARIES})
```

3. **Implement in AudioManager.cpp:**
```cpp
#include <SDL2/SDL_mixer.h>

Mix_Chunk* sound = Mix_LoadWAV("assets/sounds/footstep.wav");
Mix_PlayChannel(-1, sound, 0);
```

### Adding Textures

1. **Load textures in Renderer::initialize():**
```cpp
GLuint textureID;
glGenTextures(1, &textureID);
// Load texture data...
glBindTexture(GL_TEXTURE_2D, textureID);
```

2. **Apply during rendering:**
```cpp
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, wallTexture);
// Draw geometry...
```

## Performance Optimization

### Current Optimizations
- Frame capping via VSync
- Simple collision (no complex physics)
- Culling (don't render distant monsters)
- Basic primitives (fast to draw)

### Further Optimizations
1. **Spatial partitioning** for collision
2. **Occlusion culling** for rooms
3. **Level of Detail** for distant objects
4. **Batch rendering** for similar objects
5. **Frustum culling** for off-screen objects

## Porting to Unreal Engine

### Phase 1: Project Setup
1. Create new UE5 First Person project
2. Set up version control
3. Create folder structure matching C++ structure

### Phase 2: Core Systems
**Player:**
```cpp
// Convert to UE C++
APlayerCharacter : public ACharacter {
    // Port Player.cpp logic here
};
```

**Monster:**
```cpp
AMonsterCharacter : public ACharacter {
    // Convert to AI Controller
    // Use Behavior Tree for state machine
};
```

**Tasks:**
```cpp
UTaskComponent : public UActorComponent {
    // Port TaskSystem logic
};
```

### Phase 3: Level Design
1. Block out mansion in UE Level Editor
2. Place spawn points
3. Add hiding spot actors
4. Set up task trigger volumes

### Phase 4: Polish
1. Replace primitives with 3D models
2. Add materials and textures
3. Implement proper lighting
4. Add sound effects
5. Create UI in UMG
6. Add particle effects

### Mapping C++ to Blueprint

**Player Movement:**
- Input events → Enhanced Input System
- Movement logic → Character Movement Component

**Monster AI:**
- State machine → Behavior Tree
- Detection → Perception Component
- Pathfinding → Navigation Mesh

**Tasks:**
- Task actors → Blueprint Actors
- Trigger volumes → Overlap events
- UI updates → UMG widgets

## Testing

### Unit Testing Approach
```cpp
// Test monster detection
void testMonsterVision() {
    Monster m(Vector3(0,0,0));
    Vector3 playerPos(10, 0, 0);
    bool canSee = m.canSeePlayer(playerPos, false);
    assert(canSee == true);  // Within range and FOV
}
```

### Integration Testing
1. **Player movement** - Can move in all directions?
2. **Monster AI** - Does it patrol, chase, search?
3. **Tasks** - Can complete all 8 tasks?
4. **Hiding** - Does hiding work? Monster ignores player?
5. **Victory** - Does game end when all tasks done?
6. **Game Over** - Does player die when caught?

### Performance Testing
- Target: 60 FPS
- Memory usage: <500MB
- No memory leaks over 30min play session

## Common Development Tasks

### Debugging Monster AI
```cpp
// Add to Monster::update()
std::cout << "State: " << (int)state 
          << " Dist: " << getDistanceToPlayer(playerPos)
          << " Alert: " << alertness << std::endl;
```

### Visualizing Collision
```cpp
// Add to Renderer::renderMansion()
glColor3f(1.0f, 0.0f, 0.0f);
for (auto& room : rooms) {
    // Draw wireframe boxes for room bounds
}
```

### Adjusting Camera
```cpp
// In Player::handleInput()
mouseSensitivity = 0.15f;  // Increase for faster look
pitch = clamp(pitch, -85.0f, 85.0f);  // Wider vertical range
```

## Build Configuration

### Debug Build
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
gdb ./MansionHorror
```

### Release Build
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

### With Profiling
```bash
cmake .. -DCMAKE_CXX_FLAGS="-pg"
make
./MansionHorror
gprof MansionHorror gmon.out > analysis.txt
```

## Contributing Guidelines

1. **Code Style:**
   - CamelCase for classes
   - camelCase for methods
   - snake_case for variables (optional)
   - Clear, descriptive names

2. **Comments:**
   - Explain WHY, not WHAT
   - Document public APIs
   - Add TODO for incomplete features

3. **Testing:**
   - Test new features locally
   - Verify on Linux/macOS/Windows if possible
   - Check FPS impact

4. **Commits:**
   - Clear commit messages
   - One feature per commit
   - Test before committing

## Resources

- [SDL2 Documentation](https://wiki.libsdl.org/)
- [OpenGL Tutorial](https://learnopengl.com/)
- [Unreal Engine Docs](https://docs.unrealengine.com/)
- [Game Programming Patterns](https://gameprogrammingpatterns.com/)

## Future Features

### Planned
- [ ] Save/Load system
- [ ] Multiple difficulty levels
- [ ] Achievements
- [ ] Story cutscenes
- [ ] More monsters/variations
- [ ] Additional levels
- [ ] Multiplayer co-op

### Considering
- [ ] VR support
- [ ] Procedural generation
- [ ] Custom level editor
- [ ] Modding support
- [ ] Speedrun timer
- [ ] Easter eggs

## Troubleshooting Development

**Linker errors:**
- Check CMakeLists.txt links all libraries
- Verify SDL2 installation

**Segmentation faults:**
- Check pointer dereferences
- Verify array bounds
- Use valgrind: `valgrind ./MansionHorror`

**Rendering issues:**
- Check OpenGL version support
- Verify depth testing enabled
- Check matrix stack state

---

Happy developing! 🎮👾
