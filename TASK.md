# CV6 — Task Breakdown

## Task 1 — 3D Scene: Projection & Camera
**Functions:** `onReshape()`, `onDisplay3D()`

### 1a — Orthographic projection
In `onReshape()`, inside the `window == window1` branch:
```cpp
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(-20, 20, -20, 20, -30, 30);
```

### 1b — Camera placement
In `onDisplay3D()`, set up modelview before drawing:
```cpp
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(4, 4, 16,   // eye position
          0, 0, 0,    // looking at origin
          0, 1, 0);   // up is +Y

vykresliObjekt();
```

### 1c — Switch to perspective and compare
Replace `glOrtho(...)` with:
```cpp
gluPerspective(fov, (double)w / (double)h, nearPlane, farPlane);
// fov=60, nearPlane=0.01, farPlane=90 (already defined as globals)
```
Observe how parallel lines converge and depth becomes visible.

---

## Task 2 — 2D Scene: House with Transforms
**Functions:** `onReshape()`, `onDisplay2D()`

### 2a — Projection in onReshape()
Inside the `window == window2` branch:
```cpp
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(-double(w)/2, double(w)/2, -double(h)/2, double(h)/2, -1, 1);
```

### 2b — Draw house with transforms in onDisplay2D()
```cpp
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

// transforms applied bottom-to-top in OpenGL
glTranslatef(50, 50, 0);
glScalef(0.5, 0.5, 1.0);
glRotatef(-15, 0, 0, 1);   // clockwise = negative angle

vykresliDomecek();
```

> **Important:** OpenGL multiplies transforms right-to-left. The last call in code
> is the first applied to vertices. So the order here is: rotate → scale → translate.

**Expected result:** House is rotated 15° CW, shrunk to half size, then moved to +50x, +50y.

> ⚠️ The house verts span [50,200] range, so with the ortho centered at 0 they'll
> appear in the positive quadrant. If origin is at window center, the house sits
> upper-right. Adjust translate if needed to center visually.

---

## Task 3 — Animation
**Functions:** `onDisplay3D()`, `onDisplay2D()`, `onTimer()`

### 3a — Rotate house by 5° every 15ms
In `onTimer()`:
```cpp
void onTimer(int value) {
    if (timerOn) {
        angle += 5;
        glutSetWindow(window2);
        glutPostRedisplay();
        glutSetWindow(window1);
        glutPostRedisplay();
        glutTimerFunc(15, onTimer, value);  // re-register
    }
}
```

In `onDisplay2D()`, use `angle` in the rotation call:
```cpp
glRotatef(angle, 0, 0, 1);
```

### 3b — Static "Rotace domu" text in window2
The text must not rotate. Use push/pop to draw it outside the transform stack:
```cpp
// after drawing the house...
glPushMatrix();
  glLoadIdentity();
  glColor3f(1, 1, 1);
  bitmapText(-200, 220, GLUT_BITMAP_HELVETICA_12, "Rotace domu");
glPopMatrix();
```
Position coordinates are in the ortho space (`[-w/2, w/2]`).

### 3c — Animate 3D object in window1
In `onDisplay3D()`, add rotation around z-axis before drawing:
```cpp
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(4, 4, 16, 0, 0, 0, 0, 1, 0);
glRotatef(angle, 0, 0, 1);  // same angle variable
vykresliObjekt();
```

---

## Bonus — Reverse rotation without changing `angle`
The `angle` variable increments normally, but negate it in `glRotatef`:
```cpp
glRotatef(-angle, 0, 0, 1);  // flip sign here, not in onTimer
```
This makes the house rotate CW while `angle` still increases.

---

## Reflection Questions (from lecture)

**Q: What happens if you don't call `glLoadIdentity()` on GL_MODELVIEW?**  
Transforms accumulate across frames. Each draw call compounds onto the previous
matrix state, causing the object to spiral/scale unboundedly.

**Q: Does swapping transform order change the result?**  
Yes. Translate → Rotate ≠ Rotate → Translate. The pivot point of rotation changes.
E.g., rotating after translating orbits around the world origin; rotating before
translating spins in-place then moves.

---

## Gotchas

- `glOrtho` / `gluPerspective` go in `GL_PROJECTION` mode, `gluLookAt` / transforms go in `GL_MODELVIEW`
- Always call `glLoadIdentity()` before setting up either matrix
- The timer must re-register itself (`glutTimerFunc` inside `onTimer`) to keep firing
- `glutSetWindow()` before `glutPostRedisplay()` when updating both windows from one timer
- `glPushMatrix` / `glPopMatrix` isolate transform state for the static text overlay
- `zkos[]` (the shear matrix) is defined in the starter but not used by any task — may be a lector demo or future task
