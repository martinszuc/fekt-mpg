# 09 — Paralelní systémy a SIMD

tags: #téma9 #SIMD #cache #pipeline #zkouška

---

## Flynnova taxonomie (třídění paralelních systémů)

| Zkratka | Instrukce | Data | Příklad |
|---|---|---|---|
| **SISD** | Single | Single | klasický CPU |
| **SIMD** | Single | Multiple | vektorové instrukce (SSE, AVX) |
| **MISD** | Multiple | Single | raritní |
| **MIMD** | Multiple | Multiple | vícejádrový CPU, cluster |

### Analogie pro pochopení

- **SISD** = **Jeden kuchař, jedno jídlo.** Kuchař dělá jednu věc v jednu chvíli — klasický sekvenční procesor.
- **SIMD** = **Jeden učitel, 30 studentů řeší stejný pracovní list.** Učitel dá jednu instrukci ("sečtěte řádek 3") a všichni studenti ji provedou současně na svých datech. Každý student má jiná čísla, ale postup je stejný.
- **MISD** = **Více inspektorů kontroluje jeden výrobek.** Každý provádí jiný test na stejném kusu — v praxi se téměř nepoužívá.
- **MIMD** = **Tým kuchařů, každý vaří jiné jídlo.** Každý kuchař má vlastní recept (instrukci) a vlastní ingredience (data). Tak funguje vícejádrový procesor — každé jádro běží nezávisle.

---

## SIMD — Single Instruction, Multiple Data

> ⚠️ **Otázka:** "Popište princip SIMD. Rozdíl skalárního a vektorového zpracování."

**Princip:**
Jedna instrukce se provede **současně** na více datových prvcích (vektoru).

![[09-simd.svg]]

**Skalární zpracování:**
```
a[0] + b[0] = c[0]   (jedna operace)
a[1] + b[1] = c[1]   (druhá operace)
a[2] + b[2] = c[2]   (třetí operace)
a[3] + b[3] = c[3]   (čtvrtá operace)
→ Celkem 4 operace (4 instrukce sčítání)
```

**Vektorové (SIMD) zpracování:**
```
[a[0], a[1], a[2], a[3]] + [b[0], b[1], b[2], b[3]] = [c[0], c[1], c[2], c[3]]
(jedna instrukce, čtyři výsledky najednou)
→ Celkem 1 operace → 4× zrychlení
```

### Konkrétní příklad — sčítání dvou polí

Mějme dvě pole o 4 prvcích:
```
a = [1.0, 2.0, 3.0, 4.0]
b = [5.0, 6.0, 7.0, 8.0]
```

**Skalární (klasický) kód:**
```c
for (int i = 0; i < 4; i++) {
    c[i] = a[i] + b[i];   // 4 iterace, 4 sčítací instrukce
}
// Výsledek: c = [6.0, 8.0, 10.0, 12.0]
```

**SIMD (vektorový) kód (koncepčně):**
```c
// Načteme 4 floaty najednou do 128-bit registru
__m128 va = _mm_load_ps(a);    // va = [1.0, 2.0, 3.0, 4.0]
__m128 vb = _mm_load_ps(b);    // vb = [5.0, 6.0, 7.0, 8.0]
__m128 vc = _mm_add_ps(va, vb); // vc = [6.0, 8.0, 10.0, 12.0]
_mm_store_ps(c, vc);            // 1 sčítací instrukce místo 4
```

Reálné instrukční sady: **SSE** (128-bit, 4 floaty), **AVX** (256-bit, 8 floatů), **AVX-512** (512-bit, 16 floatů).

### Horizontální vs vertikální operace

**Vertikální (cross-lane):**
- Operace mezi **odpovídajícími prvky** dvou vektorů
- Příklad: [a₀,a₁,a₂,a₃] + [b₀,b₁,b₂,b₃] = [a₀+b₀, a₁+b₁, a₂+b₂, a₃+b₃]
- Nejběžnější, efektivní

**Horizontální:**
- Operace **uvnitř** jednoho vektoru
- Příklad: sum([a₀,a₁,a₂,a₃]) = a₀+a₁+a₂+a₃
- Méně efektivní, ale potřebné pro dot product atd.

**Praktický příklad — dot product (skalární součin) dvou vektorů:**
```
u = [1, 2, 3, 4]
v = [5, 6, 7, 8]

Krok 1 — vertikální operace (násobení po prvcích):
  [1×5, 2×6, 3×7, 4×8] = [5, 12, 21, 32]

Krok 2 — horizontální operace (součet uvnitř vektoru):
  5 + 12 + 21 + 32 = 70

→ dot(u, v) = 70
```

Dot product tedy vyžaduje **oba typy** operací: nejdřív vertikální (násobení odpovídajících párů), pak horizontální (sečtení všech mezivýsledků do jednoho čísla).

