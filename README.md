# SDL2 Roguelike Survivor

<div align="center">

![SDL2 Roguelike Survivor](https://img.shields.io/badge/SDL2-Roguelike%20Survivor-blue)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![SDL2](https://img.shields.io/badge/SDL2-2.0-green.svg)
![Emscripten](https://img.shields.io/badge/Emscripten-WebAssembly-orange.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

**A 2D top-down survival roguelike game built with SDL2 and C++17**

[🎮 Play Online](https://trananhkhoa2929.github.io/SDL2_Roguelike_Survivor) | [📖 Documentation](#documentation) | [🛠️ Development](#development)

</div>

## 🎯 Overview

SDL2 Roguelike Survivor is a fast-paced survival game where you fight endless waves of enemies, collect experience points, upgrade your weapons, and try to survive as long as possible. Built with modern C++17 and SDL2, the game features:

- **Real-time combat** with multiple weapon systems
- **Progressive difficulty** with dynamic enemy spawning
- **Character progression** with permanent upgrades
- **Procedurally generated maps** using Perlin noise
- **Cross-platform compatibility** (Windows, Linux, Web)
- **Web deployment** via Emscripten/WebAssembly

## 🎮 Gameplay Features

### Core Mechanics
- **Movement**: WASD or Arrow keys to navigate
- **Auto-combat**: Weapons automatically target nearby enemies
- **Experience System**: Kill enemies to gain XP and level up
- **Weapon Progression**: Choose from 3 starting weapons, unlock upgrades
- **Permanent Upgrades**: Spend gold on persistent improvements
- **Survival Challenge**: Endless waves with increasing difficulty

### Weapons Available
- **🔮 Magic Wand**: Automatically shoots projectiles at nearest enemy
- **🗡️ Whip**: Melee weapon that strikes horizontally  
- **🧄 Garlic**: Aura that damages all nearby enemies

### Enemy Types
- **Chaser**: Basic melee enemy that rushes toward player
- **Brute**: Tanky enemy with high health
- **Archer**: Ranged enemy that shoots arrows

### Map Features
- **Procedural Generation**: Each playthrough has a unique map
- **Environmental Hazards**: Lava tiles deal damage over time
- **Terrain Effects**: Water slows movement speed
- **Camera System**: Smooth following camera with world boundaries

## 🚀 Quick Start

### 🌐 Play in Browser (Recommended)

1. **Online Demo**: Visit [your-deployment-url]
2. **Local Web Version**:
   ```bash
   # Clone the repository
   git clone https://github.com/Trananhkhoa2929/SDL2_Roguelike_Survivor.git
   cd SDL2_Roguelike_Survivor
   
   # If you have Python 3
   python -m http.server 8000
   
   # Or Python 2
   python -m SimpleHTTPServer 8000
   
   # Open browser to localhost:8000
   ```

### 💻 Desktop Development

#### Prerequisites
- **Visual Studio 2022** (Windows)
- **SDL2 Development Libraries**
- **SDL2_image, SDL2_ttf** extensions
- **C++17 compatible compiler**

#### Windows (Visual Studio 2022)
```bash
# 1. Clone repository
git clone https://github.com/Trananhkhoa2929/SDL2_Roguelike_Survivor.git
cd SDL2_Roguelike_Survivor

# 2. Open SDL2Learning.sln in Visual Studio 2022

# 3. Configure SDL2 paths:
#    - Project Properties > Linker > General > Additional Library Directories
#    - Add: lib/SDL2/lib/x64 (for x64 builds)
#    - Project Properties > Linker > Input > Additional Dependencies
#    - Add: SDL2.lib; SDL2main.lib; SDL2_image.lib; SDL2_ttf.lib

# 4. Build and Run (F5)
```

#### Linux
```bash
# Install dependencies
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev

# Build
mkdir build && cd build
cmake ..
make -j4

# Run
./SDL2_Roguelike_Survivor
```

## 🌍 Web Deployment Guide

### Setting up Emscripten

1. **Install Emscripten**:
   ```bash
   # Download and install emsdk
   git clone https://github.com/emscripten-core/emsdk.git
   cd emsdk
   ./emsdk install latest
   ./emsdk activate latest
   source ./emsdk_env.sh
   ```

2. **Verify Installation**:
   ```bash
   em++ --version
   # Should show Emscripten compiler version
   ```

### Building for Web

1. **Prepare Assets**:
   ```bash
   # Ensure all assets use lowercase extensions
   cd Asset/
   # Rename any .PNG files to .png if needed
   ```

2. **Build Web Version**:
   ```bash
   # Clean previous build
   make clean -f Makefile.web
   
   # Build for web
   make -f Makefile.web
   
   # Files generated: index.html, index.js, index.wasm, index.data
   ```

3. **Test Locally**:
   ```bash
   # Serve files (required for CORS)
   python -m http.server 8000
   
   # Open browser to localhost:8000
   ```

4. **Deploy to GitHub Pages**:
   ```bash
   # Commit web files
   git add index.html index.js index.wasm index.data shell.html
   git commit -m "Add web build"
   git push origin main
   
   # Enable GitHub Pages in repository settings
   # Set source to "Deploy from branch: main"
   ```

### Web Build Configuration

The `Makefile.web` includes optimized settings for web deployment:

```makefile
# Key Emscripten flags used:
-s USE_SDL=2                    # Enable SDL2 port
-s USE_SDL_IMAGE=2              # Enable SDL2_image
-s USE_SDL_TTF=2                # Enable SDL2_ttf
-s WASM=1                       # Enable WebAssembly
-s ALLOW_MEMORY_GROWTH=1        # Dynamic memory allocation
--preload-file Asset@Asset      # Bundle assets
--shell-file shell.html         # Custom HTML template
```

## 🏗️ Project Structure

```
SDL2_Roguelike_Survivor/
├── 📁 Asset/                   # Game assets
│   ├── 📁 font/               # Font files
│   ├── 🖼️ *.png               # Sprite images
│   └── 🗺️ tileset_*.png        # Map tilesets
├── 📁 Headers/                # C++ header files
│   ├── 📁 Components/         # ECS Components
│   ├── 📁 Core/              # Core engine systems
│   ├── 📁 Systems/           # Game systems
│   ├── 📁 States/            # Game states
│   └── 📁 Utils/             # Utility classes
├── 📁 Sources/               # C++ source files
│   └── (mirrors Headers structure)
├── 📄 main.cpp               # Entry point
├── 📄 Makefile.web           # Web build configuration
├── 📄 shell.html             # Emscripten HTML template
└── 📄 SDL2Learning.sln       # Visual Studio solution
```

## ⚙️ Architecture

### Entity Component System (ECS)
- **Entities**: Game objects (Player, Enemies, Projectiles)
- **Components**: Data containers (Transform, Sprite, Health, AI)
- **Systems**: Logic processors (Movement, Collision, Rendering)

### Key Systems
- **RenderSystem**: Handles sprite rendering with camera
- **CollisionSystem**: AABB collision detection
- **AISystem**: Enemy behavior and pathfinding
- **WeaponControlSystem**: Weapon mechanics and timing
- **LevelSystem**: Experience and progression
- **EnemySpawnerSystem**: Dynamic enemy generation

### Memory Management
- **Smart Pointers**: Modern C++ memory safety
- **Component Pools**: Efficient ECS implementation
- **Asset Caching**: Optimized texture and font loading

## 🎨 Customization

### Adding New Weapons
1. Create component in `Headers/Components/Gameplay/WeaponComponents.h`
2. Implement logic in `WeaponControlSystem.cpp`
3. Add to upgrade registry in `UpgradeSystem.h`

### Adding New Enemies
1. Create AI component in `Headers/Components/AI/`
2. Implement behavior in corresponding AI system
3. Add spawn logic to `EnemySpawnerSystem.cpp`

### Modifying Game Balance
- Edit spawn rates in `GameEventSystem.cpp`
- Adjust weapon damage in component constructors
- Modify upgrade values in `UpgradeSystem.h`

## 🛠️ Development Tools

### Debugging
```bash
# Debug build with symbols
make -f Makefile.web CXXFLAGS="-g -O1 -DDEBUG"

# Browser developer console for web debugging
# Check Network tab for asset loading issues
```

### Performance Profiling
- **Browser DevTools**: Performance tab for web version
- **Visual Studio Profiler**: For desktop builds
- **Emscripten Profiler**: `--profiling` flag for detailed metrics

## 🐛 Troubleshooting

### Common Issues

**Assets not loading on web:**
```bash
# Ensure lowercase file extensions
cd Asset/
rename 's/\.PNG$/\.png/' *.PNG

# Rebuild
make clean -f Makefile.web && make -f Makefile.web
```

**Build errors with Visual Studio:**
- Verify SDL2 library paths in Project Properties
- Ensure x64 platform configuration matches SDL2 libraries
- Check C++17 standard is enabled

**Web build fails:**
```bash
# Verify Emscripten environment
source ~/emsdk/emsdk_env.sh
em++ --version

# Clean build directory
rm -rf build_web/
make -f Makefile.web
```

**Game crashes on startup:**
- Check console for asset loading errors
- Verify all required fonts and textures exist
- Ensure Asset/ directory structure is correct

## 📈 Performance Considerations

### Web Optimization
- **Asset Compression**: PNG images optimized for web
- **WASM Binary Size**: ~2-5MB typical build size
- **Memory Usage**: 128-256MB allocated dynamically
- **Frame Rate**: Target 60 FPS, scales on device capability

### Desktop Optimization
- **V-Sync**: Enabled by default for smooth gameplay
- **Asset Caching**: Textures loaded once, reused efficiently
- **Collision Optimization**: Spatial partitioning for large enemy counts

## 🤝 Contributing

1. Fork the repository
2. Create feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open Pull Request

### Code Style
- **C++17** modern standards
- **camelCase** for variables, **PascalCase** for classes
- **RAII** principles for resource management
- **const-correctness** where applicable

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **SDL2** - Cross-platform development library
- **Emscripten** - WebAssembly compilation toolchain  
- **Dear ImGui** - Immediate mode GUI (if used)
- **stb_image** - Image loading library
- **Community contributors** and testers

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/Trananhkhoa2929/SDL2_Roguelike_Survivor/issues)
- **Discussions**: [GitHub Discussions](https://github.com/Trananhkhoa2929/SDL2_Roguelike_Survivor/discussions)
- **Email**: [your-email@example.com]

---

<div align="center">

**Built with ❤️ using SDL2 and modern C++**

[⬆️ Back to Top](#sdl2-roguelike-survivor)

</div>
