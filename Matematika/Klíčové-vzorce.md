# Klíčové vzorce a fakta

tags: #vzorce #matematika #cheatshet

---

## De Casteljau algoritmus

Pro kubiku P₀, P₁, P₂, P₃ v čase **t**:

```
Úroveň 1:
  Q₀ = (1-t)·P₀ + t·P₁
  Q₁ = (1-t)·P₁ + t·P₂
  Q₂ = (1-t)·P₂ + t·P₃

Úroveň 2:
  R₀ = (1-t)·Q₀ + t·Q₁
  R₁ = (1-t)·Q₁ + t·Q₂

Úroveň 3 (bod na křivce):
  q(t) = (1-t)·R₀ + t·R₁
```

### Výpočet pro t = 3/4 (koeficienty)
- (1-t) = 1/4,  t = 3/4
- Úroveň 1: Q₀ = ¼P₀ + ¾P₁,  Q₁ = ¼P₁ + ¾P₂,  Q₂ = ¼P₂ + ¾P₃
- Úroveň 2: R₀ = ¼Q₀ + ¾Q₁,  R₁ = ¼Q₁ + ¾Q₂
- Výsledek: q = ¼R₀ + ¾R₁

---

## Transformační matice (4×4 homogenní)

### Posunutí o (tx, ty, tz)
```
| 1  0  0  tx |
| 0  1  0  ty |
| 0  0  1  tz |
| 0  0  0  1  |
```

### Rotace kolem osy Z o θ
```
| cosθ  -sinθ  0  0 |
| sinθ   cosθ  0  0 |
| 0      0     1  0 |
| 0      0     0  1 |
```

### Rotace kolem osy X o θ
```
| 1   0      0    0 |
| 0   cosθ  -sinθ  0 |
| 0   sinθ   cosθ  0 |
| 0   0      0    1 |
```

### Změna měřítka (sx, sy, sz)
```
| sx  0   0   0 |
| 0   sy  0   0 |
| 0   0   sz  0 |
| 0   0   0   1 |
```

### Zrcadlení přes rovinu XY (z → -z)
```
| 1  0   0   0 |
| 0  1   0   0 |
| 0  0  -1   0 |
| 0  0   0   1 |
```

### Středová souměrnost (bod [0,0,0])
```
| -1  0   0   0 |
| 0  -1   0   0 |
| 0   0  -1   0 |
| 0   0   0   1 |
```

---

## Phongův osvětlovací model

```
L = La + Ld + Ls

La = cd · Ia                                    (ambientní)
Ld = cd · I · max(0, n·l)                       (difúzní)
Ls = cp·cs · I · max(0, n·h)^p                 (spekulární)

n = normála povrchu (normalizovaná)
l = směr ke světlu (normalizovaný)
h = (l + v) / |l + v|  (půlící vektor, Blinnova aproximace)
v = směr k pozorovateli
p = exponent lesku
```

---

## Bresenhamův algoritmus (úsečka, 0 < m < 1)

```
k₁ = 2Δy
k₂ = 2(Δy - Δx)
p  = 2Δy - Δx     ← inicializace

Každý krok:
  x = x + 1
  if p > 0: y = y+1, p = p + k₂
  else:              p = p + k₁
```

---

## Perspektivní projekce

```
x' = x·d/z
y' = y·d/z
```
(d = ohnisková vzdálenost, z = hloubka bodu)

---

## Bézierovy Bernsteinovy polynomy (n=3)

```
B₀,₃(t) = (1-t)³
B₁,₃(t) = 3t(1-t)²
B₂,₃(t) = 3t²(1-t)
B₃,₃(t) = t³

Křivka: q(t) = Σᵢ Pᵢ · Bᵢ,₃(t)
```

---

## Barycentrické souřadnice

```
P = αA + βB + γC,   α+β+γ=1,  α,β,γ ≥ 0 (uvnitř trojúhelníku)

Těžiště: α = β = γ = 1/3
```

---

## Klíčová čísla k zapamatování

| Fakt | Hodnota |
|---|---|
| Bézier kubika — řídicích bodů | 4 |
| Bézier kubika — čísel (2D) | 8 |
| Bézier kubika — čísel (3D) | 12 |
| Bikubický plát — řídicích bodů | 16 |
| Bikubický plát — čísel (3D) | 48 |
| SIMD quad-vector délka | 128 bitů |
| VS 3.0 vstupní registry | 16 (v0–v15) |
| VS 3.0 pracovní registry | 12 (r0–r11) |
| float32 — bity | 32 (1+8+23) |
| MIP-mapping paměťová cena | ~33% navíc |

---

[[HOME|← Zpět domů]]