---

## Proudové zpracování instrukcí (Pipeline)

### Instrukční cyklus
Fáze jedné instrukce:
1. **Fetch** — načtení instrukce z paměti
2. **Decode** — dekódování instrukce
3. **Execute** — provedení
4. **Write-back** — zápis výsledku

### Pipeline (proudové zpracování)
- Různé instrukce jsou v různých fázích **současně**
- Zvýšení throughputu bez zvýšení frekvence
- Problém: závislosti mezi instrukcemi (hazardy) → pipeline stall

![[09-pipeline-instrukci.svg]]

### Analogie — prádelna

Pipeline funguje jako **prádelna s více stroji**:
- Máte 4 dávky prádla a 4 fáze: vkládání, praní, sušení, skládání.
- **Bez pipeline:** dokončíte celou dávku 1, pak začnete dávku 2. Celkem: 4 × 4 = 16 časových jednotek.
- **S pipeline:** zatímco dávka 1 se suší, dávka 2 se pere, dávka 3 se vkládá. Fáze se překrývají.

### Konkrétní příklad — úspora cyklů

Předpokládejme 4 fáze po 1 taktu a 4 instrukce:

| Cyklus | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|--------|---|---|---|---|---|---|---|
| **I1** | F | D | E | W | | | |
| **I2** | | F | D | E | W | | |
| **I3** | | | F | D | E | W | |
| **I4** | | | | F | D | E | W |

- **Bez pipeline:** 4 fáze × 4 instrukce = **16 cyklů**
- **S pipeline:** 4 (první instrukce) + 3 (každá další přidá 1 cyklus) = **7 cyklů**
- Zrychlení: 16/7 = **2,3×** (a čím víc instrukcí, tím blíž ke 4× zrychlení)

Pro N instrukcí s K fázemi: **K + (N-1)** cyklů místo **K × N** cyklů.

---

## Vyrovnávací paměti (Cache)

Hierarchie (od nejrychlejší/nejmenší po nejpomalejší/největší):
```
Registry → L1 Cache → L2 Cache → L3 Cache → RAM → Disk
```

![[09-cache-hierarchie.svg]]

- **L1:** nejrychlejší (~1 ns), ~32 KB, per-core
- **L2:** ~5 ns, ~256 KB, per-core
- **L3:** ~15 ns, ~MB, sdílená mezi jádry
- **RAM:** ~100 ns, ~GB

### Analogie — cache miss penalty

Přístup do L1 cache = **najít knihu na svém stole** (1 sekunda).
Přístup do RAM = **dojít do knihovny přes celý kampus** (100 sekund).

Pokud hledaná data nejsou v cache (= **cache miss**), procesor musí čekat na pomalou RAM. Jeden cache miss může stát **100× víc času** než cache hit. Proto je rozhodující, jak dobře program využívá cache.

### Cache line (řádek cache)

Procesor nenačítá z paměti jednotlivé bajty, ale celé **cache line** (typicky 64 B). Když přistoupíte k jednomu prvku, načte se rovnou celý blok okolních dat.

**Proč jsou pole (arrays) cache-friendly:**
```
Pole: [1, 2, 3, 4, 5, 6, 7, 8]  — prvky leží v paměti za sebou
→ Přístup k a[0] načte celý cache line obsahující a[0]..a[7]
→ Přístupy k a[1], a[2], ... jsou cache hits (data už jsou v cache)
```

**Proč spojové seznamy (linked lists) NEJSOU cache-friendly:**
```
Uzel 1 (adresa 0x1000) → Uzel 2 (adresa 0x5F00) → Uzel 3 (adresa 0x2A00)
→ Uzly jsou rozházené po celé paměti
→ Každý přístup k dalšímu uzlu = pravděpodobný cache miss
→ Mnohem pomalejší průchod
```

**Princip lokality:**
- **Časová lokalita:** nedávno použitá data budou brzy znovu použita (= nechej je v cache)
- **Prostorová lokalita:** data blízká právě použitým budou brzy potřeba (= proto se načítá celý cache line)

---

[[HOME|← Zpět domů]]

---

# 10 — GPU Architektura, Vertex Shader, Pixel Shader

tags: #téma10 #GPU #vertexShader #pixelShader #zkouška

---

## GPU jako paralelní systém

- GPU obsahuje tisíce jednoduchých jader (vs CPU s desítkami složitých)
- SIMD architektura: jedna instrukce na mnoho dat (vrcholů/pixelů)
- Navržena pro **throughput**, ne latenci
- GPGPU: použití GPU pro obecné výpočty (CUDA, OpenCL)

### GPU vs CPU — srovnání s konkrétními čísly

![[10-gpu-vs-cpu.svg]]

