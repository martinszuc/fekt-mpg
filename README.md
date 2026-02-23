# CV3 – Základy vykreslování ve 2D, Bézierovy křivky

## Overview

Two OpenGL windows. Window 1 covers primitive rendering and vertex arrays. Window 2 covers Bézier curves and text rendering.

**Files:** `cv3_zadani.cpp`, `simpleMesh.h`

---

## Task 1 – Primitives & antialiasing (`opakovaniUkol1()`) — Window 1

Draw the following (with/without antialiasing toggled via `bool antialiasing`):

| What | Details |
|------|---------|
| 2 blue points | coords `(200,350)` and `(300,350)`, size 25, round via `GL_POINT_SMOOTH` |
| Green line | width 25, from `(150,300)` to `(350,250)` |
| Yellow dashed line | width 5, from `(50,250)` to `(350,200)`, stipple pattern 1:1 (alternating bits, e.g. `0xAAAA`) using `glLineStipple(factor, pattern)` + `glEnable(GL_LINE_STIPPLE)` |
| Red solid line | width 5, from `(50,150)` to `(350,100)` |
| Green solid line | width 5, from `(50,100)` to `(350,150)` |

Compare rendering quality with `antialiasing = true` vs `false`.

---

## Task 2 – Vertex arrays (`vykreslovaniPole()`) — Window 1

Using arrays from `simpleMesh.h` (`coords[]`, `colors[]`, `numVer`):

1. Register arrays with `glVertexPointer` and `glColorPointer`
2. Enable with `glEnableClientState(GL_VERTEX_ARRAY)` and `glEnableClientState(GL_COLOR_ARRAY)`
3. Draw **twice** (toggled by `bool trianglestrip`):
    - `GL_TRIANGLE_STRIP` → colored rectangle
    - `GL_TRIANGLES` → individual triangles from the same data

---

## Task 3 – Bézier curve (`vykresliBeziera()`) — Window 2

Draw a cubic Bézier curve using the **matrix formula** directly:

```
Q(t) = [t³, t², t, 1] · M_bez · [P0, P1, P2, P3]ᵀ
```

Expanded per axis:
```
x(t) = (-t³+3t²-3t+1)·X0 + (3t³-6t²+3t)·X1 + (-3t³+3t²)·X2 + t³·X3
y(t) = (-t³+3t²-3t+1)·Y0 + (3t³-6t²+3t)·Y1 + (-3t³+3t²)·Y2 + t³·Y3
```

Control points are defined in `ridiciBody[4][3]` (predefined, `t ∈ [0,1]`).

Also draw:
- **Control points** as `GL_POINTS` (red, size 10)
- **Control polygon** as dashed `GL_LINE_STRIP` connecting the 4 control points

---

## Task 4 – Text labels (`bitmapText()`) — Window 2

Next to each control point, render a label using `glutBitmapCharacter`:

```
Ridici bod: 0  X: 60  Y: 400
```

- Font: `GLUT_BITMAP_8_BY_13`
- Position: `glRasterPos2i(x, y)` right beside each control point
- Build string with `std::string` + `std::to_string()` for numbers
- Render char-by-char in a for loop over `str`

---

## Key OpenGL Functions

```cpp
// Points
glPointSize(float);
glEnable(GL_POINT_SMOOTH);

// Lines
glLineWidth(float);
glLineStipple(GLint factor, GLushort pattern);  // e.g. factor=1, pattern=0xAAAA
glEnable(GL_LINE_STIPPLE);

// Vertex arrays
glVertexPointer(2, GL_INT, 0, ptr);
glColorPointer(4, GL_FLOAT, 0, ptr);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_COLOR_ARRAY);
glDrawArrays(GL_TRIANGLE_STRIP, 0, numVer);
glDrawArrays(GL_TRIANGLES, 0, numVer);

// Text
glRasterPos2i(x, y);
glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ch);
```

---

## Build

```bash
# From project root (CLion or terminal)
mkdir -p build && cd build
cmake .. && make cv3_zadani
./cv3_zadani
```