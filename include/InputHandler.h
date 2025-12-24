#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL2/SDL.h>
#include <map>
#include <vector>

class InputHandler {
public:
    InputHandler();
    
    void update();
    void handleEvent(const SDL_Event& event);
    
    bool isKeyPressed(SDL_Keycode key) const;
    bool isKeyJustPressed(SDL_Keycode key) const;
    
    bool isMouseButtonPressed(int button) const;
    bool isMouseButtonJustPressed(int button) const;
    
    void getMouseDelta(int& deltaX, int& deltaY) const;
    void getMousePosition(int& x, int& y) const;
    
    // Touch controls for mobile
    struct Touch {
        int id;
        float x, y;
        float startX, startY;
        bool active;
    };
    
    const Touch* getTouch(int id) const;
    std::vector<Touch> getActiveTouches() const;
    
    void setMouseGrabbed(bool grabbed);
    
private:
    std::map<SDL_Keycode, bool> currentKeyState;
    std::map<SDL_Keycode, bool> previousKeyState;
    
    std::map<int, bool> currentMouseState;
    std::map<int, bool> previousMouseState;
    
    int mouseX, mouseY;
    int mouseDeltaX, mouseDeltaY;
    int lastMouseX, lastMouseY;
    
    std::map<int, Touch> touches;
    
    bool mouseGrabbed;
};

#endif // INPUT_HANDLER_H
