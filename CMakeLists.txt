cmake_minimum_required(VERSION 3.10)
project(MansionHorror)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    include
)

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Monster.cpp
    src/TaskSystem.cpp
    src/Renderer.cpp
    src/InputHandler.cpp
    src/Menu.cpp
    src/AudioManager.cpp
    src/Mansion.cpp
)

# Create executable
add_executable(MansionHorror ${SOURCES})

# Link libraries
target_link_libraries(MansionHorror
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    GL
    GLU
    m
    pthread
)

# Copy assets to build directory
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
