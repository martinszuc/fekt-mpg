# Elektronická část — MCQ (12 bodů)

tags: #MCQ #elektronická #zkouška

> 6 otázek × 2 body. Vychází z minulých termínů — všechny otázky z 24/25L zde.

---

## Úloha 1 (2b)
**Jakých čípků je na lidské sítnici nejméně?**
- a. těch, které jsou citlivé na zelené odstíny
- **b. těch, které jsou citlivé na modré odstíny ✅**
- c. těch, které jsou citlivé na červené odstíny

**Důvod:** S-čípky (short wavelength, modrá) jsou nejméně zastoupeny.

---

## Úloha 2 (2b)
**Jaká barva vznikne, smícháme-li v RGB modelu červenou (R) a zelenou (G)?**
- **a. žlutá (Y) ✅**
- b. bílá (W)
- c. modrá (B)
- d. černá (K)
- e. azurová (C)

**Důvod:** RGB aditivní míchání: R+G = Y (Yellow).

---

## Úloha 3 (2b)
**Šedotónový analogový obraz lze chápat jako funkci tří proměnných.**
- Pravda
- **Nepravda ✅**

**Důvod:** Šedotónový statický obraz = f(x,y), tedy funkce dvou prostorových proměnných. Tři proměnné by byl video signál f(x,y,t).

---

## Úloha 4 (2b)
**Kolik řídicích bodů má Bézierův bikubický plát (a kolik je to čísel)?**
- a. 4 (48)
- b. 12 (12)
- c. 16 (32)
- d. 48 (48)
- e. 16 (54)
- **f. 16 (48) ✅**
- g. 32 (64)
- h. 4 (12)

**Důvod:** 4×4 mřížka řídicích bodů = 16 bodů. Každý bod má 3 souřadnice (x,y,z) → 16×3 = 48 čísel.

---

## Úloha 5 (2b)
**Patří Bézierovy křivky mezi aproximační nebo interpolační křivky?**
- **a. aproximační ✅**
- b. interpolační
- c. ani to, ani to

**Důvod:** Bézierovy křivky neprochází vnitřními řídicími body, pouze se k nim přibližují (aproximují).

---

## Úloha 6 (2b)
**Pro jaké topologie sítí je určen Catmull–Clark algoritmus?**
- a. libovolné
- b. trojúhelníky
- **c. čtyřúhelníky ✅**
- d. pro sítě, kde počet hran není vyšší než počet vrcholů

**Důvod:** Catmull-Clark subdivision je navržen pro quad-mesh (čtyřúhelníkové sítě). Po prvním kroku jsou vždy výsledné plochy čtyřúhelníky.

---

## Rychlý přehled správných odpovědí

| # | Otázka | Odpověď |
|---|---|---|
| 1 | Nejméně čípků | **b** (modré) |
| 2 | R + G = ? | **a** (žlutá) |
| 3 | Šedotón = f(3 prom.)? | **Nepravda** |
| 4 | Bikubický plát | **f** — 16 bodů, 48 čísel |
| 5 | Bézier = aproximační? | **a** (aproximační) |
| 6 | Catmull-Clark topologie | **c** (čtyřúhelníky) |

---

[[HOME|← Zpět domů]] | [[Zkouška/Písemná-část|→ Písemná část]]
