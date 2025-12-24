# Quick Start Guide - Mansion Horror

Get up and running in 5 minutes!

## For GitHub Codespaces (Easiest)

1. **Open in Codespaces**
   - Click the "Code" button on GitHub
   - Select "Create codespace on main"
   - Wait for the environment to load

2. **Run the build script**
   ```bash
   cd horror-game
   chmod +x build.sh
   ./build.sh
   ```

3. **The script will:**
   - Install all dependencies
   - Build the game
   - Ask if you want to run it
   - Type 'y' to play!

That's it! 🎮

## For Local Development

### One-Command Build (Linux/macOS)
```bash
cd horror-game
chmod +x build.sh
./build.sh
```

### Manual Build

**Install dependencies first:**

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y build-essential cmake libsdl2-dev libgl1-mesa-dev libglu1-mesa-dev

# macOS
brew install cmake sdl2

# Fedora
sudo dnf install SDL2-devel mesa-libGL-devel mesa-libGLU-devel cmake gcc-c++
```

**Then build:**

```bash
cd horror-game
mkdir -p build
cd build
cmake ..
make
./MansionHorror
```

## First Time Playing

1. **Main Menu** appears first
2. Click **"PLAY"**
3. Choose **"DESKTOP (WASD + Mouse)"** or **"MOBILE (Touch)"**
4. Game starts!

## Basic Controls

### Desktop
- **WASD** - Move
- **Mouse** - Look
- **Shift** - Sprint
- **E** - Hide (near hiding spots)
- **F** - Interact/Complete tasks
- **ESC** - Pause

### Mobile
- **Left screen** - Movement joystick
- **Right screen** - Look around
- **Buttons** - Interact, hide, sprint

## Your First Mission

1. **Stay alive** - The monster patrols the mansion
2. **Find the study key** - It's in the entrance hall (yellow marker)
3. **Complete tasks** - 8 total tasks to escape
4. **Hide when scared** - Press E near closets, under desks, etc.
5. **Watch your stamina** - Don't sprint too much!

## Tips for Beginners

- **Yellow markers** = Task locations
- **Red eyes** = The monster
- **Health bar** = Top left (red/green)
- **Stamina bar** = Below health (blue)
- **"DANGER"** warning = Monster is near!
- **Hiding spots** = Blue-ish transparent cubes

## Troubleshooting

**Game won't build?**
- Run `./build.sh` - it will install missing dependencies

**Black screen?**
- Check your graphics drivers
- Try running in a window (modify CMakeLists.txt)

**Controls not working?**
- Make sure you clicked in the game window
- ESC to grab/ungrab mouse

**Need help?**
- Check README.md for detailed documentation
- Check DEVELOPMENT.md for technical details

## Next Steps

Once you've played through once:
- Read README.md for full documentation
- Check DEVELOPMENT.md to understand the code
- Explore the mansion layout in Mansion.cpp
- Tweak monster AI in Monster.cpp
- Add your own rooms and tasks!

## Ready to Play?

```bash
cd horror-game/build
./MansionHorror
```

Good luck escaping the mansion! 🏚️👻
