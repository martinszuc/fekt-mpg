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
- **Nyquistův teorém:** vzorkovací frekvence musí být alespoň 2× vyšší než nejvyšší frekvence v signálu
- Pixely = vzorky na pravidelné mřížce

### 2. Aliasing
- Vzniká při **podvzorkování** (příliš málo pixelů pro rychle se měnící obraz)
- Projevy: zubaté hrany, moiré vzory, falešné obrazce
- **Řešení: antialiasing** — rozmazání před vzorkováním (low-pass filtr), nebo supersampling

#### Supersampling
- Renderuj ve vyšším rozlišení → průměruj → výsledný pixel
- Odstraňuje aliasing, ale drahé na výpočet

### 3. Kvantování (quantization)
- Spojité hodnoty jasu → diskrétní čísla
- 8 bitů = 256 úrovní šedi
- Méně bitů → viditelné přechody (banding)

---

## Čísla v počítači

> ⚠️ **Otázka z písemky:** "Jaký je hlavní rozdíl mezi pevnou a plovoucí řádovou čárkou?"

### Pevná řádová čárka (Fixed point)
- Čárka je na pevné pozici v binárním čísle
- **Výhody:** jednoduchost, rychlost, přesnost pro malý rozsah
- **Nevýhody:** omezený rozsah hodnot, nutno předem znát škálu

### Plovoucí řádová čárka (Floating point — IEEE 754)
- `(-1)^S × M × 2^E` (znaménko, mantissa, exponent)
- **Výhody:** obrovský rozsah (od velmi malých po velmi velká čísla)
- **Nevýhody:** zaokrouhlovací chyby, složitější HW, různá přesnost v různých rozsazích
- Single precision (float32): 1b znaménko + 8b exponent + 23b mantissa

### Binární převody
- Dekadické → binární: dělení 2, zbytky odspodu
- Příklad: 13 → 1101₂ (13/2=6zb1, 6/2=3zb0, 3/2=1zb1, 1/2=0zb1 → čteme zdola: 1101)

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
