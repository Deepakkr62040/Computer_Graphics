# Computer_Graphics
Mini Project

# Animated Village Scene

A 2D computer graphics project written in **C** using **OpenGL (GLUT)**. It renders an animated village with a moving car, drifting clouds, a rotating sun and a day/night mode.

## Features

- Day and night modes (toggle at any time)
  - Day: gradient blue sky and a sun with rotating rays
  - Night: dark sky, crescent moon, twinkling stars, lit house windows and a glowing car headlight beam (alpha blending)
- Car that drives along the road, with rotating wheel spokes and adjustable speed
- Clouds moving at different speeds and wrapping around the screen
- Scenery: house with roof, door, windows and chimney; trees; two hills; grass and a dashed road
- Keyboard interaction and an on-screen help line

## Requirements

- A C compiler (GCC or Clang)
- OpenGL and GLUT (freeglut on Linux and Windows)

### Installing the dependencies

| Platform | Command / Setup |
|----------|-----------------|
| Ubuntu / Debian | `sudo apt install build-essential freeglut3-dev` |
| Fedora | `sudo dnf install gcc freeglut-devel` |
| Windows (MinGW) | Download freeglut for MinGW, then copy the `include`, `lib` and `bin` files into your MinGW folders (or set include/library paths). |
| macOS | GLUT ships with Xcode command line tools: `xcode-select --install` |

## Build and Run

```bash
# Linux
gcc village_scene.c -o village -lGL -lGLU -lglut -lm

# Windows (MinGW)
gcc village_scene.c -o village -lfreeglut -lopengl32 -lglu32 -lm

# macOS
gcc village_scene.c -o village -framework OpenGL -framework GLUT -lm
```

Then run it:

```bash
./village        # Linux / macOS
village.exe      # Windows
```

If you use Code::Blocks or Dev-C++, create a GLUT project, add `village_scene.c`, and link `freeglut`, `opengl32`, `glu32`.
macOS may print deprecation warnings for OpenGL and GLUT; they are harmless.

## Controls

| Key | Action |
|-----|--------|
| `N` | Toggle day / night |
| `+` | Increase car speed |
| `-` | Decrease car speed |
| `Space` | Pause / resume animation |
| `Esc` | Quit |

## Project Structure

```
village_scene.c   # complete source code (single file)
README.md         # this file
```

Main parts of the code:

| Function | Purpose |
|----------|---------|
| `rect`, `tri`, `circle`, `text` | Drawing helpers for basic shapes and text |
| `drawSky`, `drawSunMoon`, `drawStars`, `cloud` | Sky and weather elements |
| `drawHills`, `drawGround`, `drawHouse`, `drawTree` | Scenery |
| `drawCar`, `drawWheel` | The animated car |
| `display` | Draws the whole scene each frame |
| `update` | Timer callback that advances the animation (about 60 FPS) |
| `keyboard` | Handles key input |
| `init`, `main` | Window, projection and callback setup |

## Computer Graphics Concepts Used

- **2D primitives:** `GL_QUADS`, `GL_TRIANGLES`, `GL_LINES`, `GL_POINTS`
- **Circle generation** using `GL_TRIANGLE_FAN` with `sin`/`cos`
- **Colour interpolation:** gradient sky using per-vertex colours
- **Geometric transformations:** `glTranslatef` and `glRotatef` with `glPushMatrix`/`glPopMatrix` (sun rays, wheel spokes)
- **Orthographic projection:** `gluOrtho2D(0, 1000, 0, 600)`
- **Animation:** `glutTimerFunc` with double buffering (`GLUT_DOUBLE`, `glutSwapBuffers`)
- **Alpha blending:** translucent headlight beam at night
- **Event handling:** keyboard callback

## Customisation Ideas

- Change window size with the `W` and `H` macros
- Adjust `carSpeed` (initial value) or the cloud speeds in `update`
- Add more houses or trees by calling `drawHouse(x, y)` and `drawTree(x, y)` in `display`
- Change colours in the individual draw functions

## Possible Extensions

- Rain or snow particles
- A train or a second car in the opposite direction
- Birds flying across the sky
- Mouse interaction, for example clicking to switch day/night
- A 3D version using perspective projection and lighting

## Troubleshooting

| Problem | Fix |
|---------|-----|
| `GL/glut.h: No such file or directory` | Install freeglut (see Requirements) |
| `undefined reference to glutInit` | Add `-lglut` (Linux) or `-lfreeglut` (Windows) to the link command |
| `freeglut.dll` not found (Windows) | Copy `freeglut.dll` next to the executable |
| Window closes immediately | Run it from a terminal to see error messages |

## Author
Deepak Kumar, 25051121010002, MCA/Parul University
Anand Kumar, 25051121010001, MCA/Parul University
Karan Kumar, 25051121010003, MCA/Parul University

