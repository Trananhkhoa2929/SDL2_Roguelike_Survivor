# SDL2 Roguelike Survivor

A fast-paced roguelike survivor game built with SDL2 and C++, featuring Emscripten web build support.

## Features

- Entity Component System (ECS) architecture
- Multiple weapon types and upgrades
- Enemy AI with different behaviors
- Level progression and permanent upgrades
- Save/load system
- Cross-platform support (Desktop and Web)

## Building the Game

### Prerequisites

For desktop builds:
- CMake 3.16 or higher
- C++17 compatible compiler
- SDL2, SDL2_image, SDL2_ttf libraries (bundled in `lib/` directory)

For web builds:
- [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html)

### Desktop Build

1. Create build directory:
```bash
mkdir build
cd build
```

2. Configure with CMake:
```bash
cmake ..
```

3. Build:
```bash
make -j$(nproc)
```

4. Run:
```bash
./SDL2_Roguelike_Survivor
```

### Web Build (Emscripten)

1. Install and activate Emscripten:
```bash
# Download emsdk
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate latest version
./emsdk install latest
./emsdk activate latest

# Activate PATH and other environment variables
source ./emsdk_env.sh
```

2. Build for web:
```bash
# From the project root directory
./build_web.sh
```

Or manually:
```bash
mkdir build_web
cd build_web
emcmake cmake .. -DEMSCRIPTEN=ON
emmake make -j$(nproc)
```

3. Serve the game:
```bash
# In the build_web directory
python3 -m http.server 8080
```

4. Open in browser:
```
http://localhost:8080/SDL2_Roguelike_Survivor.html
```

**Note:** The web version must be served from a web server due to CORS restrictions. Opening the HTML file directly will not work.

## Game Controls

- **WASD / Arrow Keys:** Move character
- **Mouse:** Navigate menus and select upgrades
- **ESC:** Pause game
- **Click Pause Icon:** Pause game

## Gameplay

1. Choose your starting weapon from the main menu
2. Survive waves of enemies
3. Collect XP orbs to level up and choose upgrades
4. Collect gold coins for permanent upgrades in the shop
5. Try to survive as long as possible!

## Project Structure

```
SDL2_Roguelike_Survivor/
├── Asset/                  # Game assets (textures, fonts, maps)
│   ├── font/
│   └── [images]
├── Headers/                # Header files
│   ├── Components/
│   ├── Core/
│   ├── Events/
│   ├── Managers/
│   ├── States/
│   ├── Systems/
│   └── Utils/
├── Sources/                # Source files
│   ├── Components/
│   ├── Core/
│   ├── Events/
│   ├── Managers/
│   ├── States/
│   ├── Systems/
│   └── Utils/
├── lib/                    # Bundled SDL2 libraries
├── main.cpp                # Main entry point
├── CMakeLists.txt          # CMake build configuration
├── build_web.sh            # Emscripten build script
└── web_template.html       # HTML template for web builds
```

## Asset Loading

The game uses relative asset paths starting with `Asset/`. This ensures compatibility across different platforms and deployment methods:

- Desktop: Assets are copied to the build directory
- Web: Assets are preloaded and embedded in the WebAssembly build

## Architecture

The game uses an Entity Component System (ECS) architecture with the following key systems:

- **Render System:** Handles sprite and text rendering
- **Input System:** Processes player input
- **Physics System:** Manages movement and collisions
- **AI System:** Controls enemy behavior
- **Weapon System:** Manages weapon mechanics
- **Level System:** Handles progression and upgrades

## Emscripten Configuration

The web build includes the following optimizations:

- Asset preloading with `--preload-file`
- Memory management with growth limits
- SDL2 integration through Emscripten ports
- Custom HTML shell with game information and loading progress

## Troubleshooting

### Common Issues

1. **Font loading fails:** Ensure the font file exists at `Asset/font/your_font.ttf`
2. **Assets not found:** Check that asset paths are relative (start with `Asset/`)
3. **Web build doesn't load:** Make sure to serve from a web server, not open directly
4. **Build fails:** Verify all dependencies are installed and Emscripten is activated

### Debug Build

For debugging, use:
```bash
# Desktop debug build
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Web debug build
./build_web.sh Debug
```

Debug builds include additional logging and debugging symbols.

## License

This project is open source. See the repository for license details.

## Contributing

Contributions are welcome! Please ensure all changes maintain compatibility with both desktop and web builds.