# MishPong

A 2-player pong game built in C using [raylib](https://www.raylib.com/). Made as a learning project to explore game loops, collision detection, and low-level C concepts.

![Menu Screen](screenshots/window_screenshot.png)
![Gameplay](screenshots/gameplay.png)

---

## About

This is not a polished product — it's a learning project. I built it from scratch while picking up C and raylib for the first time, starting from a bouncing circle and working up to a full 2-player game. The code reflects that journey: incremental, honest, and commented.

---

## Features

- 2-player local multiplayer
- Custom player name input on the menu screen
- First to 5 points wins
- Random ball direction on each serve
- Game over screen showing the winner's name
- Restart without closing the window

---

## Controls

| Action | Player 1 | Player 2 |
|--------|----------|----------|
| Move Up | `W` | `↑` |
| Move Down | `S` | `↓` |
| Start Game | `Enter` | — |
| Restart | `R` | — |

---

## Building

**Requirements:**
- macOS (tested on M4)
- [raylib](https://www.raylib.com/) installed via Homebrew
- clang

**Install raylib:**
```bash
brew install raylib
```

**Build and run:**
```bash
make
./game
```

---

## Project Structure

```
MishPong/
├── main.c          # Game loop, update and draw logic
├── ball.h          # Ball struct definition
├── paddle.h        # Paddle struct definition (includes name, points)
├── gameScreen.h    # GameScreen enum (MENU, PLAYING, GAME_OVER)
├── Makefile        # Build configuration
├── screenshots/    # Screenshots for this README
└── .gitignore
```

---

## What I Learned

- The game loop pattern — update state, then draw state, every frame
- Delta time for frame-rate independent movement
- Collision detection and resolution in C
- Structs and header files in C
- Scene management with an enum as single source of truth
- C string handling — `char` arrays, `strncpy`, null terminators
- Single responsibility — one function, one job, honest name

Full dev notes are in [`raylib_pong_devnote.md`](raylib_pong_devnote.md).

---

## Built With

- [raylib 5.x](https://www.raylib.com/) — simple and easy-to-use game library in C
- C99
- clang on macOS
