# fekt-mpg

## cv2 – OpenGL: Introduction to Rendering

Three windows, three tasks.

### Task 1 — House Outline (`vykresliDomecek`)
Complete a closed house silhouette using `GL_LINE_STRIP`.
**Solution:** Added vertices to trace walls and triangular roof in one continuous line.

### Task 2 — Parametric Curve (`vykresliKrivku`)
Draw a parabola using parametric equations: x = 150 + 150t, y = 300t², t ∈ [-1, 1].
**Solution:** Implemented loop with 100 steps rendering parabola with `GL_LINE_STRIP` and dotted line pattern using `GL_LINE_STIPPLE`.

### Task 3 — Transparency Blending (`vykresliPruhlednost`)
Create overlapping transparent shapes: red circle (r=100, α=80%), white circle (r=50, α=100%), blue triangle (α=60%).
**Solution:** Used `glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)` to render shapes with specified transparency values. 