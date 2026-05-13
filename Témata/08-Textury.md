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

![[08-uv-mapping.svg]]

> **Analogie: Balení dárku.** Představte si, že máte kus balicího papíru (= 2D textura) a chcete jím obalit krabici (= 3D objekt). UV souřadnice jsou jako rozložený střih papíru — říkají, který bod na papíru odpovídá kterému bodu na povrchu krabice. Souřadnice (u, v) jsou v rozsahu [0, 1], kde (0, 0) je levý dolní roh papíru a (1, 1) je pravý horní roh.

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

![[08-mip-pyramida.svg]]

> ⚠️ **Velmi časté na písemce!**

**Kde se používá:**
Texturování 3D objektů, zejména vzdálených nebo pod šikmým pohledem.

**Proč:**
Bez MIP-mappingu vzdálený objekt zabírá méně pixelů než textura má texelů → aliasing (scintilace, moiré).

> **Intuitivní příklad:** Představte si šachovnicovou podlahu, která se táhne do dálky (jako v závodní hře). Blízko u kamery je podlaha velká a detailní — to je v pořádku. Ale daleko na horizontu se celá šachovnice zmenší do pár pixelů. Bez MIP-mappingu se GPU snaží číst z plné textury (třeba 1024x1024) a nacpat ji do 5 pixelů — výsledkem je **blikající, třepotavý vzor** (moiré), který se mění s každým snímkem. S MIP-mappingem GPU automaticky použije zmenšenou verzi textury (třeba 8x8) pro vzdálenou část, takže výsledek je hladký a stabilní.

**Jak funguje:**
1. Předpočítá se **pyramida** textur v různých rozlišeních: 256×256, 128×128, 64×64, ...
2. Při renderování se vybere MIP úroveň odpovídající velikosti objektu na obrazovce
3. Přechod mezi úrovněmi → trilineární interpolace (viz níže)

**Paměťová cena:** ~33% navíc oproti původní textuře (geometrická řada: 1/4 + 1/16 + ... = 1/3)

#### Příklad výpočtu paměti

Mějme texturu **1024 x 1024 pixelů** ve formátu **RGBA** (4 bajty na pixel):

| MIP úroveň | Rozlišení | Velikost |
|-------------|-----------|----------|
| 0 | 1024 x 1024 | 4 194 304 B = **4.00 MB** |
| 1 | 512 x 512 | 1 048 576 B = 1.00 MB |
| 2 | 256 x 256 | 262 144 B = 0.25 MB |
| 3 | 128 x 128 | 65 536 B |
| 4 | 64 x 64 | 16 384 B |
| ... | ... | ... |
| 10 | 1 x 1 | 4 B |
| **Celkem** | | **≈ 5.33 MB** |

Celkem: 4 MB × 4/3 ≈ **5.33 MB**. Za pouhých 33 % paměti navíc výrazně zlepšíme kvalitu renderování.

---

## Mapování prostředí (Environment mapping)
- Textura simuluje odrazy okolí na lesklém povrchu
- Cube mapping: 6 stran krychle jako textura okolí

> **Intuice cube mappingu:** Představte si, že kolem celé scény postavíte obrovskou krychli a na každou z jejích **6 stěn** nakreslíte (vyfotíte), co by bylo vidět z centra scény daným směrem — nebe nahoře, zem dole, okolí na čtyřech stranách. Když pak chcete vykreslit zrcadlový objekt (třeba chromovou kouli), pro každý pixel spočítáte směr odrazu a podíváte se, jaká barva je na příslušné stěně krychle. Výsledkem jsou realisticky vypadající odrazy okolí bez nutnosti skutečného ray tracingu.

## Pohledově závislé mapování
- Texturové souřadnice závisí na směru pohledu (view vector)
- Použití: lesklé odrazy, efekty jako sheen

## Zvrásnění povrchu (Bump / Normal mapping)
- Simulace detailů povrchu bez přidání polygonů
- **Bump mapping:** perturbace normál ze šedotónové mapy výšek
- **Normal mapping:** normály přímo uloženy v RGB textuře

