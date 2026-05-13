# 03 — Obraz jako signál

tags: #téma3 #aliasing #vzorkování #číslaPočítač #zkouška

---

## Matematický model obrazu

- Šedotónový obraz = funkce **dvou** proměnných: `f(x, y)` → jas
- Barevný obraz = tři funkce: `R(x,y), G(x,y), B(x,y)`
- **Časový (video)** obraz = funkce **tří** proměnných: `f(x, y, t)`

> ⚠️ **MCQ otázka:** "Šedotónový analogový obraz lze chápat jako funkci tří proměnných."
> **→ Nepravda!** Je to funkce dvou proměnných (x, y). Třetí proměnná by byla čas (video).

---

## Digitalizace obrazu

### 1. Vzorkování (sampling)
- Spojitý obraz → diskrétní mřížka pixelů
- Pixely = vzorky na pravidelné mřížce

![[03-vzorkovani.svg]]

#### Analogie: fotografování točícího se kola
Představte si, že fotíte kolo vozu, které se otáčí. Pokud fotoaparát pořizuje snímky dostatečně rychle (vysoká vzorkovací frekvence), vidíte kolo točit se správným směrem. Pokud ale snímáte příliš pomalu, kolo se zdá stát na místě, nebo se dokonce otáčí **pozpátku** — to je tzv. **wagon wheel effect** (efekt kočárového kola). Přesně stejný problém nastává u obrazu: pokud nemáme dostatečně jemnou mřížku pixelů, jemné detaily se zkreslí nebo úplně zmizí.

#### Nyquistův teorém
**Vzorkovací frekvence musí být alespoň 2x vyšší než nejvyšší frekvence v signálu.** Jinak dojde k aliasingu.

**Konkrétní příklad s čísly:**
Představte si, že nejjemnější detail na obrázku (např. tenké proužky na tkanině) se opakuje každé **2 mm**. To znamená, že prostorová frekvence tohoto detailu je **1 / 2 mm = 0,5 cyklů/mm**. Podle Nyquista musíme vzorkovat s frekvencí alespoň **2 × 0,5 = 1 vzorek/mm**, tedy minimálně **1 pixel na každý milimetr**. V praxi se používá ještě vyšší hustota (např. 1,5–2× Nyquistova frekvence), aby se předešlo artefaktům na hranici.

### 2. Aliasing
- Vzniká při **podvzorkování** (příliš málo pixelů pro rychle se měnící obraz)
- Projevy: zubaté hrany, moiré vzory, falešné obrazce
- **Řešení: antialiasing** — rozmazání před vzorkováním (low-pass filtr), nebo supersampling

![[03-aliasing.svg]]

#### Moiré vzory — konkrétní příklad
Zkuste vyfotit mobilem **obrazovku monitoru** nebo LCD televize. Na fotografii uvidíte podivné barevné proužky a vlny, které na obrazovce ve skutečnosti nejsou — to je **moiré**. Vzniká interferencí pixelové mřížky monitoru s pixelovou mřížkou fotoaparátu. Stejný efekt nastane, když televizní moderátor má na sobě **košili s jemnými proužky** — proužky na tkanině interferují s rastrem snímání a vytvářejí vlnící se barevné obrazce.

#### Supersampling
- Renderuj ve vyšším rozlišení → průměruj → výsledný pixel
- Odstraňuje aliasing, ale drahé na výpočet

**Konkrétní příklad (4× supersampling):**
Chceme určit barvu jednoho pixelu. Místo jednoho vzorku spočítáme **4 vzorky** v podmřížce 2×2 uvnitř pixelu:

```
Subpixel (0.25, 0.25) → jas = 200
Subpixel (0.75, 0.25) → jas = 180
Subpixel (0.25, 0.75) → jas = 220
Subpixel (0.75, 0.75) → jas =  80
```
Výsledný pixel = průměr = **(200 + 180 + 220 + 80) / 4 = 170**.

Kdybychom vzali jen jeden vzorek uprostřed pixelu, mohli bychom dostat třeba 80 (právě jsme trefili tmavou hranu). Supersampling dává hodnotu, která lépe odpovídá tomu, co pixel skutečně pokrývá.

### 3. Kvantování (quantization)
- Spojité hodnoty jasu → diskrétní čísla
- 8 bitů = 256 úrovní šedi
- Méně bitů → viditelné přechody (banding)

#### Co se stane při různém počtu bitů?

| Bitová hloubka | Počet úrovní | Vizuální efekt |
|---|---|---|
| **1 bit** | 2 úrovně (černá / bílá) | Žádné odstíny šedi, obraz vypadá jako černobílý tisk — hrubý, ztráta všech jemných detailů. Vhodné maximálně pro text. |
| **2 bity** | 4 úrovně | Černá, tmavě šedá, světle šedá, bílá. Obraz je extrémně „posterizovaný" — viditelné ostré skoky mezi barvami, žádné plynulé přechody. |
| **4 bity** | 16 úrovní | Přechody jsou stále viditelné (tzv. banding — pruhy), ale obraz je již rozpoznatelný. Připomíná staré herní konzole. |
| **8 bitů** | 256 úrovní | Lidské oko přechody téměř nerozezná. Standard pro běžné obrázky (JPEG, PNG). |

---

## Čísla v počítači

> ⚠️ **Otázka z písemky:** "Jaký je hlavní rozdíl mezi pevnou a plovoucí řádovou čárkou?"

