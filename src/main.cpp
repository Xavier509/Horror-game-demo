#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "==================================" << std::endl;
    std::cout << "  MANSION HORROR - 2003" << std::endl;
    std::cout << "  First Person Survival Horror" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << std::endl;
    
    Game game;
    
    if (!game.initialize()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }
    
    std::cout << "Game initialized successfully!" << std::endl;
    std::cout << "Starting game loop..." << std::endl;
    
    game.run();
    game.cleanup();
    
    std::cout << "Game ended. Thank you for playing!" << std::endl;
    
    return 0;
}
