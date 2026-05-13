# 01 — Vykreslovací řetězec

tags: #téma1 #pipeline #zkouška

---

## Hlavní kroky řetězce

```
Geometrie (3D) → Vektorová část → Rasterizace → Rastrová část → Obraz (2D)
```

### Vektorová část (geometry pipeline)
Pracuje s **vrcholy (vertices)** a **primitivy** (trojúhelníky, úsečky, body).

Kroky:
1. **Vstup geometrie** — data vrcholů a primitiv
2. **Tessellation** — dělení primitiv
3. **Vertex shader** — transformace vrcholů (World → View → Projection)
4. **Ořezání (clipping)** — odstranění objektů mimo pohledový objem
5. **Mapování** — převod do screen space

### Rastrová část (raster pipeline)
Pracuje s **fragmenty/pixely**.

Kroky:
1. **Rasterizace trojúhelníků** — určení, které pixely patří trojúhelníku
2. **Pixel shader** (fragment shader) — výpočet barvy každého pixelu
3. **Texture blending / Fog blending**
4. **Průhlednost, maska, dither**
5. **Vykreslení** do frame bufferu

---

## Principiální rozdíl vektorové vs rastrové části

| | Vektorová | Rastrová |
|---|---|---|
| Vstup | vrcholy, primitiva | fragmenty |
| Doména | 3D prostor → 2D | 2D pixelová mřížka |
| Klíčová operace | transformace | interpolace, texturování |
| Shader | vertex shader | pixel/fragment shader |

---

## Pevné vs programovatelné části

- **Pevné (fixed function):** starší GPU, nelze změnit chování
- **Programovatelné:** vertex shader a pixel shader jsou dnes plně uživatelsky programovatelné (od DirectX 8+)

---

## Kontrolní otázky

**Q: Jaké jsou hlavní kroky vykreslovacího řetězce?**
Geometrie → vertex shader → clipping → rasterizace → pixel shader → výstup.

**Q: Jaký je principiální rozdíl mezi vektorovou a rastrovou částí?**
Vektorová pracuje s vrcholy a primitivy ve 3D → 2D transformaci; rastrová pracuje s pixely a určuje jejich barvu.

**Q: Co jsou vertex a fragment shader a čím se liší od pevných částí?**
Jsou to programovatelné procesory na GPU — uživatel sám definuje algoritmus transformace/stínování. Pevné části měly napevno zakódované operace.

---

[[HOME|← Zpět domů]]
