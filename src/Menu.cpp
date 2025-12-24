#include "Menu.h"
#include "Renderer.h"
#include "InputHandler.h"
#include <GL/gl.h>
#include <cmath>

Menu::Menu(Game* game)
    : game(game), currentMenu(MenuType::MAIN_MENU),
      screenWidth(game->getScreenWidth()), screenHeight(game->getScreenHeight()) {
    setupMainMenu();
}

void Menu::setMenuType(MenuType type) {
    currentMenu = type;
    buttons.clear();
    
    switch (type) {
        case MenuType::MAIN_MENU: setupMainMenu(); break;
        case MenuType::PAUSE_MENU: setupPauseMenu(); break;
        case MenuType::CONTROL_SELECT: setupControlSelect(); break;
        case MenuType::GAME_OVER_MENU: setupGameOverMenu(); break;
        case MenuType::VICTORY_MENU: setupVictoryMenu(); break;
        default: break;
    }
}

void Menu::setupMainMenu() {
    int centerX = screenWidth / 2;
    int startY = screenHeight / 2 - 50;
    int buttonWidth = 240;
    int buttonHeight = 60;
    int spacing = 80;
    
    MenuButton playBtn;
    playBtn.text = "PLAY GAME";
    playBtn.x = centerX - buttonWidth / 2;
    playBtn.y = startY;
    playBtn.width = buttonWidth;
    playBtn.height = buttonHeight;
    playBtn.hovered = false;
    playBtn.onClick = [this]() { setMenuType(MenuType::CONTROL_SELECT); };
    buttons.push_back(playBtn);
    
    MenuButton quitBtn;
    quitBtn.text = "QUIT";
    quitBtn.x = centerX - buttonWidth / 2;
    quitBtn.y = startY + spacing;
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
    int startY = screenHeight / 2 - 70;
    int buttonWidth = 300;
    int buttonHeight = 60;
    int spacing = 80;
    
    MenuButton desktopBtn;
    desktopBtn.text = "DESKTOP CONTROLS";
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
    
    MenuButton mobileBtn;
    mobileBtn.text = "MOBILE CONTROLS";
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
    
    MenuButton backBtn;
    backBtn.text = "BACK";
    backBtn.x = centerX - buttonWidth / 2;
    backBtn.y = startY + spacing * 2;
    backBtn.width = buttonWidth;
    backBtn.height = buttonHeight;
    backBtn.hovered = false;
    backBtn.onClick = [this]() { setMenuType(MenuType::MAIN_MENU); };
    buttons.push_back(backBtn);
}

void Menu::setupPauseMenu() {
    int centerX = screenWidth / 2;
    int startY = screenHeight / 2 - 50;
    int buttonWidth = 240;
    int buttonHeight = 60;
    int spacing = 80;
    
    MenuButton resumeBtn;
    resumeBtn.text = "RESUME";
    resumeBtn.x = centerX - buttonWidth / 2;
    resumeBtn.y = startY;
    resumeBtn.width = buttonWidth;
    resumeBtn.height = buttonHeight;
    resumeBtn.hovered = false;
    resumeBtn.onClick = [this]() { game->setState(GameState::PLAYING); };
    buttons.push_back(resumeBtn);
    
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
    int startY = screenHeight / 2 + 50;
    int buttonWidth = 240;
    int buttonHeight = 60;
    int spacing = 80;
    
    MenuButton retryBtn;
    retryBtn.text = "RETRY";
    retryBtn.x = centerX - buttonWidth / 2;
    retryBtn.y = startY;
    retryBtn.width = buttonWidth;
    retryBtn.height = buttonHeight;
    retryBtn.hovered = false;
    retryBtn.onClick = [this]() { game->setState(GameState::PLAYING); };
    buttons.push_back(retryBtn);
    
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
    int startY = screenHeight / 2 + 80;
    int buttonWidth = 240;
    int buttonHeight = 60;
    
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
        case MenuType::MAIN_MENU: renderMainMenu(renderer); break;
        case MenuType::PAUSE_MENU: renderPauseMenu(renderer); break;
        case MenuType::CONTROL_SELECT: renderControlSelect(renderer); break;
        case MenuType::GAME_OVER_MENU: renderGameOverMenu(renderer); break;
        case MenuType::VICTORY_MENU: renderVictoryMenu(renderer); break;
        default: break;
    }
}