> **Intuice:** Místo toho, abychom modelovali každý výstupek a drážku cihlové zdi miliony polygonů, použijeme **rovný povrch** (třeba jen 2 trojúhelníky) a "podvedeme" osvětlení tím, že pro každý pixel uložíme **falešnou normálu** směřující jinam, než by odpovídalo plochému povrchu. Osvětlovací výpočet pak vytvoří iluzi hrbolatosti, i když geometrie je ve skutečnosti rovná.

#### Bump mapping vs Normal mapping — srovnání

| Vlastnost | Bump mapping | Normal mapping |
|-----------|-------------|----------------|
| **Uložení dat** | Šedotónová textura (heightmapa) — každý pixel = výška (0-255) | RGB textura — každý pixel = normálový vektor (R=x, G=y, B=z) |
| **Výpočet normály** | Normála se **vypočítá** z rozdílů sousedních výšek (parciální derivace) | Normála se **přímo přečte** z textury (rychlejší) |
| **Flexibilita** | Jednodušší na tvorbu (stačí namalovat výšky ve Photoshopu) | Přesnější výsledky, lze "zapéct" z high-poly modelu |
| **Typická barva textury** | Černobílá (výšková mapa) | Modrofialová (protože normály mířící "vzhůru" mají z≈1 → B≈255, x≈0 → R≈128, y≈0 → G≈128) |

---

## Interpolace textur

### Lineární interpolace
```
f(t) = (1-t)·f₀ + t·f₁,    t ∈ [0,1]
```
1D interpolace mezi dvěma hodnotami.

### Bilineární interpolace

![[08-bilinearni.svg]]

- 2D interpolace v textuře
- Nejprve lineárně ve směru u, pak v v
```
f(u,v) = (1-u)(1-v)·f₀₀ + u(1-v)·f₁₀ + (1-u)v·f₀₁ + uv·f₁₁
```
- Výsledek: hladký přechod, bez pixelování pro blízké objekty

#### Kompletní numerický příklad

Mějme 4 sousední texely s hodnotami:
```
f(0,0) = 100    f(1,0) = 200
f(0,1) = 150    f(1,1) = 250
```

Chceme zjistit barvu v bodě **(u, v) = (0.3, 0.7)**.

**Krok 1 — Lineární interpolace ve směru u (dva řádky zvlášť):**
```
Dolní řádek (v=0):  f_dolní = (1-0.3)·100 + 0.3·200 = 0.7·100 + 0.3·200 = 70 + 60 = 130
Horní řádek (v=1):  f_horní = (1-0.3)·150 + 0.3·250 = 0.7·150 + 0.3·250 = 105 + 75 = 180
```

**Krok 2 — Lineární interpolace ve směru v (mezi výsledky):**
```
f(0.3, 0.7) = (1-0.7)·130 + 0.7·180 = 0.3·130 + 0.7·180 = 39 + 126 = 165
```

**Kontrola jedním vzorcem:**
```
f = (1-0.3)(1-0.7)·100 + 0.3·(1-0.7)·200 + (1-0.3)·0.7·150 + 0.3·0.7·250
  = 0.21·100 + 0.09·200 + 0.49·150 + 0.21·250
  = 21 + 18 + 73.5 + 52.5
  = 165 ✓
```

### Trilineární interpolace
- Bilineární interpolace mezi **dvěma MIP úrovněmi** + lineární interpolace mezi nimi
- Eliminuje "skokové" přechody mezi MIP úrovněmi
- Používá se v kombinaci s MIP-mappingem

> **Proč je trilineární nutná?** Bez ní, když se objekt vzdaluje od kamery, v určitém okamžiku GPU přepne z MIP úrovně 3 (64x64) na úroveň 4 (32x32). Na hranici tohoto přepnutí by byla **viditelná ostrá čára** — náhlá změna ostrosti/detailu. Trilineární interpolace provede bilineární vzorkování v **obou** MIP úrovních a výsledky lineárně smíchá, takže přechod je plynulý a neviditelný.

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
