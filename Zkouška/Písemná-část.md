# Písemná část — Minulé otázky s odpověďmi

tags: #písemná #zkouška #modelovéOdpovědi

> Sbírka skutečných otázek z minulých termínů (24/25L a dřívější).

---

## 🗓️ Termín 12. 5. 2025 — Strana 1

### Q1 (2b): Proč v šeru špatně rozeznáváme odstíny barev?

**Odpověď:**
V šeru (skotopickém vidění) jsou aktivní primárně **tyčinky**, nikoli čípky. Tyčinky jsou citlivé pouze na **intenzitu světla**, nikoli na vlnovou délku. Čípky (S, M, L — pro modrou, zelenou, červenou) jsou receptory barev, ale potřebují dostatek světla ke svému fungování. Proto ve tmě vidíme jen odstíny šedi.

---

### Q2 (4b): Matice C křivky — posun o 1 ve směru y

Křivka: `q(t) = TC` kde T = [t³ t² t 1]

**Odpověď:**
Výsledek `q(t)` je součin T a C. Člen `[1]·[d_x, d_y]` tvoří absolutní člen (konstantu). Aby se křivka posunula o 1 ve směru y, je třeba **přičíst 1 k hodnotě `d_y`** — tedy poslednímu prvku druhého sloupce matice C (4. řádek, 2. sloupec).

Žádná jiná hodnota v C není konstantou y-složky — ostatní jsou násobeny t, t², t³.

---

### Q3 (6b): Hlavní myšlenka subdivision + vlastnost posloupnosti bodů

**Odpověď:**

**Hlavní myšlenka:**
Subdivision je metoda, kde **opakovaným půlením (dělením) křivek nebo plátů** vznikají jemnější a jemnější geometrické struktury. Každý krok nahradí každý segment více segmenty. Výsledek konverguje ke hladkému povrchu.

**Vlastnost posloupnosti bodů:**
Posloupnost bodů vzniklá opakovaným dělením tvoří **Cauchyovskou posloupnost** — konverguje k limitní křivce/ploše. Tato limita je hladká (alespoň C¹, pro Bézier dokonce C²). Prakticky: po dostatečném počtu dělení je výsledná polygonální síť nerozeznatelná od hladkého povrchu.

---

### Q4 (4b): Středová souměrnost v rovině xy — transformační matice

**Odpověď:**
Středová souměrnost "v rámci roviny xy" = **zrcadlení přes rovinu xy** = negace z-souřadnice.

```
M = | 1   0   0   0 |
    | 0   1   0   0 |
    | 0   0  -1   0 |
    | 0   0   0   1 |
```

Tedy: x' = x, y' = y, z' = -z. (4×4 homogenní matice)

> Pozor: Středová souměrnost **bodu** [0,0,0] by byla S(-1,-1,-1). Souměrnost v "rámci roviny xy" = symetrie přes rovinu xy = jen z → -z.

---

### Q5 (6b): MIP-mapping — kde se používá a proč, postup

**Odpověď:**

**Kde:** Při texturování 3D objektů — zejména vzdálených nebo zobrazených pod šikmým pohledem.

**Proč:** Bez MIP-mappingu vzdálený objekt zabírá na obrazovce méně pixelů než má textura texelů. Jeden pixel pokrývá velkou oblast textury → aliasing (scintilace, moiré vzory).

**Jak funguje:**
1. Předpočítá se **pyramida** textur (mipmapy): originál + postupně zmenšené verze na ½, ¼, ⅛... rozlišení
2. Při renderování: podle vzdálenosti objektu (footprint pixelu v texturním prostoru) se vybere odpovídající úroveň
3. Trilineární interpolace: bilineární interpolace ve dvou sousedních úrovních + lineární přechod mezi nimi → hladký výsledek bez "skoků"

**Paměťová cena:** ~33% navíc (součet geometrické řady 1/4 + 1/16 + ... = 1/3).

---

### Q6 (6b): Vertex shader a pixel shader — místo v pipeline, princip, datové typy

**Odpověď:**