| | **CPU** | **GPU** |
|---|---|---|
| Počet jader | ~8–24 složitých jader | ~5 000–16 000 jednoduchých jader |
| Frekvence | ~4–5 GHz | ~1,5–2 GHz |
| Optimalizováno pro | **Latenci** (co nejrychleji dokončit jednu úlohu) | **Propustnost** (co nejvíc úloh za sekundu) |
| Řízení | Velký řadič, predikce skoků, out-of-order | Minimální řadič, jednoduché in-order |
| Cache | Velká hierarchická cache | Malá cache, spoléhá na paralelismus |
| Ideální úloha | Komplexní logika, větvení, sekvenční kód | Masivně paralelní výpočty na velkých datech |

**Analogie:** CPU je jako **sporťák** — extrémně rychlý, ale vezme jen 2 lidi. GPU je jako **autobus** — pomalejší, ale vezme 50 lidí najednou. Pokud potřebujete přepravit 10 000 lidí, autobusy vyhrávají.

---

## Vertex Shader

**Co je:**
Programovatelná jednotka provádějící kód **pro každý vrchol** (vertex) vstupní geometrie.

**Vstup:** jeden vrchol (pozice, normála, UV, atd.)
**Výstup:** jeden transformovaný vrchol

**Datový typ:** 128-bit quad-vector [X, Y, Z, W] — 4× float32

**Typická operace:** World × View × Projection matice → pozice na obrazovce (screen space)

**Architektura SIMD:** zpracovává 4 floaty najednou (x, y, z, w)

**Omezení:** nelze přidávat ani odebírat vrcholy (pouze transformovat)

### Konkrétní příklad — transformace vrcholu

Mějme vrchol krychle v lokálním souřadném systému modelu:
```
Vstupní vrchol: pozice = (1, 2, 3), w = 1

1. Model matice (např. zvětšení 2× a posun):
   (1, 2, 3) → (2, 4, 6)   [world space]

2. View matice (kamera na pozici (0, 0, 10), dívá se na počátek):
   (2, 4, 6) → (2, 4, -4)   [view/camera space]

3. Projection matice (perspektivní projekce):
   (2, 4, -4) → (0.35, 0.7, 0.6, 1.0)   [clip space]

4. Perspektivní dělení (HW automaticky):
   → (0.35, 0.7, 0.6)   [NDC: normalized device coordinates, -1 až 1]

5. Viewport transform (rozlišení 800×600):
   → pixel (540, 90)   [screen space]
```

Vertex shader provádí kroky 1–3 (násobení matic). Kroky 4–5 dělá hardware automaticky.

### Architektura VS 3.0
- 16 vstupních registrů (v0–v15): 16×128 bitů
- 12 pracovních registrů (r0–r11)
- Paměť konstant (c0–c255)
- Výstup: oD0, oD1, oFog, Pos, oPts, oT0–oT7

---

## Pixel Shader (Fragment Shader)

**Co je:**
Programovatelná jednotka provádějící kód **pro každý pixel (fragment)** na obrazovce.

**Vstup:** fragment (interpolované hodnoty z rasterizace — barva, UV, pozice)
**Výstup:** barva pixelu (RGBA)

**Datový typ:** stejný jako VS — 128-bit, 4× float32 [X, Y, Z, W]

**Typická operace:** výpočet barvy pixelu (texturování, osvětlení per-pixel, efekty)

**Bývá jednodušší** než VS (víc fragmentů než vrcholů → optimalizace nutná)

### Konkrétní příklad — výpočet barvy pixelu

```
Fragment na pozici pixelu (400, 300), hloubka z = 0.65

1. Interpolované UV souřadnice z vrcholů: UV = (0.5, 0.5)
   → střed textury

2. Vzorkování textury (texture sampling):
   texture2D(myTexture, UV) → baseColor = (0.8, 0.2, 0.1, 1.0)
   → červenohnědý povrch

3. Phongovo osvětlení:
   - Ambient:  0.1 × baseColor = (0.08, 0.02, 0.01)
   - Diffuse:  max(dot(N, L), 0) × baseColor = 0.7 × (0.8, 0.2, 0.1)
              = (0.56, 0.14, 0.07)
   - Specular: pow(max(dot(R, V), 0), 32) × white = (0.3, 0.3, 0.3)

4. Finální barva:
   ambient + diffuse + specular = (0.94, 0.46, 0.38, 1.0)
   → výstup do framebufferu
```

---

## Unified Shader Architecture (jednotná architektura shaderů)

**Starší GPU (před ~2006):** Měly **oddělené** hardwarové jednotky pro vertex a pixel shadery. Problém: pokud scéna měla málo vrcholů ale hodně pixelů, vertex jednotky zahálely a pixel jednotky nestíhaly (nebo naopak).

**Moderní GPU (od ~2006, např. GeForce 8800):** Používají **unified shadery** — všechna jádra jsou univerzální a mohou spouštět vertex shader, pixel shader, geometry shader i compute shader. Hardware dynamicky přiděluje jádra podle aktuální potřeby.

