#!/bin/bash

# Build script for Emscripten web deployment
# This script builds the SDL2 Roguelike Survivor game for web using Emscripten

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}SDL2 Roguelike Survivor - Emscripten Build Script${NC}"
echo "=================================================="

# Check if Emscripten is available
if ! command -v emcc &> /dev/null; then
    echo -e "${RED}Error: Emscripten compiler (emcc) not found!${NC}"
    echo "Please install Emscripten and activate the environment:"
    echo "  1. Download emsdk from: https://github.com/emscripten-core/emsdk"
    echo "  2. ./emsdk install latest"
    echo "  3. ./emsdk activate latest"
    echo "  4. source ./emsdk_env.sh"
    exit 1
fi

echo -e "${GREEN}✓ Emscripten found:${NC} $(emcc --version | head -n1)"

# Configuration
BUILD_DIR="build_web"
BUILD_TYPE="${1:-Release}"  # Default to Release, can be overridden with Debug
PROJECT_NAME="SDL2_Roguelike_Survivor"

echo -e "${BLUE}Build configuration:${NC}"
echo "  Build type: $BUILD_TYPE"
echo "  Build directory: $BUILD_DIR"
echo "  Output: ${PROJECT_NAME}.html"

# Clean previous build
if [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Cleaning previous build...${NC}"
    rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo -e "${BLUE}Configuring CMake for Emscripten...${NC}"

# Configure with CMake
emcmake cmake .. \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_TOOLCHAIN_FILE="$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" \
    -DEMSCRIPTEN=ON

if [ $? -ne 0 ]; then
    echo -e "${RED}Error: CMake configuration failed!${NC}"
    exit 1
fi

echo -e "${BLUE}Building project...${NC}"

# Build with emmake
emmake make -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Build failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Build completed successfully!${NC}"

# Check if output files exist
if [ -f "${PROJECT_NAME}.html" ] && [ -f "${PROJECT_NAME}.js" ] && [ -f "${PROJECT_NAME}.wasm" ]; then
    echo -e "${GREEN}✓ Output files generated:${NC}"
    echo "  - ${PROJECT_NAME}.html (main page)"
    echo "  - ${PROJECT_NAME}.js (JavaScript loader)"
    echo "  - ${PROJECT_NAME}.wasm (WebAssembly binary)"
    echo "  - ${PROJECT_NAME}.data (asset data)"
    
    # Show file sizes
    echo -e "${BLUE}File sizes:${NC}"
    ls -lh ${PROJECT_NAME}.html ${PROJECT_NAME}.js ${PROJECT_NAME}.wasm ${PROJECT_NAME}.data 2>/dev/null || true
    
    echo ""
    echo -e "${GREEN}🎮 Game built successfully for web!${NC}"
    echo ""
    echo -e "${BLUE}To run the game:${NC}"
    echo "  1. Start a local web server in the build directory:"
    echo "     ${YELLOW}python3 -m http.server 8080${NC}"
    echo "  2. Open your browser and go to:"
    echo "     ${YELLOW}http://localhost:8080/${PROJECT_NAME}.html${NC}"
    echo ""
    echo -e "${YELLOW}Note: The game must be served from a web server due to CORS restrictions.${NC}"
    echo -e "${YELLOW}Opening the HTML file directly in a browser will not work.${NC}"
    
else
    echo -e "${RED}Error: Some output files are missing!${NC}"
    echo "Expected files:"
    echo "  - ${PROJECT_NAME}.html"
    echo "  - ${PROJECT_NAME}.js" 
    echo "  - ${PROJECT_NAME}.wasm"
    echo "  - ${PROJECT_NAME}.data"
    exit 1
fi