**Vertex shader:**
- Místo: **vektorová část** pipeline, zpracovává každý vrchol (vertex)
- Účel: transformace vrcholů z 3D prostoru na 2D obrazovku (World → View → Projection matice)
- Vstup: jeden vertex (pozice, normála, UV); Výstup: transformovaný vertex
- **Nelze** přidávat ani odebírat vrcholy
- Datový typ: **128-bitový quad-vector** [X, Y, Z, W], každý prvek = 32-bit float

**Pixel shader (fragment shader):**
- Místo: **rastrová část** pipeline, zpracovává každý pixel (fragment)
- Účel: výpočet výsledné barvy pixelu (texturování, osvětlení per-pixel, post-processing)
- Vstup: interpolované hodnoty z rasterizace; Výstup: barva RGBA
- Datový typ: stejný — 128-bit, 4× float32

---

## 🗓️ Termín 12. 5. 2025 — Strana 2

### Q1 (4b): Parametrické vs geometrické napojení křivek

**Odpověď:**

**Parametrické napojení (Cⁿ):**
Vyžaduje shodu n-tých derivací v uzlovém bodě:
- C⁰: q₁(1) = q₂(0) — křivky se dotýkají
- C¹: q₁'(1) = q₂'(0) — tečné vektory jsou **totožné** (stejný směr i délka)
- Silnější podmínka

**Geometrické napojení (Gⁿ):**
- G¹: tečné vektory jsou **kolineární** (stejný směr), mohou mít různou délku
- Slabší podmínka — C¹ → G¹, ale ne naopak

**Příklady (nakreslit):**
- C⁰/G⁰: dva segmenty se dotýkají, ale mohou mít "lom"
- G¹: plynulý přechod bez lomu, ale rychlost se změní
- C¹: plynulý přechod, stejná rychlost (tečný vektor)

---

### Q2 (4b): Postup rasterizace elipsy

**Odpověď:**
Implicitní rovnice elipsy: b²x² + a²y² = a²b²

Rasterizace je podobná kružnici, ale 4× symetrie (jen kvadranty, ne oktanty).

**Postup (midpoint):**
1. Inicializace: (x,y) = (0, b), p₁ = b² − a²b + a²/4
2. **Fáze 1** (řídicí osa x): dokud není |∂F/∂y| > |∂F/∂x|
   - Pro pᵢ ≤ 0: pohyb (x+1, y), pᵢ₊₁ = pᵢ + b²(2xᵢ+1)
   - Pro pᵢ > 0: pohyb (x+1, y-1), pᵢ₊₁ = pᵢ + b²(2xᵢ+1) − 2a²yᵢ
3. **Přepnutí na řídicí osu y** (bod zlomu kde tečna = −45°)
4. **Fáze 2** (řídicí osa y): symetrický postup
5. Aplikace 4× symetrie (zrcadlení do všech kvadrantů)

---

### Q3 (4b): Lokální vs globální osvětlování — hlavní rozdíl

**Odpověď:**

**Lokální model (Phong):**
- Uvažuje pouze **přímé světlo** od zdrojů na daný bod
- Nezohledňuje odrazy světla od jiných objektů
- Rychlý, použitelný v reálném čase
- Artefakty: nerealistické stíny, chybějící globální odrazy

**Globální metody (ray tracing, radiosity):**
- Simulují **interakci světla mezi všemi objekty** ve scéně
- Odrazy, lomy, kaustics, měkké stíny, průsvitnost
- Výpočetně náročné (offline rendering)
- Fotorealistické výsledky

---

## 🗓️ Otázky z dalších termínů (různé)

**Q: Jaký je rozdíl pevná vs plovoucí řádová čárka?**
Pevná: čárka na fixní pozici, malý rozsah, vysoká přesnost pro daný rozsah. Plovoucí: pohyblivá čárka (mantissa × 2^exponent), obrovský rozsah, zaokrouhlovací chyby.

**Q: Bézierova křivka — bod v t=3/4 algoritmem de Casteljau.**
→ Viz [[Matematika/Klíčové-vzorce#De Casteljau]]

**Q: Projekce kolmá vs perspektivní — geometrie a matematika.**
→ Viz [[Témata/07-Promítání-a-osvětlování#Promítání]]

---

[[HOME|← Zpět domů]] | [[Zkouška/Elektronická-část|← MCQ otázky]]