### Pevná řádová čárka (Fixed point)
- Čárka je na pevné pozici v binárním čísle
- **Výhody:** jednoduchost, rychlost, přesnost pro malý rozsah
- **Nevýhody:** omezený rozsah hodnot, nutno předem znát škálu

**Konkrétní příklad — reprezentace čísla 0,1:**
Ve formátu 8.8 (8 bitů celá část, 8 bitů zlomková) je 0,1 reprezentováno jako:
`00000000.00011001` ≈ 0 + 1/16 + 1/32 + 1/256 = **0,09765625** (chyba ≈ 0,0024).
Přesnost je omezená, ale **vždy stejná** v celém rozsahu — ať reprezentujeme 0,1 nebo 200,1, zlomková část má vždy 8 bitů.

### Plovoucí řádová čárka (Floating point — IEEE 754)
- `(-1)^S × M × 2^E` (znaménko, mantissa, exponent)
- **Výhody:** obrovský rozsah (od velmi malých po velmi velká čísla)
- **Nevýhody:** zaokrouhlovací chyby, složitější HW, různá přesnost v různých rozsazích
- Single precision (float32): 1b znaménko + 8b exponent + 23b mantissa

![[03-ieee754.svg]]

**Konkrétní příklad — reprezentace čísla 0,1 v plovoucí čárce (float32):**
0,1 v binární soustavě = 0,0001100110011… (nekonečný periodický rozvoj!). Proto **0,1 nelze v plovoucí čárce reprezentovat přesně** — uloží se přibližná hodnota 0,100000001490116… Chyba je ~10⁻⁸, ale opakovaným sčítáním se kumuluje. Proto `0.1 + 0.1 + 0.1 ≠ 0.3` v mnoha programovacích jazycích.

**Srovnání fixed vs floating pro číslo 0,1:**

| Vlastnost | Pevná čárka (8.8) | Plovoucí čárka (float32) |
|---|---|---|
| Uložená hodnota | 0,09765625 | 0,10000000149… |
| Absolutní chyba | 0,0024 | 1,5 × 10⁻⁸ |
| Rozsah formátu | 0 až 255,996 | ±3,4 × 10³⁸ |
| Přesnost | Stejná všude | Klesá u velkých čísel |

#### Kódování IEEE 754 — krok za krokem (příklad: 6,75)

1. **Převod na binární:** 6 = 110₂ a 0,75 = 0,11₂ → tedy 6,75 = **110,11₂**
2. **Normalizace:** posuneme čárku tak, aby před ní byla jen jednička: 110,11 = **1,1011 × 2²**
3. **Znaménko (S):** číslo je kladné → **S = 0**
4. **Exponent (E):** skutečný exponent je 2, přičteme bias 127 → **E = 2 + 127 = 129 = 10000001₂**
5. **Mantisa (M):** z 1,1011 vezmeme jen část za "1," → **M = 10110000000000000000000** (doplněno nulami na 23 bitů)

**Výsledek v paměti:**
```
0  10000001  10110000000000000000000
S  Exponent  Mantisa
```
Zpětná kontrola: (-1)⁰ × 1,1011₂ × 2² = 1 × 1,6875 × 4 = **6,75** ✓

### Binární převody
- Dekadické → binární: dělení 2, zbytky odspodu
- Příklad: 13 → 1101₂ (13/2=6zb1, 6/2=3zb0, 3/2=1zb1, 1/2=0zb1 → čteme zdola: 1101)

#### Další příklad: 42 → binární

```
42 / 2 = 21  zbytek 0
21 / 2 = 10  zbytek 1
10 / 2 =  5  zbytek 0
 5 / 2 =  2  zbytek 1
 2 / 2 =  1  zbytek 0
 1 / 2 =  0  zbytek 1
```
Čteme zbytky **odspodu**: 42 = **101010₂**

Kontrola: 32 + 8 + 2 = 42 ✓

#### Převod zlomkového čísla: 0,375 → binární

Násobíme 2 a sledujeme celou část:
```
0,375 × 2 = 0,75  → 0
0,750 × 2 = 1,50  → 1
0,500 × 2 = 1,00  → 1
```
Čteme **shora**: 0,375 = **0,011₂**

Kontrola: 0/2 + 1/4 + 1/8 = 0 + 0,25 + 0,125 = 0,375 ✓

---

## Reprezentace obrazu v počítači

| Režim | Popis |
|---|---|
| 1 bit / pixel | černobílá (pouze dva stavy) |
| 8 bpp paleta | 256 barev z palety |
| 24 bpp (True Color) | 8b R + 8b G + 8b B = 16M barev |
| 32 bpp | 24b barva + 8b alfa (průhlednost) |

---

## Kontrolní otázky

**Q: Šedotónový analogový obraz — funkce kolika proměnných?**
Dvou (x, y). Funkce tří by byl video signál f(x, y, t).

**Q: Co je aliasing a jak mu bráníme?**
Aliasing = artefakty při podvzorkování (zubaté hrany, moiré). Bráníme mu low-pass filtrem před vzorkováním (antialiasingový filtr) nebo supresamplingem.

**Q: Pevná vs plovoucí řádová čárka — hlavní rozdíl?**
Pevná: čárka na fixní pozici → malý rozsah, vysoká přesnost. Plovoucí: čárka se pohybuje → obrovský rozsah, ale zaokrouhlovací chyby.

---

[[HOME|← Zpět domů]]
