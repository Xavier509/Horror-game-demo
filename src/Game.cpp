#include "Game.h"
#include "Player.h"
#include "Monster.h"
#include "TaskSystem.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "Menu.h"
#include "AudioManager.h"
#include "Mansion.h"
#include <iostream>

Game::Game() 
    : window(nullptr), glContext(nullptr), 
      screenWidth(1280), screenHeight(720),
      running(false), currentState(GameState::PLAYING),
      controlMode(ControlMode::DESKTOP), lastTime(0) {
}

Game::~Game() {
}

bool Game::initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    // Create window
    window = SDL_CreateWindow(
        "Mansion Horror - 2003",
        0,
        0,
        screenWidth,
        screenHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    SDL_RaiseWindow(window);
    
    // Create OpenGL context
    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "OpenGL context creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Make context current
    if (SDL_GL_MakeCurrent(window, glContext) < 0) {
        std::cerr << "Failed to make OpenGL context current: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Enable VSync
    SDL_GL_SetSwapInterval(1);
    
    // Initialize game systems
    renderer = std::make_unique<Renderer>(screenWidth, screenHeight);
    renderer->initialize();
    
    inputHandler = std::make_unique<InputHandler>();
    inputHandler->setMouseGrabbed(true);
    menu = std::make_unique<Menu>(this);
    audioManager = std::make_unique<AudioManager>();
    audioManager->initialize();
    
    mansion = std::make_unique<Mansion>();
    mansion->initialize();
    
    player = std::make_unique<Player>(Vector3(5.0f, 0.0f, 5.0f));
    monster = std::make_unique<Monster>(Vector3(50.0f, 0.0f, 50.0f));
    monster->setPatrolPoints(mansion->getMonsterPatrolPoints());
    
    taskSystem = std::make_unique<TaskSystem>();
    taskSystem->initialize();
    
    running = true;
    lastTime = SDL_GetTicks();
    
    std::cout << "All systems initialized!" << std::endl;
    return true;
}

void Game::run() {
    std::cout << "Entering game loop" << std::endl;
    int frameCount = 0;
    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        // Cap delta time to prevent large jumps
        if (deltaTime > 0.1f) deltaTime = 0.1f;
        
        handleEvents();
        update(deltaTime);
        render();
        
        SDL_GL_SwapWindow(window);
        
        frameCount++;
        if (frameCount % 60 == 0) {  // Print every second
            std::cout << "Frame: " << frameCount << std::endl;
        }
        if (frameCount >= 600) {  // Exit after ~10 seconds at 60fps
            running = false;
        }
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        inputHandler->handleEvent(event);
        
        if (event.type == SDL_QUIT) {
            running = false;
        }
        
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            if (currentState == GameState::PLAYING) {
                currentState = GameState::PAUSED;
                inputHandler->setMouseGrabbed(false);
                menu->setMenuType(MenuType::PAUSE_MENU);
            } else if (currentState == GameState::PAUSED) {
                currentState = GameState::PLAYING;
                inputHandler->setMouseGrabbed(true);
            }
        }
        
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            if (currentState == GameState::MAIN_MENU || currentState == GameState::PAUSED ||
                currentState == GameState::GAME_OVER || currentState == GameState::VICTORY) {
                menu->handleClick(event.button.x, event.button.y);
            }
        }
        
        if (event.type == SDL_MOUSEMOTION) {
            if (currentState == GameState::MAIN_MENU || currentState == GameState::PAUSED ||
                currentState == GameState::GAME_OVER || currentState == GameState::VICTORY) {
                menu->handleMouseMove(event.motion.x, event.motion.y);
            }
        }
    }
}

void Game::update(float deltaTime) {
    inputHandler->update();
    audioManager->update();
    
    if (currentState == GameState::PLAYING) {
        // Update player
        player->handleInput(*inputHandler, deltaTime, controlMode);
        player->update(deltaTime);
        
        // Check hiding spots
        HidingSpot* nearestSpot = mansion->getNearestHidingSpot(player->getPosition(), 2.0f);
        if (nearestSpot && inputHandler->isKeyJustPressed(SDLK_e)) {
            player->setHiding(!player->isHiding());
            if (player->isHiding()) {
                audioManager->playSound("hide");
            }
        }
        
        // Update monster
        monster->update(deltaTime, player->getPosition(), player->isHiding());
        
        // Check if monster caught player
        float distToMonster = monster->getDistanceToPlayer(player->getPosition());
        if (distToMonster < 2.0f && !player->isHiding()) {
            player->takeDamage(30.0f * deltaTime);
            if (!player->isAlive()) {
                currentState = GameState::GAME_OVER;
                menu->setMenuType(MenuType::GAME_OVER_MENU);
                audioManager->stopMusic();
                audioManager->playSound("death");
            }
        }
        
        // Update tasks
        taskSystem->update(player->getPosition());
        
        // Check for task interaction
        if (inputHandler->isKeyJustPressed(SDLK_f)) {
            if (taskSystem->checkTaskCompletion(player->getPosition())) {
                audioManager->playSound("task_complete");
            }
        }
        
        // Check victory condition
        if (taskSystem->allTasksCompleted()) {
            currentState = GameState::VICTORY;
            menu->setMenuType(MenuType::VICTORY_MENU);
            audioManager->stopMusic();
            audioManager->playSound("victory");
        }
    } else if (currentState == GameState::MAIN_MENU || currentState == GameState::PAUSED ||
               currentState == GameState::GAME_OVER || currentState == GameState::VICTORY) {
        menu->update(*inputHandler);
    }
}

void Game::render() {
    renderer->beginFrame();
    
    if (currentState == GameState::PLAYING || currentState == GameState::PAUSED) {
        // Set camera
        renderer->setCamera(player->getPosition(), player->getYaw(), player->getPitch());
        
        // Render 3D scene
        renderer->renderMansion(mansion->getRooms(), mansion->getDoors());
        renderer->renderHidingSpots(mansion->getHidingSpots());
        renderer->renderTasks(taskSystem->getTasks());
        renderer->renderMonster(*monster, player->getPosition());
        
        // Render HUD
        if (currentState == GameState::PLAYING) {
            renderer->renderHUD(*player, *taskSystem, *monster);
            renderer->renderCrosshair();
        }
    }
    
    // Render menu overlay
    if (currentState != GameState::PLAYING) {
        menu->render(*renderer);
    }
    
    renderer->endFrame();
}

void Game::cleanup() {
    audioManager->cleanup();
    
    if (glContext) {
        SDL_GL_DeleteContext(glContext);
    }
    
    if (window) {
        SDL_DestroyWindow(window);
    }
    
    SDL_Quit();
}
