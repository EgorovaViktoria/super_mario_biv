# Super Mario BIV - Qt Version

This is a Qt6-based graphical version of the Super Mario game project.

## Prerequisites

- CMake 3.16 or higher
- Qt6 (Widgets component)
- C++17 compatible compiler

### Installing Qt6 on Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install -y qt6-base-dev
```

## Building

1. Create a build directory:
```bash
mkdir build
cd build
```

2. Configure with CMake:
```bash
cmake ../src
```

3. Build:
```bash
cmake --build . -j$(nproc)
```

Or use the provided build script:
```bash
./build.sh
```

## Running

After building, run the executable:
```bash
./build/super_mario
```

## Controls

- **Arrow Left / A**: Move Mario left
- **Arrow Right / D**: Move Mario right
- **Arrow Up / Space**: Jump
- **Escape**: Exit game

## Features

- Qt6-based graphical interface
- Game runs at ~60 FPS
- Smooth rendering with QPainter
- Keyboard input handling
- Multiple levels
- Various enemies (flying, jumping, regular)
- Collectible items (coins, boxes)

## Project Structure

- `src/main.cpp` - Main entry point with game loop
- `src/ui/qt/` - Qt UI implementation
  - `qt_game_map.*` - Main game widget (QWidget)
  - `qt_ui_factory.*` - Factory for creating Qt UI objects
  - `ui_objects/` - Qt wrappers for game entities
- `src/controller/` - Game logic
- `src/model/` - Game model classes
- `src/objects/` - Game objects (Mario, enemies, etc.)
- `src/levels/` - Level definitions

## Architecture

The project follows the MVC (Model-View-Controller) pattern with the UI layer being independent from the game logic. The Qt implementation is one possible UI backend - the game logic works independently of the UI framework used.
