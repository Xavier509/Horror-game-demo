#include "Menu.h"
#include "Renderer.h"
#include "InputHandler.h"
#include <GL/gl.h>

Menu::Menu(Game* game)
    : game(game), currentMenu(MenuType::MAIN_MENU),
      screenWidth(game->getScreenWidth()), screenHeight(game->getScreenHeight()) {
    setupMainMenu();
}

void Menu::setMenuType(MenuType type) {
    currentMenu = type;
    buttons.clear();
    
    switch (type) {
        case MenuType::MAIN_MENU:
            setupMainMenu();
            break;
        case MenuType::PAUSE_MENU:
            setupPauseMenu();
            break;
        case MenuType::CONTROL_SELECT:
            setupControlSelect();
            break;
        case MenuType::GAME_OVER_MENU:
            setupGameOverMenu();
            break;
        case MenuType::VICTORY_MENU:
            setupVictoryMenu();
            break;
        default:
            break;
    }
}

void Menu::setupMainMenu() {
    int centerX = screenWidth / 2;
    int startY = screenHeight / 2 - 100;
    int buttonWidth = 200;
    int buttonHeight = 50;
    int spacing = 70;
    
    // Play button
    MenuButton playBtn;
    playBtn.text = "PLAY";
    playBtn.x = centerX - buttonWidth / 2;
    playBtn.y = startY;
    playBtn.width = buttonWidth;
    playBtn.height = buttonHeight;
    playBtn.hovered = false;
    playBtn.onClick = [this]() {
        setMenuType(MenuType::CONTROL_SELECT);
    };
    buttons.push_back(playBtn);
    
    // Settings button
    MenuButton settingsBtn;
    settingsBtn.text = "SETTINGS";
    settingsBtn.x = centerX - buttonWidth / 2;
    settingsBtn.y = startY + spacing;
    settingsBtn.width = buttonWidth;
    settingsBtn.height = buttonHeight;
    settingsBtn.hovered = false;
    settingsBtn.onClick = []() {
        // TODO: Implement settings menu
    };
    buttons.push_back(settingsBtn);
    
    // Quit button
    MenuButton quitBtn;
    quitBtn.text = "QUIT";
    quitBtn.x = centerX - buttonWidth / 2;
    quitBtn.y = startY + spacing * 2;
    quitBtn.width = buttonWidth;
    quitBtn.height = buttonHeight;
    quitBtn.hovered = false;
    quitBtn.onClick = []() {
        SDL_Event quitEvent;
        quitEvent.type = SDL_QUIT;
        SDL_PushEvent(&quitEvent);
    };
    buttons.push_back(quitBtn);
}

void Menu::setupControlSelect() {
    int centerX = screenWidth / 2;
    int startY = screenHeight / 2 - 50;
    int buttonWidth = 250;
    int buttonHeight = 50;
    int spacing = 70;
    
    // Desktop controls button
    MenuButton desktopBtn;
    desktopBtn.text = "DESKTOP (WASD + Mouse)";
    desktopBtn.x = centerX - buttonWidth / 2;
    desktopBtn.y = startY;
    desktopBtn.width = buttonWidth;
    desktopBtn.height = buttonHeight;
    desktopBtn.hovered = false;
    desktopBtn.onClick = [this]() {
        game->setControlMode(ControlMode::DESKTOP);
        game->setState(GameState::PLAYING);
    };
    buttons.push_back(desktopBtn);
    
    // Mobile controls button
    MenuButton mobileBtn;
    mobileBtn.text = "MOBILE (Touch)";
    mobileBtn.x = centerX - buttonWidth / 2;
    mobileBtn.y = startY + spacing;
    mobileBtn.width = buttonWidth;
    mobileBtn.height = buttonHeight;
    mobileBtn.hovered = false;
    mobileBtn.onClick = [this]() {
        game->setControlMode(ControlMode::MOBILE);
        game->setState(GameState::PLAYING);
    };
    buttons.push_back(mobileBtn);
    
    // Back button
    MenuButton backBtn;
    backBtn.text = "BACK";
    backBtn.x = centerX - buttonWidth / 2;
    backBtn.y = startY + spacing * 2;
    backBtn.width = buttonWidth;
    backBtn.height = buttonHeight;
    backBtn.hovered = false;
    backBtn.onClick = [this]() {
        setMenuType(MenuType::MAIN_MENU);
    };
    buttons.push_back(backBtn);
}

