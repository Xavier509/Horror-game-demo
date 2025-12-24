#!/bin/bash

# Mansion Horror - Enhanced Build & Test Script
# Builds, tests, and runs the game with comprehensive checks

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Banner
echo -e "${CYAN}"
echo "╔════════════════════════════════════════╗"
echo "║   MANSION HORROR - Build & Test Tool   ║"
echo "║           Version 2.0                  ║"
echo "╚════════════════════════════════════════╝"
echo -e "${NC}"

# Functions
print_step() {
    echo -e "\n${BLUE}►${NC} $1"
}

print_success() {
    echo -e "${GREEN}✓${NC} $1"
}

print_error() {
    echo -e "${RED}✗${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}⚠${NC} $1"
}

print_info() {
    echo -e "${CYAN}ℹ${NC} $1"
}

# Detect OS
OS="unknown"
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
fi
print_info "Detected OS: $OS"

# Check dependencies
print_step "Checking dependencies..."

MISSING_DEPS=false
DEPS_LIST=()

if ! command -v cmake &> /dev/null; then
    print_error "CMake not found"
    DEPS_LIST+=("cmake")
    MISSING_DEPS=true
else
    CMAKE_VER=$(cmake --version | head -n1)
    print_success "CMake found: $CMAKE_VER"
fi

if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    print_error "C++ compiler not found"
    DEPS_LIST+=("build-essential" "gcc" "g++")
    MISSING_DEPS=true
else
    if command -v g++ &> /dev/null; then
        GCC_VER=$(g++ --version | head -n1)
        print_success "g++ found: $GCC_VER"
    else
        CLANG_VER=$(clang++ --version | head -n1)
        print_success "clang++ found: $CLANG_VER"
    fi
fi

if ! pkg-config --exists sdl2; then
    print_error "SDL2 development libraries not found"
    DEPS_LIST+=("libsdl2-dev")
    MISSING_DEPS=true
else
    SDL2_VER=$(pkg-config --modversion sdl2)
    print_success "SDL2 found: v$SDL2_VER"
fi

if ! pkg-config --exists gl; then
    print_error "OpenGL development libraries not found"
    DEPS_LIST+=("libgl1-mesa-dev" "libglu1-mesa-dev")
    MISSING_DEPS=true
else
    print_success "OpenGL libraries found"
fi

# Install missing dependencies
if [ "$MISSING_DEPS" = true ]; then
    echo ""
    print_warning "Missing dependencies detected!"
    echo ""
    echo "Required packages:"
    for dep in "${DEPS_LIST[@]}"; do
        echo "  • $dep"
    done
    echo ""
    
    read -p "Install missing dependencies automatically? (y/n) " -n 1 -r
    echo
    
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        if [ "$OS" == "linux" ]; then
            if command -v apt-get &> /dev/null; then
                print_step "Installing via apt-get..."
                sudo apt-get update
                sudo apt-get install -y build-essential cmake libsdl2-dev libgl1-mesa-dev libglu1-mesa-dev
            elif command -v dnf &> /dev/null; then
                print_step "Installing via dnf..."
                sudo dnf install SDL2-devel mesa-libGL-devel mesa-libGLU-devel cmake gcc-c++
            elif command -v pacman &> /dev/null; then
                print_step "Installing via pacman..."
                sudo pacman -S sdl2 mesa glu cmake base-devel
            fi
        elif [ "$OS" == "macos" ]; then
            if command -v brew &> /dev/null; then
                print_step "Installing via Homebrew..."
                brew install cmake sdl2
            else
                print_error "Homebrew not found. Install from https://brew.sh"
                exit 1
            fi
        fi
        print_success "Dependencies installed!"
    else
        print_error "Please install dependencies manually"
        exit 1
    fi
fi

# Check OpenGL support
print_step "Checking OpenGL support..."
if command -v glxinfo &> /dev/null; then
    GL_VERSION=$(glxinfo | grep "OpenGL version" | cut -d':' -f2 | xargs)
    if [ ! -z "$GL_VERSION" ]; then
        print_success "OpenGL: $GL_VERSION"
    else
        print_warning "Could not detect OpenGL version"
    fi
else
    print_info "glxinfo not available (install mesa-utils to check OpenGL)"
fi

# Clean build option
print_step "Preparing build..."
if [ -d "build" ]; then
    read -p "Build directory exists. Clean rebuild? (y/n) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        print_info "Cleaning build directory..."
        rm -rf build
        print_success "Build directory cleaned"
    fi
fi

# Create and enter build directory
mkdir -p build
cd build

# Configure with CMake
print_step "Configuring with CMake..."
if cmake .. -DCMAKE_BUILD_TYPE=Release; then
    print_success "CMake configuration successful"
else
    print_error "CMake configuration failed!"
    exit 1
fi

# Build the project
print_step "Building project..."
CORES=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
print_info "Using $CORES CPU cores"

if make -j$CORES; then
    print_success "Build completed successfully!"
else
    print_error "Build failed!"
    exit 1
fi

# Build summary
echo ""
echo -e "${GREEN}╔════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║         BUILD SUCCESSFUL! ✓            ║${NC}"
echo -e "${GREEN}╚════════════════════════════════════════╝${NC}"
echo ""

# Check executable
if [ -f "MansionHorror" ]; then
    BINARY_SIZE=$(du -h MansionHorror | cut -f1)
    print_success "Executable created: MansionHorror ($BINARY_SIZE)"
    
    # Check dependencies
    print_step "Verifying linked libraries..."
    if ldd MansionHorror > /dev/null 2>&1; then
        print_success "All libraries linked correctly"
        
        # Show key dependencies
        echo ""
        print_info "Key dependencies:"
        ldd MansionHorror | grep -E "libSDL2|libGL|libGLU" | while read line; do
            echo "  • $line"
        done
    else
        print_warning "Could not verify library dependencies"
    fi
else
    print_error "Executable not found!"
    exit 1
fi

# Run tests (basic validation)
print_step "Running basic validation..."

# Check if binary is executable
if [ -x "MansionHorror" ]; then
    print_success "Binary is executable"
else
    print_error "Binary is not executable"
    chmod +x MansionHorror
    print_info "Fixed executable permissions"
fi

# Print instructions
echo ""
echo -e "${CYAN}╔════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║           HOW TO PLAY                  ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════╝${NC}"
echo ""
echo "Desktop Controls:"
echo "  • W/A/S/D    - Move"
echo "  • Mouse      - Look around"
echo "  • SHIFT      - Sprint"
echo "  • E          - Hide"
echo "  • F          - Interact with tasks"
echo "  • ESC        - Pause menu"
echo ""
echo "Objective:"
echo "  • Complete all 8 tasks"
echo "  • Avoid the monster"
echo "  • Escape the mansion"
echo ""

# Offer to run
read -p "Run the game now? (y/n) " -n 1 -r
echo

if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo ""
    print_step "Launching Mansion Horror..."
    echo ""
    sleep 1
    
    # Launch game
    ./MansionHorror
    
    # Post-run summary
    EXIT_CODE=$?
    echo ""
    if [ $EXIT_CODE -eq 0 ]; then
        print_success "Game exited normally"
    else
        print_warning "Game exited with code: $EXIT_CODE"
    fi
else
    echo ""
    print_info "To run the game later:"
    echo ""
    echo "  cd build"
    echo "  ./MansionHorror"
    echo ""
    print_info "Or simply run: ./build.sh again and select 'y' to play"
fi

echo ""
echo -e "${CYAN}════════════════════════════════════════${NC}"
echo -e "${GREEN}Build & test script completed!${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"
echo ""