| | Oddělené shadery | Unified shadery |
|---|---|---|
| Flexibilita | Fixní poměr VS/PS jednotek | Dynamické přidělování |
| Využití HW | Některé jednotky mohou zahálat | Lepší využití všech jader |
| Programování | Různé instrukční sady | Jedna instrukční sada |

---

## Vertex vs Pixel Shader — Srovnání

| | Vertex Shader | Pixel Shader |
|---|---|---|
| Vstup | vrchol (vertex) | fragment (pixel) |
| Výstup | transformovaný vrchol | barva pixelu |
| Místo v pipeline | vektorová část | rastrová část |
| Datový typ | 128-bit quad-vector | 128-bit quad-vector |
| Typická operace | MVP transformace | texturování, osvětlení |
| Počet paralelních | jeden per vertex | jeden per pixel |

---

## CUDA (Compute Unified Device Architecture)

- NVIDIA platforma pro GPGPU
- Programovací model: kernel = funkce spouštěná N vlákny paralelně
- Paměťové typy: global, shared, local, constant, texture
- `cudaMemcpy()` — kopírování dat CPU↔GPU

### Koncepční příklad — sčítání dvou polí

**CPU (sekvenční):**
```c
// Sčítání 1 000 000 prvků — jedno po druhém
for (int i = 0; i < 1000000; i++) {
    c[i] = a[i] + b[i];
}
// → 1 000 000 sekvenčních operací
```

**GPU (CUDA — paralelní):**
```c
// Kernel: každé vlákno sečte JEDEN pár
__global__ void add(float *a, float *b, float *c) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    c[i] = a[i] + b[i];   // každé vlákno dělá jen toto
}

// Spuštění: 1 000 000 vláken najednou
add<<<ceil(1000000/256), 256>>>(d_a, d_b, d_c);
// → Všech 1 000 000 sčítání proběhne (téměř) současně
```

### Hierarchie vláken v CUDA

CUDA organizuje vlákna do tří úrovní:

```
Grid (mřížka)
├── Block 0
│   ├── Thread 0
│   ├── Thread 1
│   ├── ...
│   └── Thread 255
├── Block 1
│   ├── Thread 0
│   ├── ...
│   └── Thread 255
├── ...
└── Block 3905
    └── ...

Celkem: 3906 bloků × 256 vláken = 1 000 000 vláken
```

- **Thread (vlákno):** nejmenší jednotka, provádí jeden výpočet
- **Block (blok):** skupina vláken (max ~1024), sdílejí **shared memory** (rychlá, on-chip)
- **Grid (mřížka):** všechny bloky jednoho kernelu

**Proč na shared memory záleží:**
- Global memory (VRAM): velká (GB), ale pomalá (~400 cyklů přístupu)
- Shared memory: malá (~48 KB per blok), ale extrémně rychlá (~1 cyklus)
- Pokud vlákna v bloku potřebují sdílet mezivýsledky, shared memory je **100× rychlejší** než global memory

---

## Kontrolní otázky

**Q: Vysvětlete princip SIMD a rozdíl skalárního a vektorového zpracování.**
SIMD: jedna instrukce na více datových prvků (vektor). Skalární: jedna op na jednu hodnotu, vektorové: jedna op na více hodnot najednou. Příklad: sčítání 4 floatů — skalárně 4 instrukce, SIMD 1 instrukce (128-bit registr pojme 4× float32).

**Q: Co jsou horizontální a vertikální operace?**
Vertikální: mezi odpovídajícími prvky dvou vektorů (nejběžnější). Horizontální: uvnitř jednoho vektoru (např. součet všech prvků). Dot product vyžaduje oba typy — vertikální násobení párů, pak horizontální součet.

**Q: Co jsou vertex a pixel shader?**
VS: programovatelné zpracování každého vrcholu ve vektorové části pipeline (transformace pozice pomocí MVP matic). PS: zpracování každého pixelu v rastrové části (vzorkování textur, výpočet osvětlení → finální barva).

**Q: Jaký datový typ používají shadery?**
128-bitový quad-vector: 4× 32-bit float, označen X, Y, Z, W.

**Q: Co je unified shader architektura?**
Moderní GPU mají univerzální jádra, která mohou spouštět libovolný typ shaderu (VS, PS, GS, CS). Hardware dynamicky přiděluje jádra podle potřeby, čímž se eliminuje problém nevyužitých specializovaných jednotek.

**Q: Jak je organizována hierarchie vláken v CUDA?**
Grid → Blocks → Threads. Vlákna v bloku sdílejí rychlou shared memory. Grid obsahuje všechny bloky jednoho kernelu. Typicky max 1024 vláken na blok.

---

[[HOME|← Zpět domů]]