void Menu::setupPauseMenu() {
    int centerX = screenWidth / 2;
    int startY = screenHeight / 2 - 50;
    int buttonWidth = 200;
    int buttonHeight = 50;
    int spacing = 70;
    
    // Resume button
    MenuButton resumeBtn;
    resumeBtn.text = "RESUME";
    resumeBtn.x = centerX - buttonWidth / 2;
    resumeBtn.y = startY;
    resumeBtn.width = buttonWidth;
    resumeBtn.height = buttonHeight;
    resumeBtn.hovered = false;
    resumeBtn.onClick = [this]() {
        game->setState(GameState::PLAYING);
    };
    buttons.push_back(resumeBtn);
    
    // Main menu button
    MenuButton mainMenuBtn;
    mainMenuBtn.text = "MAIN MENU";
    mainMenuBtn.x = centerX - buttonWidth / 2;
    mainMenuBtn.y = startY + spacing;
    mainMenuBtn.width = buttonWidth;
    mainMenuBtn.height = buttonHeight;
    mainMenuBtn.hovered = false;
    mainMenuBtn.onClick = [this]() {
        game->setState(GameState::MAIN_MENU);
        setMenuType(MenuType::MAIN_MENU);
    };
    buttons.push_back(mainMenuBtn);
}

void Menu::setupGameOverMenu() {
    int centerX = screenWidth / 2;
    int startY = screenHeight / 2;
    int buttonWidth = 200;
    int buttonHeight = 50;
    int spacing = 70;
    
    // Retry button
    MenuButton retryBtn;
    retryBtn.text = "RETRY";
    retryBtn.x = centerX - buttonWidth / 2;
    retryBtn.y = startY;
    retryBtn.width = buttonWidth;
    retryBtn.height = buttonHeight;
    retryBtn.hovered = false;
    retryBtn.onClick = [this]() {
        // Would reset game state here
        game->setState(GameState::PLAYING);
    };
    buttons.push_back(retryBtn);
    
    // Main menu button
    MenuButton mainMenuBtn;
    mainMenuBtn.text = "MAIN MENU";
    mainMenuBtn.x = centerX - buttonWidth / 2;
    mainMenuBtn.y = startY + spacing;
    mainMenuBtn.width = buttonWidth;
    mainMenuBtn.height = buttonHeight;
    mainMenuBtn.hovered = false;
    mainMenuBtn.onClick = [this]() {
        game->setState(GameState::MAIN_MENU);
        setMenuType(MenuType::MAIN_MENU);
    };
    buttons.push_back(mainMenuBtn);
}

void Menu::setupVictoryMenu() {
    int centerX = screenWidth / 2;
    int startY = screenHeight / 2 + 50;
    int buttonWidth = 200;
    int buttonHeight = 50;
    
    // Main menu button
    MenuButton mainMenuBtn;
    mainMenuBtn.text = "MAIN MENU";
    mainMenuBtn.x = centerX - buttonWidth / 2;
    mainMenuBtn.y = startY;
    mainMenuBtn.width = buttonWidth;
    mainMenuBtn.height = buttonHeight;
    mainMenuBtn.hovered = false;
    mainMenuBtn.onClick = [this]() {
        game->setState(GameState::MAIN_MENU);
        setMenuType(MenuType::MAIN_MENU);
    };
    buttons.push_back(mainMenuBtn);
}

void Menu::update(const InputHandler& input) {
    int mouseX, mouseY;
    input.getMousePosition(mouseX, mouseY);
    handleMouseMove(mouseX, mouseY);
}

void Menu::render(Renderer& renderer) {
    switch (currentMenu) {
        case MenuType::MAIN_MENU:
            renderMainMenu(renderer);
            break;
        case MenuType::PAUSE_MENU:
            renderPauseMenu(renderer);
            break;
        case MenuType::CONTROL_SELECT:
            renderControlSelect(renderer);
            break;
        case MenuType::GAME_OVER_MENU:
            renderGameOverMenu(renderer);
            break;
        case MenuType::VICTORY_MENU:
            renderVictoryMenu(renderer);
            break;
        default:
            break;
    }
}

