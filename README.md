# Pinball

A small but well-structured Pinball game written in C++ using the SFML 2.x library.

## Project Structure

```
pinball/
├── assets/
│   ├── fonts/        ← place font.ttf here
│   ├── sounds/       ← place hit.wav and score.wav here
│   └── textures/     ← optional texture files
├── src/
│   ├── main.cpp
│   ├── Game.h / Game.cpp       – main game loop & event handling
│   ├── Ball.h / Ball.cpp       – ball physics (velocity, gravity)
│   ├── Flipper.h / Flipper.cpp – left/right flippers with rotation
│   ├── Physics.h / Physics.cpp – collision detection & resolution
│   └── UI.h / UI.cpp           – HUD, start screen, game-over overlay
└── CMakeLists.txt
```

## Dependencies

- **SFML 2.5+** – graphics, window, audio, system modules
- **CMake 3.16+**
- **C++17** compiler (GCC, Clang, MSVC)

### Install SFML on Ubuntu/Debian

```bash
sudo apt install libsfml-dev
```

## Build

```bash
cd pinball
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The executable `pinball` is placed in `build/`. Assets are copied automatically.

## Run

```bash
cd pinball/build
./pinball
```

> **Font note:** Place a `font.ttf` file in `assets/fonts/` before running.
> A free option: [Ubuntu Font](https://fonts.google.com/specimen/Ubuntu) or any TTF font.
> Without a font the game still runs but text will not be visible.

## Controls

| Key           | Action              |
|---------------|---------------------|
| `Left Arrow`  | Activate left flipper  |
| `Right Arrow` | Activate right flipper |
| `Space`       | Start / Restart game   |
| `Escape`      | Quit                   |

## Gameplay

- The ball falls under gravity and bounces off walls.
- Use the left and right flippers to keep the ball in play.
- Hitting bumpers awards points (25–100 per hit).
- The game ends when the ball falls below the screen.
- Your high score is tracked for the current session.

## Architecture

```
Game
├── Ball       – circular shape, velocity, gravity
├── Flipper    – rectangle shape, rotation, keyboard input
├── Physics    – wall & flipper collision resolution
└── UI         – score HUD, start screen, game-over overlay
```
