#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <string>
#include <map>
#include "Game.h"

// Simple audio manager - in full implementation would use SDL_mixer or similar
class AudioManager {
public:
    AudioManager();
    ~AudioManager();
    
    void initialize();
    void cleanup();
    
    void playSound(const std::string& soundName);
    void playMusic(const std::string& musicName, bool loop = true);
    void stopMusic();
    
    void setMusicVolume(float volume);
    void setSoundVolume(float volume);
    
    void update();
    
    // Placeholder for 3D audio positioning
    void playSound3D(const std::string& soundName, const Vector3& position, const Vector3& listenerPos);
    
private:
    float musicVolume;
    float soundVolume;
    bool initialized;
    
    // In full implementation, would store audio buffers/sources
    std::map<std::string, int> soundBuffers;
};

#endif // AUDIO_MANAGER_H