void Menu::renderMainMenu(Renderer& renderer) {
    // Background overlay
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Semi-transparent background
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();
    
    // Title
    renderer.renderText("MANSION HORROR", screenWidth / 2 - 100, 150, 0.8f, 0.1f, 0.1f);
    renderer.renderText("2003", screenWidth / 2 - 30, 200, 0.6f, 0.6f, 0.6f);
    
    // Render buttons
    for (const auto& button : buttons) {
        float r = button.hovered ? 0.8f : 0.4f;
        float g = button.hovered ? 0.2f : 0.2f;
        float b = button.hovered ? 0.2f : 0.2f;
        
        glColor3f(r, g, b);
        glBegin(GL_QUADS);
        glVertex2f(button.x, button.y);
        glVertex2f(button.x + button.width, button.y);
        glVertex2f(button.x + button.width, button.y + button.height);
        glVertex2f(button.x, button.y + button.height);
        glEnd();
        
        renderer.renderText(button.text, button.x + 20, button.y + 20, 1.0f, 1.0f, 1.0f);
    }
}

void Menu::renderPauseMenu(Renderer& renderer) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();
    
    renderer.renderText("PAUSED", screenWidth / 2 - 50, 150, 1.0f, 1.0f, 1.0f);
    
    for (const auto& button : buttons) {
        float r = button.hovered ? 0.6f : 0.3f;
        float g = button.hovered ? 0.6f : 0.3f;
        float b = button.hovered ? 0.8f : 0.4f;
        
        glColor3f(r, g, b);
        glBegin(GL_QUADS);
        glVertex2f(button.x, button.y);
        glVertex2f(button.x + button.width, button.y);
        glVertex2f(button.x + button.width, button.y + button.height);
        glVertex2f(button.x, button.y + button.height);
        glEnd();
        
        renderer.renderText(button.text, button.x + 20, button.y + 20, 1.0f, 1.0f, 1.0f);
    }
}

void Menu::renderControlSelect(Renderer& renderer) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor4f(0.0f, 0.0f, 0.0f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();
    
    renderer.renderText("SELECT CONTROL MODE", screenWidth / 2 - 120, 150, 1.0f, 1.0f, 1.0f);
    
    for (const auto& button : buttons) {
        float r = button.hovered ? 0.8f : 0.4f;
        float g = button.hovered ? 0.6f : 0.4f;
        float b = button.hovered ? 0.2f : 0.2f;
        
        glColor3f(r, g, b);
        glBegin(GL_QUADS);
        glVertex2f(button.x, button.y);
        glVertex2f(button.x + button.width, button.y);
        glVertex2f(button.x + button.width, button.y + button.height);
        glVertex2f(button.x, button.y + button.height);
        glEnd();
        
        renderer.renderText(button.text, button.x + 20, button.y + 20, 1.0f, 1.0f, 1.0f);
    }
}

void Menu::renderGameOverMenu(Renderer& renderer) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor4f(0.2f, 0.0f, 0.0f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();
    
    renderer.renderText("YOU DIED", screenWidth / 2 - 70, 200, 1.0f, 0.0f, 0.0f);
    
    for (const auto& button : buttons) {
        float r = button.hovered ? 0.8f : 0.5f;
        
        glColor3f(r, 0.2f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(button.x, button.y);
        glVertex2f(button.x + button.width, button.y);
        glVertex2f(button.x + button.width, button.y + button.height);
        glVertex2f(button.x, button.y + button.height);
        glEnd();
        
        renderer.renderText(button.text, button.x + 20, button.y + 20, 1.0f, 1.0f, 1.0f);
    }
}

void Menu::renderVictoryMenu(Renderer& renderer) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor4f(0.0f, 0.2f, 0.0f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();
    
    renderer.renderText("YOU ESCAPED!", screenWidth / 2 - 100, 200, 0.2f, 1.0f, 0.2f);
    renderer.renderText("The truth about your father awaits...", screenWidth / 2 - 150, 250, 0.8f, 0.8f, 0.8f);
    
    for (const auto& button : buttons) {
        float g = button.hovered ? 0.8f : 0.5f;
        
        glColor3f(0.2f, g, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(button.x, button.y);
        glVertex2f(button.x + button.width, button.y);
        glVertex2f(button.x + button.width, button.y + button.height);
        glVertex2f(button.x, button.y + button.height);
        glEnd();
        
        renderer.renderText(button.text, button.x + 20, button.y + 20, 1.0f, 1.0f, 1.0f);
    }
}

void Menu::handleClick(int x, int y) {
    for (auto& button : buttons) {
        if (isPointInButton(x, y, button)) {
            button.onClick();
            break;
        }
    }
}

void Menu::handleMouseMove(int x, int y) {
    for (auto& button : buttons) {
        button.hovered = isPointInButton(x, y, button);
    }
}

bool Menu::isPointInButton(int x, int y, const MenuButton& button) {
    return x >= button.x && x <= button.x + button.width &&
           y >= button.y && y <= button.y + button.height;
}
