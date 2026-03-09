# FEKT-MPG

Moderní počítačová grafika (Modern Computer Graphics) – Cvičení 4: Komunikační prostředky, rastrová grafika v OpenGL.

## Overview

This project implements an OpenGL/GLUT application with Bezier curve editing and a colored rectangle mesh. It demonstrates user input handling (menus, keyboard, mouse), timers for animation, and double buffering.

## What Was Done

### Task 1: Bezier/Rect Context Menu
- Added a **Zobrazeni** submenu to the right-click context menu with entries **Bezier** and **Rect**
- Switches between displaying a cubic Bezier curve (with 4 control points) and a colored rectangle mesh in the same window
- Implemented in `createMenu()` using `glutCreateMenu`, `glutAddMenuEntry`, `glutAddSubMenu`
- `onMenu()` handles `MENU_BEZIER` and `MENU_RECT` by toggling the `bezier` variable

### Task 2: Keyboard Control for Triangle Strip
- Keys **w** and **s** toggle the rectangle rendering mode
- **w** → `trianglestrip = true` (draws as `GL_TRIANGLE_STRIP` – rectangle)
- **s** → `trianglestrip = false` (draws as `GL_TRIANGLES` – separate triangles)
- Implemented in `onKeyboard()` with a switch/case on the key

### Task 3: Mouse Drag for Control Points
- Control points of the Bezier curve can be moved by click-and-drag
- **onMouse()**: On left-button press, finds which control point was clicked using hit detection (20px radius)
- **onMotion()**: Updates the grabbed point’s position as the mouse moves
- Y-coordinate is flipped so the origin is bottom-left (OpenGL convention)

### Task 4: Timer Pause/Start Submenu
- Added an **Animace** submenu with **Spustit animaci** and **Pozastavit animaci**
- **Spustit animaci** sets `timerOn = true` and registers `glutTimerFunc` – rectangle bounces horizontally
- **Pozastavit animaci** sets `timerOn = false` – timer stops re-registering, animation pauses

### Additional Fix
- Replaced `#include <GLUT\glut.h>` with `#include <GLUT/glut.h>` for macOS compatibility (forward slashes)

## Build

Requirements: CMake 3.20+, OpenGL, GLUT (included with Xcode on macOS).

```bash
mkdir build && cd build
cmake ..
make
```

Or use CLion: open the project and build the `cv4_zadani` target.

## Run

```bash
./build/cv4_zadani
```

## How to Test

### Test Task 1 (Bezier/Rect Menu)
1. Right-click in the window → **Zobrazeni** submenu
2. Select **Bezier** → Bezier curve with 4 control points and dashed polygon
3. Select **Rect** → Colored rectangle mesh (orange gradient)
4. Confirm switching works in both directions

### Test Task 2 (Keyboard w/s)
1. Switch to Rect view (right-click → Zobrazeni → Rect)
2. Press **w** → Rectangle drawn as a strip (single connected shape)
3. Press **s** → Rectangle drawn as separate triangles (may look different)
4. Press **w** again to switch back

### Test Task 3 (Mouse Drag)
1. Switch to Bezier view (right-click → Zobrazeni → Bezier)
2. Click and hold on a red control point (within ~20px)
3. Drag the mouse → control point moves, Bezier curve updates in real time
4. Release → point stays; click elsewhere to deselect
5. Use **Reset pozice** (right-click menu) to restore default control points

### Test Task 4 (Timer Animation)
1. Switch to Rect view
2. Right-click → **Animace** → **Spustit animaci** → rectangle bounces left/right
3. Right-click → **Animace** → **Pozastavit animaci** → animation stops
4. Start again to resume

### Other Controls
- **Arrow keys**: Move the rectangle (when in Rect view); hold Shift/Ctrl/Alt for larger steps
- **Reset pozice**: Restores Bezier control points to default
- **Konec** → **Ano**: Exits the application

## Project Structure

```
mpc-mpg/
├── cv4_zadani.cpp    # Main application (all tasks)
├── simpleMesh.h      # Vertex coordinates and colors for rectangle mesh
├── CMakeLists.txt
├── cv4_2026.txt      # Assignment text (converted from PDF)
└── README.md
```
