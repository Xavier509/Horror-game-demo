# Mansion Horror - First Person Survival Horror Game

A first-person horror game set in 2003 in a mysterious mansion. Escape from a corrupted scientist-turned-monster while completing objectives and discovering the dark truth.

## Game Overview

### Story
A scientist became corrupted by a sentient black slime substance, transforming him into a monster that now hunts through his mansion. You play as his son (revealed later in the full game), trying to escape while uncovering what happened.

### Objectives
- Complete 8 tasks throughout the mansion
- Avoid or hide from the monster
- Discover research notes and clues
- Escape through the front door

### Core Mechanics
- **First-person movement** - WASD + Mouse (Desktop) or Touch controls (Mobile)
- **Sprinting** - Hold Shift (drains stamina)
- **Hiding** - Press E near hiding spots (closets, under furniture, etc.)
- **Task interaction** - Press F when near task objectives
- **Monster AI** - Patrols, searches, and chases based on sight and sound
- **Stamina system** - Sprint carefully or you'll be caught
- **Health system** - Monster attacks damage you

## Requirements

### For GitHub Codespaces / Linux
- Ubuntu 24.04 or similar
- CMake 3.10+
- GCC/G++ with C++17 support
- SDL2 development libraries
- OpenGL development libraries

### For Local Development
Same as above, or:
- Windows: MinGW-w64 or Visual Studio with C++17
- macOS: Xcode with command line tools

## Installation & Setup

### In GitHub Codespaces

1. **Open this repository in Codespaces**

2. **Install dependencies:**
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libsdl2-dev libgl1-mesa-dev libglu1-mesa-dev
```

3. **Build the game:**
```bash
cd horror-game
mkdir -p build
cd build
cmake ..
make
```

4. **Run the game:**
```bash
./MansionHorror
```

### On Local Linux

1. **Clone the repository:**
```bash
git clone <your-repo-url>
cd horror-game
```

2. **Install dependencies:**
```bash
# Ubuntu/Debian
sudo apt-get install libsdl2-dev libgl1-mesa-dev libglu1-mesa-dev cmake build-essential

# Fedora
sudo dnf install SDL2-devel mesa-libGL-devel mesa-libGLU-devel cmake gcc-c++

# Arch
sudo pacman -S sdl2 mesa glu cmake base-devel
```

3. **Build and run:**
```bash
mkdir -p build
cd build
cmake ..
make
./MansionHorror
```

### On Windows

1. **Install dependencies:**
   - Download and install CMake from https://cmake.org/
   - Install MinGW-w64 or Visual Studio
   - Download SDL2 development libraries from https://www.libsdl.org/

2. **Build:**
```cmd
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
MansionHorror.exe
```

### On macOS

1. **Install dependencies:**
```bash
brew install cmake sdl2
```

2. **Build and run:**
```bash
mkdir -p build
cd build
cmake ..
make
./MansionHorror
```

## Controls

### Desktop Mode (Default)
- **W/A/S/D** - Move forward/left/backward/right
- **Mouse** - Look around
- **Left Shift** - Sprint (drains stamina)
- **E** - Hide in nearby hiding spot
- **F** - Interact with tasks
- **ESC** - Pause menu
- **Left Click** - Menu interaction

### Mobile Mode
- **Left side of screen** - Virtual joystick for movement
- **Right side of screen** - Touch and drag to look around
- **On-screen buttons** - Interact, hide, sprint

## Project Structure

```
horror-game/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── include/                # Header files
│   ├── Game.h             # Main game manager
│   ├── Player.h           # Player controller
│   ├── Monster.h          # Monster AI
│   ├── TaskSystem.h       # Objectives system
│   ├── Renderer.h         # OpenGL renderer
│   ├── InputHandler.h     # Input management
│   ├── Menu.h             # UI menus
│   ├── AudioManager.h     # Sound system
│   └── Mansion.h          # Level layout
├── src/                    # Implementation files
│   ├── main.cpp
│   ├── Game.cpp
│   ├── Player.cpp
│   ├── Monster.cpp
│   ├── TaskSystem.cpp
│   ├── Renderer.cpp
│   ├── InputHandler.cpp
│   ├── Menu.cpp
│   ├── AudioManager.cpp
│   └── Mansion.cpp
├── assets/                 # Game assets (future)
│   ├── textures/
│   ├── models/
│   └── sounds/
└── build/                  # Build output (generated)
```

## Porting to Unreal Engine

This game is designed to be ported to Unreal Engine later. The architecture follows Unreal Engine conventions:

1. **Class Structure** - Similar to Actor/Component pattern
2. **Game Flow** - Matches GameMode/GameState architecture
3. **Input System** - Can map directly to Enhanced Input system
4. **AI System** - Monster AI can translate to Behavior Trees
5. **Task System** - Maps to Quest/Objective systems

### Migration Steps:
1. Create UE5 project with First Person template
2. Migrate Player logic to Character Blueprint/C++ class
3. Convert Monster AI to AI Controller with Behavior Tree
4. Implement TaskSystem as Actor Components
5. Replace OpenGL rendering with UE5 materials/meshes
6. Add audio using UE5 audio system
7. Build mansion level in UE5 Level Editor

## Troubleshooting

### "SDL2 not found"
Install SDL2 development libraries for your platform (see Installation section).

### "OpenGL headers not found"
Install mesa development packages:
```bash
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev
```

### "undefined reference to SDL_main"
Make sure you're linking SDL2 correctly. Check CMakeLists.txt.

### Black screen on startup
This may be a graphics driver issue. Try updating your GPU drivers.

### Low FPS
The game uses basic OpenGL rendering. Should run at 60+ FPS on most modern systems.

## Development Roadmap

### Current (Stage 1) ✓
- Core movement and controls
- Monster AI with patrol/chase/search
- Task system with 8 objectives
- Hiding mechanic
- Basic mansion layout with 9 rooms
- Main menu and UI
- Desktop and mobile controls

### Future Stages
- **Stage 2**: Enhanced graphics, textures, better models
- **Stage 3**: Sound effects and atmospheric audio
- **Stage 4**: Additional levels and story progression
- **Stage 5**: Full lore integration and cutscenes
- **Stage 6**: Polish and Unreal Engine port

## Technical Details

- **Language**: C++17
- **Graphics**: OpenGL 2.1 (for compatibility)
- **Windowing**: SDL2
- **Architecture**: Entity-Component-like system
- **AI**: State machine with detection systems
- **Physics**: Simple collision and gravity

## Contributing

This is a personal project, but suggestions are welcome! Focus areas:
- Performance optimization
- Additional gameplay mechanics
- Level design improvements
- Better AI behaviors

## License

[Add your license here]

## Credits

Developed as a horror game prototype with plans for Unreal Engine migration.

## Contact

[Add your contact information]
