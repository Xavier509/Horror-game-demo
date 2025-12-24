#!/bin/bash

# Mansion Horror Build Script
# This script handles dependency installation, building, and running the game

set -e  # Exit on error

echo "=========================================="
echo "  Mansion Horror - Build Script"
echo "=========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[*]${NC} $1"
}

print_error() {
    echo -e "${RED}[!]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[!]${NC} $1"
}

# Detect OS
OS="unknown"
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
else
    OS="other"
fi

print_status "Detected OS: $OS"

# Check for dependencies
print_status "Checking dependencies..."

MISSING_DEPS=false

if ! command -v cmake &> /dev/null; then
    print_error "CMake not found"
    MISSING_DEPS=true
fi

if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    print_error "C++ compiler not found"
    MISSING_DEPS=true
fi

# Check for SDL2
if ! pkg-config --exists sdl2; then
    print_error "SDL2 development libraries not found"
    MISSING_DEPS=true
fi

if [ "$MISSING_DEPS" = true ]; then
    echo ""
    print_warning "Missing dependencies detected!"
    echo ""
    echo "To install dependencies:"
    echo ""
    
    if [ "$OS" == "linux" ]; then
        echo "  Ubuntu/Debian:"
        echo "    sudo apt-get update"
        echo "    sudo apt-get install -y build-essential cmake libsdl2-dev libgl1-mesa-dev libglu1-mesa-dev"
        echo ""
        echo "  Fedora:"
        echo "    sudo dnf install SDL2-devel mesa-libGL-devel mesa-libGLU-devel cmake gcc-c++"
        echo ""
        echo "  Arch:"
        echo "    sudo pacman -S sdl2 mesa glu cmake base-devel"
    elif [ "$OS" == "macos" ]; then
        echo "  macOS:"
        echo "    brew install cmake sdl2"
    fi
    
    echo ""
    read -p "Would you like to try installing dependencies automatically? (y/n) " -n 1 -r
    echo
    
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        if [ "$OS" == "linux" ]; then
            if command -v apt-get &> /dev/null; then
                print_status "Installing via apt-get..."
                sudo apt-get update
                sudo apt-get install -y build-essential cmake libsdl2-dev libgl1-mesa-dev libglu1-mesa-dev
            elif command -v dnf &> /dev/null; then
                print_status "Installing via dnf..."
                sudo dnf install SDL2-devel mesa-libGL-devel mesa-libGLU-devel cmake gcc-c++
            elif command -v pacman &> /dev/null; then
                print_status "Installing via pacman..."
                sudo pacman -S sdl2 mesa glu cmake base-devel
            fi
        elif [ "$OS" == "macos" ]; then
            if command -v brew &> /dev/null; then
                print_status "Installing via Homebrew..."
                brew install cmake sdl2
            else
                print_error "Homebrew not found. Please install from https://brew.sh"
                exit 1
            fi
        fi
    else
        print_error "Please install dependencies manually and run this script again."
        exit 1
    fi
fi

print_status "All dependencies found!"

# Create build directory
print_status "Creating build directory..."
mkdir -p build

# Run CMake
print_status "Running CMake..."
cd build
cmake .. || {
    print_error "CMake configuration failed!"
    exit 1
}

# Build the project
print_status "Building project..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4) || {
    print_error "Build failed!"
    exit 1
}

echo ""
print_status "Build successful!"
echo ""
echo "=========================================="
echo "  Build Complete!"
echo "=========================================="
echo ""
echo "To run the game:"
echo "  cd build"
echo "  ./MansionHorror"
echo ""

# Ask if user wants to run the game
read -p "Would you like to run the game now? (y/n) " -n 1 -r
echo

if [[ $REPLY =~ ^[Yy]$ ]]; then
    print_status "Launching Mansion Horror..."
    echo ""
    ./MansionHorror
fi
