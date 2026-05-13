# 04 — 2D Křivky a Bézierovy křivky

tags: #téma4 #bezier #křivky #deCasteljau #zkouška

---

## Aproximační vs Interpolační křivky

| | Aproximační | Interpolační |
|---|---|---|
| Prochází řídicími body? | **NE** (pouze přibližuje) | **ANO** |
| Příklad | **Bézierovy křivky** | Catmull-Rom spline, Lagrange |
| Výhoda | hladší, předvídatelnější | prochází zadanými body |

**→ Bézierovy křivky jsou APROXIMAČNÍ.**

---

## Maticový tvar křivky

```
q(t) = T · C = [t³  t²  t  1] · C
```

kde matice `C` (4×2 pro 2D) obsahuje koeficienty polynomů.

**Posun křivky o 1 ve směru osy y:**
Změní se hodnota `d_y` (poslední řádek, druhý sloupec) — přičteme 1.
Protože `q(t) = ... + [1] · [d_x, d_y]`, tedy `d_y` je absolutní člen pro y-složku.

---

## Bézierova kubika

- **Stupeň:** 3 (kubika)
- **Počet řídicích bodů:** 4 (P₀, P₁, P₂, P₃)
- **Počet čísel (2D):** 4 × 2 = **8 čísel**
- **Počet čísel (3D):** 4 × 3 = **12 čísel**

### Proč kubiky?
- Stupeň 1 = úsečka, stupeň 2 = kvadratika (omezená flexibilita)
- Stupeň 3 = kubika: **nejnižší stupeň umožňující inflexní bod** (změnu zakřivení)
- Vyšší stupně: numericky nestabilní, těžko ovladatelné
- Kubika je ideální kompromis: dostatečná flexibilita + stabilita

### Bernsteinovy polynomy (váhové funkce)
Pro kubiku (n=3):
```
B₀,₃(t) = (1-t)³
B₁,₃(t) = 3t(1-t)²
B₂,₃(t) = 3t²(1-t)
B₃,₃(t) = t³
```
Vlastnosti: vždy ≥ 0, součet = 1 → **convex hull property** (křivka leží v konvexním obalu řídicích bodů).

---

## Algoritmus de Casteljau

Geometrická metoda výpočtu bodu na Bézierově křivce v čase t.

### Postup pro kubiku (4 body: P₀, P₁, P₂, P₃):

**Úroveň 1:** lineární interpolace sousedních bodů
```
P₀¹ = (1-t)·P₀ + t·P₁
P₁¹ = (1-t)·P₁ + t·P₂
P₂¹ = (1-t)·P₂ + t·P₃
```

**Úroveň 2:**
```
P₀² = (1-t)·P₀¹ + t·P₁¹
P₁² = (1-t)·P₁¹ + t·P₂¹
```

**Úroveň 3 (výsledek):**
```
q(t) = P₀³ = (1-t)·P₀² + t·P₁²
```

### Příklad: t = 3/4

Pro t = 3/4, tedy (1-t) = 1/4:
- **Úroveň 1:** každé `Pᵢ¹ = (1/4)·Pᵢ + (3/4)·Pᵢ₊₁`
- **Úroveň 2:** každé `Pᵢ² = (1/4)·Pᵢ¹ + (3/4)·Pᵢ₊₁¹`
- **Úroveň 3:** `q = (1/4)·P₀² + (3/4)·P₁²`

### Využití de Casteljau — Subdivision
Body z mezivýpočtů definují **dvě poloviny** křivky:
- Levá polovina: P₀, P₀¹, P₀², q(t)
- Pravá polovina: q(t), P₁², P₂¹, P₃

---

## Napojení křivek

### Parametrické (Cⁿ) napojení
Shoda derivací v uzlovém bodě:
- **C⁰:** q₁(1) = q₂(0) — křivky se dotýkají
- **C¹:** q₁'(1) = q₂'(0) — shodné tečné vektory (stejný směr i délka)
- **C²:** q₁''(1) = q₂''(0) — shodná zakřivení

### Geometrické (Gⁿ) napojení
Mírnější podmínka — pouze **směr** tečny musí souhlasit, ne délka:
- **G¹:** tečné vektory jsou kolineární (souběžné), ale mohou mít různé délky

**Rozdíl C¹ vs G¹:**
- C¹ → G¹ (silnější podmínka implikuje slabší)
- G¹ ≠ C¹ (G¹ nevyžaduje stejnou délku tečného vektoru)

---

## Racionální Bézierovy křivky

- Každý řídicí bod má váhu wᵢ
- `q(t) = Σ wᵢ·Pᵢ·Bᵢ,ₙ(t) / Σ wᵢ·Bᵢ,ₙ(t)`
- **Výhoda:** lze přesně vyjádřit kuželosečky (kružnice, elipsy, paraboly)
- Pokud wᵢ = 0 → bod má nulový vliv (jako by neexistoval)

## NURBS
- Non-Uniform Rational B-Splines
- **Výhody oproti Bézier:**
  1. Lokální editace (změna jednoho bodu neovlivní celou křivku)
  2. Libovolně dlouhé křivky bez explicitního napojování segmentů

---

## Kontrolní otázky

**Q: Jsou Bézierovy křivky aproximační nebo interpolační?**
**Aproximační** — neprochází řídicími body (jen prvním a posledním).

**Q: Kolik řídicích bodů má 2D Bézierova kubika a kolik čísel?**
4 body, 4 × 2 = **8 čísel**.

**Q: Proč se používají kubiky?**
Nejnižší stupeň umožňující inflexní bod, dobrý kompromis mezi flexibilitou a numerickou stabilitou.

**Q: Který parametr v C změnit, aby se křivka posunula o 1 ve směru y?**
Přičíst 1 k hodnotě `d_y` (absolutní člen y-složky, poslední řádek, druhý sloupec matice C).

---

[[HOME|← Zpět domů]]
