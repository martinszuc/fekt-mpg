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

---

## SIMD — Single Instruction, Multiple Data

> ⚠️ **Otázka:** "Popište princip SIMD. Rozdíl skalárního a vektorového zpracování."

**Princip:**
Jedna instrukce se provede **současně** na více datových prvcích (vektoru).

**Skalární zpracování:**
```
a[0] + b[0] = c[0]   (jedna operace)
a[1] + b[1] = c[1]   (druhá operace)
...
```

**Vektorové (SIMD) zpracování:**
```
[a[0], a[1], a[2], a[3]] + [b[0], b[1], b[2], b[3]] = [c[0], c[1], c[2], c[3]]
(jedna instrukce, čtyři výsledky najednou)
```

### Horizontální vs vertikální operace

**Vertikální (cross-lane):**
- Operace mezi **odpovídajícími prvky** dvou vektorů
- Příklad: [a₀,a₁,a₂,a₃] + [b₀,b₁,b₂,b₃] = [a₀+b₀, a₁+b₁, a₂+b₂, a₃+b₃]
- Nejběžnější, efektivní

**Horizontální:**
- Operace **uvnitř** jednoho vektoru
- Příklad: sum([a₀,a₁,a₂,a₃]) = a₀+a₁+a₂+a₃
- Méně efektivní, ale potřebné pro dot product atd.

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

---

## Vyrovnávací paměti (Cache)

Hierarchie (od nejrychlejší/nejmenší po nejpomalejší/největší):
```
Registry → L1 Cache → L2 Cache → L3 Cache → RAM → Disk
```

- **L1:** nejrychlejší (~1 ns), ~32 KB, per-core
- **L2:** ~256 KB, per-core
- **L3:** ~MB, sdílená

**Princip lokality:**
- Časová lokalita: nedávno použitá data budou brzy znovu použita
- Prostorová lokalita: data blízká právě použitým budou brzy potřeba

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

---

## Kontrolní otázky

**Q: Vysvětlete princip SIMD a rozdíl skalárního a vektorového zpracování.**
SIMD: jedna instrukce na více datových prvků (vektor). Skalární: jedna op na jednu hodnotu, vektorové: jedna op na více hodnot najednou.

**Q: Co jsou horizontální a vertikální operace?**
Vertikální: mezi odpovídajícími prvky dvou vektorů (nejběžnější). Horizontální: uvnitř jednoho vektoru (např. součet všech prvků).

**Q: Co jsou vertex a pixel shader?**
VS: programovatelné zpracování každého vrcholu ve vektorové části pipeline (transformace). PS: zpracování každého pixelu v rastrové části (barva, textury).

**Q: Jaký datový typ používají shadery?**
128-bitový quad-vector: 4× 32-bit float, označen X, Y, Z, W.

---

[[HOME|← Zpět domů]]
