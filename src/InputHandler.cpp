#include "InputHandler.h"

InputHandler::InputHandler()
    : mouseX(0), mouseY(0), mouseDeltaX(0), mouseDeltaY(0),
      lastMouseX(0), lastMouseY(0), mouseGrabbed(false) {
}

void InputHandler::update() {
    // Copy current to previous for "just pressed" detection
    previousKeyState = currentKeyState;
    previousMouseState = currentMouseState;
    
    // Reset mouse delta
    mouseDeltaX = 0;
    mouseDeltaY = 0;
}

void InputHandler::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            currentKeyState[event.key.keysym.sym] = true;
            break;
            
        case SDL_KEYUP:
            currentKeyState[event.key.keysym.sym] = false;
            break;
            
        case SDL_MOUSEBUTTONDOWN:
            currentMouseState[event.button.button] = true;
            break;
            
        case SDL_MOUSEBUTTONUP:
            currentMouseState[event.button.button] = false;
            break;
            
        case SDL_MOUSEMOTION:
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            
            if (mouseGrabbed) {
                mouseDeltaX = event.motion.xrel;
                mouseDeltaY = event.motion.yrel;
            }
            break;
            
        case SDL_FINGERDOWN: {
            Touch touch;
            touch.id = event.tfinger.fingerId;
            touch.x = event.tfinger.x * 1280; // Assuming screen size
            touch.y = event.tfinger.y * 720;
            touch.startX = touch.x;
            touch.startY = touch.y;
            touch.active = true;
            touches[touch.id] = touch;
            break;
        }
            
        case SDL_FINGERUP:
            if (touches.find(event.tfinger.fingerId) != touches.end()) {
                touches[event.tfinger.fingerId].active = false;
                touches.erase(event.tfinger.fingerId);
            }
            break;
            
        case SDL_FINGERMOTION:
            if (touches.find(event.tfinger.fingerId) != touches.end()) {
                touches[event.tfinger.fingerId].x = event.tfinger.x * 1280;
                touches[event.tfinger.fingerId].y = event.tfinger.y * 720;
            }
            break;
    }
}

bool InputHandler::isKeyPressed(SDL_Keycode key) const {
    auto it = currentKeyState.find(key);
    return it != currentKeyState.end() && it->second;
}

bool InputHandler::isKeyJustPressed(SDL_Keycode key) const {
    auto current = currentKeyState.find(key);
    auto previous = previousKeyState.find(key);
    
    bool isCurrentlyPressed = (current != currentKeyState.end() && current->second);
    bool wasPreviouslyPressed = (previous != previousKeyState.end() && previous->second);
    
    return isCurrentlyPressed && !wasPreviouslyPressed;
}

bool InputHandler::isMouseButtonPressed(int button) const {
    auto it = currentMouseState.find(button);
    return it != currentMouseState.end() && it->second;
}

bool InputHandler::isMouseButtonJustPressed(int button) const {
    auto current = currentMouseState.find(button);
    auto previous = previousMouseState.find(button);
    
    bool isCurrentlyPressed = (current != currentMouseState.end() && current->second);
    bool wasPreviouslyPressed = (previous != previousMouseState.end() && previous->second);
    
    return isCurrentlyPressed && !wasPreviouslyPressed;
}

void InputHandler::getMouseDelta(int& deltaX, int& deltaY) const {
    deltaX = mouseDeltaX;
    deltaY = mouseDeltaY;
}

void InputHandler::getMousePosition(int& x, int& y) const {
    x = mouseX;
    y = mouseY;
}

const InputHandler::Touch* InputHandler::getTouch(int id) const {
    auto it = touches.find(id);
    if (it != touches.end()) {
        return &it->second;
    }
    return nullptr;
}

std::vector<InputHandler::Touch> InputHandler::getActiveTouches() const {
    std::vector<Touch> result;
    for (const auto& pair : touches) {
        if (pair.second.active) {
            result.push_back(pair.second);
        }
    }
    return result;
}

void InputHandler::setMouseGrabbed(bool grabbed) {
    mouseGrabbed = grabbed;
    SDL_SetRelativeMouseMode(grabbed ? SDL_TRUE : SDL_FALSE);
    
    if (grabbed) {
        SDL_ShowCursor(SDL_DISABLE);
    } else {
        SDL_ShowCursor(SDL_ENABLE);
    }
}
