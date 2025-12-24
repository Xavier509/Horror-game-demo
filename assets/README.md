# Assets Directory

This directory is for game assets that will be loaded at runtime.

## Directory Structure

- **textures/** - Image files for textures (PNG, JPG, etc.)
- **models/** - 3D model files (OBJ, FBX, etc.)
- **sounds/** - Audio files (WAV, MP3, OGG, etc.)

## Future Assets to Add

### Textures
- Wall textures (wood, stone, wallpaper)
- Floor textures (carpet, tile, wood)
- Door textures
- Furniture textures
- Monster texture/skin

### Models
- Furniture (desks, beds, chairs, closets)
- Doors and windows
- Lab equipment
- Monster model
- Player hands/weapons (if added)

### Sounds
- Footstep sounds (walking, running)
- Door open/close sounds
- Monster growls and breathing
- Ambient mansion sounds
- Task completion sound
- Monster chase music
- Ambient horror music
- Death scream
- Victory music

## Adding Assets

1. Place files in the appropriate subfolder
2. Update the respective manager (AudioManager, Renderer, etc.)
3. Load assets during initialization
4. Reference assets by filename

Example:
```cpp
audioManager->playSound("assets/sounds/footstep.wav");
```

## Notes

- Currently the game uses placeholder graphics (colored cubes)
- AudioManager has placeholder functions ready for real audio
- When porting to Unreal Engine, these assets will be reimported into the Content folder