void Menu::renderButton(const MenuButton& btn, float baseR, float baseG, float baseB) {
    static float animTime = 0.0f;
    animTime += 0.05f;
    
    float hoverBoost = btn.hovered ? 0.3f : 0.0f;
    float pulse = btn.hovered ? 0.1f * sin(animTime * 3.0f) : 0.0f;
    
    // Button shadow
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(btn.x + 4, btn.y + 4);
    glVertex2f(btn.x + btn.width + 4, btn.y + 4);
    glVertex2f(btn.x + btn.width + 4, btn.y + btn.height + 4);
    glVertex2f(btn.x + 4, btn.y + btn.height + 4);
    glEnd();
    
    // Button background
    glColor3f(baseR + hoverBoost + pulse, baseG + hoverBoost + pulse, baseB + hoverBoost + pulse);
    glBegin(GL_QUADS);
    glVertex2f(btn.x, btn.y);
    glVertex2f(btn.x + btn.width, btn.y);
    glVertex2f(btn.x + btn.width, btn.y + btn.height);
    glVertex2f(btn.x, btn.y + btn.height);
    glEnd();
    
    // Button border
    glColor3f(0.8f, 0.8f, 0.8f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(btn.x, btn.y);
    glVertex2f(btn.x + btn.width, btn.y);
    glVertex2f(btn.x + btn.width, btn.y + btn.height);
    glVertex2f(btn.x, btn.y + btn.height);
    glEnd();
    
    // Button text
    int textX = btn.x + (btn.width - btn.text.length() * 8) / 2;
    int textY = btn.y + (btn.height - 16) / 2;
    
    // Text shadow
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(textX + 2, textY + 14);
    
    // Text
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(textX, textY + 12);
}

void Menu::renderMainMenu(Renderer& renderer) {
    static float titlePulse = 0.0f;
    titlePulse += 0.03f;
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Background with gradient
    glBegin(GL_QUADS);
    glColor3f(0.1f, 0.0f, 0.0f);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glColor3f(0.0f, 0.0f, 0.05f);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();
    
    // Title backdrop
    float titleGlow = 0.1f + 0.05f * sin(titlePulse);
    glColor4f(titleGlow, 0.0f, 0.0f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(screenWidth/2 - 350, 80);
    glVertex2f(screenWidth/2 + 350, 80);
    glVertex2f(screenWidth/2 + 350, 220);
    glVertex2f(screenWidth/2 - 350, 220);
    glEnd();
    
    // Title text
    float titleR = 0.9f + 0.1f * sin(titlePulse);
    glColor3f(titleR, 0.1f, 0.1f);
    glRasterPos2i(screenWidth/2 - 200, 130);
    
    glColor3f(0.7f, 0.7f, 0.7f);
    glRasterPos2i(screenWidth/2 - 80, 180);
    
    // Subtitle
    glColor3f(0.5f, 0.5f, 0.5f);
    glRasterPos2i(screenWidth/2 - 200, 250);
    
    // Render buttons
    for (const auto& btn : buttons) {
        renderButton(btn, 0.2f, 0.05f, 0.05f);
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
    
    // Semi-transparent overlay
    glColor4f(0.0f, 0.0f, 0.0f, 0.75f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();
    
    // Pause panel
    glColor4f(0.1f, 0.1f, 0.15f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(screenWidth/2 - 300, 100);
    glVertex2f(screenWidth/2 + 300, 100);
    glVertex2f(screenWidth/2 + 300, screenHeight - 100);
    glVertex2f(screenWidth/2 - 300, screenHeight - 100);
    glEnd();
    
    // Title
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(screenWidth/2 - 80, 170);
    
    for (const auto& btn : buttons) {
        renderButton(btn, 0.15f, 0.15f, 0.25f);
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
    
    // Background
    glBegin(GL_QUADS);
    glColor3f(0.05f, 0.05f, 0.08f);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glColor3f(0.02f, 0.02f, 0.05f);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();
    
    // Title panel
    glColor4f(0.15f, 0.1f, 0.1f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(screenWidth/2 - 300, 80);
    glVertex2f(screenWidth/2 + 300, 80);
    glVertex2f(screenWidth/2 + 300, 160);
    glVertex2f(screenWidth/2 - 300, 160);
    glEnd();
    
    glColor3f(1.0f, 0.9f, 0.5f);
    glRasterPos2i(screenWidth/2 - 180, 120);
    
    // Instructions
    glColor3f(0.8f, 0.8f, 0.8f);
    glRasterPos2i(screenWidth/2 - 220, 200);
    
    glColor3f(0.7f, 0.7f, 0.7f);
    glRasterPos2i(screenWidth/2 - 250, 230);
    
    for (const auto& btn : buttons) {
        if (btn.text == "BACK") {
            renderButton(btn, 0.2f, 0.1f, 0.1f);
        } else {
            renderButton(btn, 0.1f, 0.2f, 0.1f);
        }
    }
}

void Menu::renderGameOverMenu(Renderer& renderer) {
    static float deathPulse = 0.0f;
    deathPulse += 0.08f;
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Red tinted background
    float redPulse = 0.15f + 0.1f * sin(deathPulse);
    glBegin(GL_QUADS);
    glColor3f(redPulse, 0.0f, 0.0f);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glColor3f(0.05f, 0.0f, 0.0f);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();
    
    // Death message panel
    glColor4f(0.2f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(screenWidth/2 - 350, 120);
    glVertex2f(screenWidth/2 + 350, 120);
    glVertex2f(screenWidth/2 + 350, 280);
    glVertex2f(screenWidth/2 - 350, 280);
    glEnd();
    
    float textIntensity = 0.7f + 0.3f * sin(deathPulse * 2);
    glColor3f(textIntensity, 0.0f, 0.0f);
    glRasterPos2i(screenWidth/2 - 120, 180);
    
    glColor3f(0.6f, 0.6f, 0.6f);
    glRasterPos2i(screenWidth/2 - 200, 240);
    
    for (const auto& btn : buttons) {
        renderButton(btn, 0.3f, 0.05f, 0.05f);
    }
}

void Menu::renderVictoryMenu(Renderer& renderer) {
    static float victoryPulse = 0.0f;
    victoryPulse += 0.04f;
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Green victory background
    float greenPulse = 0.05f + 0.03f * sin(victoryPulse);
    glBegin(GL_QUADS);
    glColor3f(0.0f, greenPulse, 0.0f);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glColor3f(0.0f, 0.0f, 0.05f);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();
    
    // Victory panel
    glColor4f(0.0f, 0.2f, 0.0f, 0.85f);
    glBegin(GL_QUADS);
    glVertex2f(screenWidth/2 - 400, 100);
    glVertex2f(screenWidth/2 + 400, 100);
    glVertex2f(screenWidth/2 + 400, 320);
    glVertex2f(screenWidth/2 - 400, 320);
    glEnd();
    
    float titleGlow = 0.6f + 0.4f * sin(victoryPulse * 2);
    glColor3f(0.2f, titleGlow, 0.2f);
    glRasterPos2i(screenWidth/2 - 150, 160);
    
    glColor3f(0.7f, 0.7f, 0.7f);
    glRasterPos2i(screenWidth/2 - 250, 220);
    
    glColor3f(0.6f, 0.8f, 0.6f);
    glRasterPos2i(screenWidth/2 - 200, 270);
    
    for (const auto& btn : buttons) {
        renderButton(btn, 0.05f, 0.25f, 0.05f);
    }
}

void Menu::handleClick(int x, int y) {
    for (auto& btn : buttons) {
        if (isPointInButton(x, y, btn)) {
            btn.onClick();
            break;
        }
    }
}

void Menu::handleMouseMove(int x, int y) {
    for (auto& btn : buttons) {
        btn.hovered = isPointInButton(x, y, btn);
    }
}

bool Menu::isPointInButton(int x, int y, const MenuButton& btn) {
    return x >= btn.x && x <= btn.x + btn.width &&
           y >= btn.y && y <= btn.y + btn.height;
}
