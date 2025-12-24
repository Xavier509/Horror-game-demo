#ifndef MENU_H
#define MENU_H

#include "Game.h"
#include <string>
#include <vector>
#include <functional>

enum class MenuType {
    MAIN_MENU,
    PAUSE_MENU,
    SETTINGS_MENU,
    CONTROL_SELECT,
    GAME_OVER_MENU,
    VICTORY_MENU
};

struct MenuButton {
    std::string text;
    int x, y, width, height;
    std::function<void()> onClick;
    bool hovered;
};

class Menu {
public:
    Menu(Game* game);
    
    void update(const InputHandler& input);
    void render(Renderer& renderer);
    
    void setMenuType(MenuType type);
    MenuType getMenuType() const { return currentMenu; }
    
    void handleClick(int x, int y);
    void handleMouseMove(int x, int y);
    
private:
    void setupMainMenu();
    void setupPauseMenu();
    void setupControlSelect();
    void setupGameOverMenu();
    void setupVictoryMenu();
    
    void renderButton(const MenuButton& button, float baseR, float baseG, float baseB);
    void renderMainMenu(Renderer& renderer);
    void renderPauseMenu(Renderer& renderer);
    void renderControlSelect(Renderer& renderer);
    void renderGameOverMenu(Renderer& renderer);
    void renderVictoryMenu(Renderer& renderer);
    
    bool isPointInButton(int x, int y, const MenuButton& button);
    
    Game* game;
    MenuType currentMenu;
    std::vector<MenuButton> buttons;
    
    int screenWidth;
    int screenHeight;
};

#endif // MENU_H
