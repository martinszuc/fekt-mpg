# 08 — Textury a Nanášení textur

tags: #téma8 #textury #MIPmapping #interpolace #zkouška

---

## Rozdělení textur

| Typ | Popis | Příklad |
|---|---|---|
| **Bitmapová (rastrová)** | 2D obrázek (pole pixelů) | fotografie dřeva |
| **Procedurální** | generovaná algoritmem | Perlinův šum, mramor |
| **Prostorová** | definovaná ve 3D prostoru | voxelová, procedurální 3D |

**Rozdíl bitmapová vs procedurální:**
- Bitmapová: fixní rozlišení, paměťově náročná, snadná tvorba
- Procedurální: nekonečné rozlišení (škáluje bez artefaktů), menší paměť, komplexnější na tvorbu

---

## Mapování 2D textury na 3D objekt

### Inverzní mapování (inverse mapping)
- Z pozice fragmentu na obrazovce → zpět na povrch 3D objektu → texturové souřadnice (u, v)
- "Inverzní" proto, že jdeme od pixelu zpět, ne od textury dopředu

**Pro válec:** (x,y,z) → θ = atan2(x,z) → u = θ/2π, v = y/výška

**Pro kouli:** (x,y,z) → (φ, θ) → u = φ/2π, v = θ/π

### UV mapování
- Každý vertex má přiřazené UV souřadnice (u,v) ∈ [0,1]²
- Interpolace UV přes trojúhelník → texturové souřadnice pro každý pixel

---

## MIP-mapping

> ⚠️ **Velmi časté na písemce!**

**Kde se používá:**
Texturování 3D objektů, zejména vzdálených nebo pod šikmým pohledem.

**Proč:**
Bez MIP-mappingu vzdálený objekt zabírá méně pixelů než textura má texelů → aliasing (scintilace, moiré).

**Jak funguje:**
1. Předpočítá se **pyramida** textur v různých rozlišeních: 256×256, 128×128, 64×64, ...
2. Při renderování se vybere MIP úroveň odpovídající velikosti objektu na obrazovce
3. Přechod mezi úrovněmi → trilineární interpolace (viz níže)

**Paměťová cena:** ~33% navíc oproti původní textuře (geometrická řada: 1/4 + 1/16 + ... = 1/3)

---

## Mapování prostředí (Environment mapping)
- Textura simuluje odrazy okolí na lesklém povrchu
- Cube mapping: 6 stran krychle jako textura okolí

## Pohledově závislé mapování
- Texturové souřadnice závisí na směru pohledu (view vector)
- Použití: lesklé odrazy, efekty jako sheen

## Zvrásnění povrchu (Bump / Normal mapping)
- Simulace detailů povrchu bez přidání polygonů
- **Bump mapping:** perturbace normál ze šedotónové mapy výšek
- **Normal mapping:** normály přímo uloženy v RGB textuře

---

## Interpolace textur

### Lineární interpolace
```
f(t) = (1-t)·f₀ + t·f₁,    t ∈ [0,1]
```
1D interpolace mezi dvěma hodnotami.

### Bilineární interpolace
- 2D interpolace v textuře
- Nejprve lineárně ve směru u, pak v v
```
f(u,v) = (1-u)(1-v)·f₀₀ + u(1-v)·f₁₀ + (1-u)v·f₀₁ + uv·f₁₁
```
- Výsledek: hladký přechod, bez pixelování pro blízké objekty

### Trilineární interpolace
- Bilineární interpolace mezi **dvěma MIP úrovněmi** + lineární interpolace mezi nimi
- Eliminuje "skokové" přechody mezi MIP úrovněmi
- Používá se v kombinaci s MIP-mappingem

---

## Komprese textur
- Standardní formáty: S3TC (DXT1/3/5), ASTC, ETC
- Účel: snížit paměťové nároky a šířku pásma GPU
- Pevný poměr komprese (DXT1: 8:1), rychlá hardwarová dekomprese

---

## Aliasing v texturách
- Při dalekém pohledu: jeden pixel pokrývá mnoho texelů → aliasing
- Řešení: **MIP-mapping** + **anisotropic filtering** (pro šikmý pohled)

---

## Kontrolní otázky

**Q: Jaký je rozdíl procedurální a bitmapové textury?**
Bitmapová = uložený obrázek, fixní rozlišení. Procedurální = generovaná algoritmem, nekonečné rozlišení, méně paměti.

**Q: Kde se používá MIP-mapping a proč?**
Při renderování vzdálených/šikmých texturovaných povrchů. Předchází aliasingu tím, že používá předpřipravené nižší rozlišení textury odpovídající velikosti objektu na obrazovce.

**Q: Jaký je princip lineární, bilineární a trilineární interpolace?**
Lineární: 1D mezi 2 hodnotami. Bilineární: 2D ve čtyřech bodech textury (interpolace u pak v). Trilineární: bilineární ve dvou MIP úrovních + lineární interpolace mezi nimi.

**Q: Co vyjadřuje normálový vektor plochy?**
Viz téma 6 — kolmý na tečnou rovinu, vyjadřuje orientaci povrchu.

---

[[HOME|← Zpět domů]]
