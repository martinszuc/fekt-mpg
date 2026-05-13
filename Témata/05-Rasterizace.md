# 05 — Rasterizace

tags: #téma5 #rasterizace #bresenham #DDA #zkouška

---

## Co je rasterizace?
Převod vektorových (spojitých) geometrických objektů na **diskrétní pixely** rastrového displeje.

---

## Rasterizace úsečky

Řídicí osa: pro |m| ≤ 1 → osa x; pro |m| > 1 → osa y.

### 1. DDA (Digital Differential Analyzer)
- Přímé numerické řešení z y = mx + b
- V každém kroku: x += 1, y += m, zaokrouhlit y
- **Nevýhoda:** používá reálnou aritmetiku (pomalé)
- **Výhoda:** jednoduchý

### 2. Bresenhamův algoritmus
- **Celočíselná** aritmetika → rychlejší
- Rozhodovací člen `p` určuje, zda y naroste

**Postup (pro 0 < m < 1, x₁ < x₂):**
```
k₁ = 2Δy
k₂ = 2(Δy − Δx)
p  = 2Δy − Δx   (inicializace)

Pro každý krok:
  x = x + 1
  if p > 0: y = y+1, p = p + k₂
  else:              p = p + k₁
  vykresli [x, y]
```

**Výsledek shodný s DDA, ale rychlejší.**

### 3. Mid-point algoritmus
- Pracuje s **implicitním** tvarem přímky: f(x,y) = y − mx − b = 0
- Výsledek shodný s Bresenhamem

### Přerušovaná čára
Stejný algoritmus, ale pixel se vykreslí jen pokud na příslušné pozici vzoru (pattern) je 1.

---

## Rasterizace kružnice

Využívá **8-násobnou symetrii** — stačí spočítat 1/8 a zrcadlit.
Podmínka: počítáme v oktantu kde x ≤ y.

Implicitní rovnice: `x² + y² − r² = 0`

### Midpoint algoritmus pro kružnici
```
Inicializace: (x, y) = (0, r), p = 1 − r

if p < 0:  nový bod (x+1, y),   p_new = p + 2x + 3
if p >= 0: nový bod (x+1, y-1), p_new = p + 2(x−y) + 5
```

---

## Rasterizace elipsy

- Symetrie pouze 4× (4 kvadranty)
- Nutno **přepnout řídicí osu** v bodě zlomu (kde tečna = −45°)
- Implicitní rovnice: `b²x² + a²y² = a²b²`

### Midpoint pro elipsu — dvě fáze:
- Fáze 1 (řídicí osa x): `pi+1 = pi + b²(2xi + 1)` (p ≤ 0) nebo `pi + b²(2xi+1) − 2a²yi` (p > 0)
- Fáze 2: přepínáme osu, symetrický postup

---

## Rasterizace trojúhelníku

### Barycentrické souřadnice
Bod P uvnitř trojúhelníku ABC vyjádříme jako:
```
P = α·A + β·B + γ·C
kde α + β + γ = 1, α,β,γ ≥ 0
```

- **Těžiště trojúhelníka:** α = β = γ = 1/3
- Využití: interpolace barvy, normál, UV souřadnic přes trojúhelník

### Postup rasterizace trojúhelníku
1. Najdi bounding box trojúhelníku
2. Pro každý pixel v bbox: spočítej barycentrické souřadnice
3. Pokud α,β,γ ≥ 0 → pixel je uvnitř → vykresli (s interpolovanou barvou)

---

## Rasterizace Bézierovy křivky

### Neadaptivní
- Rozděl interval [0,1] na N stejných dílků
- Spočítej body q(t) pro t = 0, 1/N, 2/N, ...
- Spoj úsečkami
- Nevýhoda: v rovných místech zbytečně mnoho bodů, v křivolakých málo

### Adaptivní
- Rekurzivní subdivision (de Casteljau)
- Dělíme segment, dokud je dostatečně rovný (kontrola flatness)
- Výhoda: efektivnější, rovnoměrnější kvalita

---

## Kontrolní otázky

**Q: Jaké algoritmy pro rasterizaci úsečky znáte?**
DDA (reálná aritmetika, jednoduchý), Bresenham (celočíselný, rychlejší), Mid-point (implicitní tvar přímky).

**Q: Proč Bresenham nepracuje identicky pro všechny směrnice?**
Algoritmus v základní formě platí jen pro 0 < m < 1, x₁ < x₂. Pro jiné směrnice nutno symetrizovat (prohodit osy, znaménka).

**Q: Co jsou barycentrické souřadnice a jaké má těžiště?**
Trojice (α,β,γ), α+β+γ=1, vyjadřuje polohu bodu vůči vrcholům trojúhelníka. Těžiště: α=β=γ=1/3.

**Q: Jaký je rozdíl neadaptivní vs adaptivní rasterizace Bézierovy křivky?**
Neadaptivní: rovnoměrné dělení parametru t. Adaptivní: rekurzivní subdivision, dělí jen tam kde je zakřivení velké.

---

[[HOME|← Zpět domů]]
