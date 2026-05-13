# 07 — Promítání a Osvětlování

tags: #téma7 #projekce #Phong #osvětlování #zbuffer #zkouška

---

## Promítání

![[07-projekce.svg]]

### Kolmá (rovnoběžná) projekce

- Paprsky jsou **rovnoběžné** s sebou a kolmé na průmětnu
- Matematicky: jednoduše zahodíme z-složku
- Zachovává **vzdálenosti a úhly** (objekty nevypadají menší dál od kamery)
- Matice projekce: `z = 0` pro všechny vrcholy

```
Pkolmo = | 1  0  0  0 |
          | 0  1  0  0 |
          | 0  0  0  0 |
          | 0  0  0  1 |
```

> **Intuice:** Představte si, že sledujete předměty přes obrovský dalekohled z nekonečna. Všechny paprsky přicházejí rovnoběžně, takže vzdálené objekty vypadají stejně velké jako blízké. Proto se kolmá projekce používá v technických výkresech a CAD systémech, kde nechceme, aby perspektiva zkreslovala rozměry.

### Perspektivní projekce

- Paprsky vychází z **jednoho bodu** (středu projekce = poloha kamery)
- Vzdálené objekty vypadají menší (jako v realitě)
- Matematicky: vydělení z-souřadnicí → W ≠ 1 v homogenních souřadnicích

```
x' = x·d/z,   y' = y·d/z    (d = vzdálenost průmětny)
```

Matice pro perspektivu (zjednodušená):
```
Ppersp = | 1  0  0  0  |
          | 0  1  0  0  |
          | 0  0  1  0  |
          | 0  0  1/d  0 |
```
Po homogenním dělení (W = z/d) dostaneme perspektivní transformaci.

#### Konkrétní příklad: perspektivní projekce bodu

Mějme bod **P = (3, 2, 5)** a vzdálenost průmětny **d = 1**.

**Krok 1:** Vynásobíme perspektivní maticí (homogenní souřadnice):
```
[3, 2, 5, 1] × Ppersp = [3, 2, 5, 5/1] = [3, 2, 5, 5]
```

**Krok 2:** Provedeme perspektivní dělení (dělíme W = z/d = 5):
```
x' = 3/5 = 0.6
y' = 2/5 = 0.4
```

**Výsledek:** Bod (3, 2, 5) se promítne na pozici **(0.6, 0.4)** na průmětně.

Když posuneme bod dál na (3, 2, 10), dostaneme x' = 3/10 = 0.3, y' = 2/10 = 0.2 — bod je promítnut blíže ke středu, tedy vypadá menší. To je přesně to, co odpovídá lidskému vnímání.

**Klíčový rozdíl:**
- Kolmá: jednoduché lineární operace, W = 1 stále
- Perspektivní: dělení z → nelineární operace, W ≠ 1

---

## Pohledový objem (Viewing Frustum)

- Kolmá projekce: **kvádr** (ortho box)
- Perspektivní projekce: **komolý jehlan** (frustum)
- Definován parametry:
  - `near` — blízká ořezová rovina
  - `far` — vzdálená ořezová rovina
  - `fov` — úhel zorného pole (field of view)
  - `aspect` — poměr stran

### Ořezové roviny (Clipping planes)
- Objekty mimo frustum se ořezávají (clipping)
- Near plane: zabraňuje dělení nulou, omezuje přesnost z-bufferu
- Far plane: výkonnostní limit

> **Proč near plane nemůže být 0?** Při perspektivní projekci dělíme souřadnice hodnotou z (viz `x' = x·d/z`). Pokud by objekt ležel přímo na pozici kamery (z = 0), dělili bychom nulou — výsledek by byl nekonečno. Near plane = 0 by tedy vedl k **dělení nulou** pro objekty na pozici kamery. Navíc platí: čím blíže je near plane nule, tím méně přesnosti z-bufferu zbývá pro vzdálenější objekty (protože rozložení přesnosti je nelineární — většina bitů se "spotřebuje" na oblast blízko kamery).

---

## Řešení viditelnosti

### Z-buffer

![[07-zbuffer.svg]]

- V rastrové části pipeline
- Paměť hloubky: pro každý pixel uchovává nejmenší z-souřadnici dosud vykresleného fragmentu
- Nový fragment překreslí pixel jen pokud má **menší z** (je blíže)
- Uložení: **normalizované** hloubky v rozsahu [0, 1], nelineárně (více přesnosti blízko kamery)

#### Konkrétní příklad: Z-buffer v akci

Mějme **tři trojúhelníky**, které se všechny promítají na stejný pixel. Zpracováváme je v pořadí, v jakém přišly do pipeline:

| Krok | Trojúhelník | Hloubka z | Z-buffer předtím | Podmínka z < z-buffer? | Akce | Z-buffer potom |
|------|------------|-----------|-------------------|------------------------|------|----------------|
| 0 | — | — | ∞ | — | inicializace | ∞ |
| 1 | Modrý | 0.7 | ∞ | 0.7 < ∞ → **ANO** | zapsat barvu + hloubku | 0.7 |
| 2 | Červený | 0.3 | 0.7 | 0.3 < 0.7 → **ANO** | přepsat barvu + hloubku | 0.3 |
| 3 | Zelený | 0.5 | 0.3 | 0.5 < 0.3 → **NE** | zamítnout (zelený je za červeným) | 0.3 |

**Výsledek:** Pixel má barvu **červeného** trojúhelníku (z = 0.3), protože je nejblíže kameře. Zelený trojúhelník (z = 0.5) byl zamítnut, přestože přišel jako poslední — z-buffer zajistil správnou viditelnost bez ohledu na pořadí vykreslování.

> **Klíčová vlastnost:** Z-buffer funguje správně **bez ohledu na pořadí**, v jakém trojúhelníky zpracováváme. Ať přijdou v jakémkoliv pořadí, výsledek bude vždy správný.

### Z-fighting
- **Co je:** Dva povrchy na (skoro) stejné hloubce → z-buffer nedokáže rozlišit, který je před kterým → blikající artefakt
- **Proč:** Omezená přesnost z-bufferu (typicky 24 bitů) + nelineární distribuce přesnosti
- **Jak zabránit:** 
  - Nastavit `near` co nejvyšší a `far` co nejnižší (užší frustum)
  - Polygon offset (GL_POLYGON_OFFSET_FILL)
  - Nevrstvit objekty na stejné z-vzdálenosti

> **Praktický příklad z-fightingu:** Představte si hru, kde máte dvě překrývající se zdi na téměř stejné pozici (např. podlaha a koberec na ní s tloušťkou 0). Hráč uvidí **blikající vzor** — někde prosvítá podlaha, někde koberec, a při pohybu kamery se vzor neustále mění. Vypadá to jako šachovnicový "pruhovaný" artefakt, který se třese. Řešení: posunout koberec alespoň o malou vzdálenost nad podlahu, nebo použít polygon offset.

---

## Osvětlování

### Zdroje světla
| Typ | Homogenní souřadnice | Příklad |
|---|---|---|
| Bodový (positional) | (x, y, z, **1**) | žárovka |
| Směrový (directional) | (x, y, z, **0**) | slunce |
| Spotlight | bodový s kuželem | reflektor |

Intenzita bodového světla klesá s **druhou mocninou** vzdálenosti.

---

## Phongův lokální osvětlovací model

![[07-phong-slozky.svg]]

### Tři složky oddrazu:

**1. Ambientní (ambient):**
```
La = cd · Ia
```
Rovnoměrné pozadí, žádný specifický zdroj.

> **Intuice:** Ambientní složka simuluje **nepřímé světlo** — to, které se v reálném světě mnohokrát odrazilo od stěn, podlahy a stropu. I v místnosti, kde na objekt přímo nesvítí žádná lampa, je stále trochu viditelný. Ambientní složka je ten "základní záblesk", který vidíte i ve stínu. Bez ní by stínované části objektu byly úplně černé.

**2. Difúzní (diffuse) — Lambertův zákon:**
```
Ld = cd · I · max(0, ⟨n, l⟩)
```
- n = normála povrchu, l = směr ke světlu
- Závisí na úhlu dopadu světla, **nezávisí na pozorovateli**

> **Intuice:** Difúzní složka odpovídá **matným povrchům** — jako křída, papír nebo nezpracované dřevo. Světlo dopadá na povrch a rovnoměrně se rozptyluje do všech směrů. Proto vypadá stejně, ať se díváte z jakéhokoliv úhlu. Jediné, co se mění, je jak moc je povrch natočen ke světlu: plocha kolmo ke světlu je nejjasnější, plocha pod úhlem je tmavší.

**3. Spekulární (specular) — Blinnova aproximace:**
```
Ls = cp·cs · I · max(0, ⟨n, h⟩)^p
```
- h = půlící vektor mezi l a v (view direction)
- p = exponent lesku (vyšší → ostřejší odlesk)
- Závisí na **pozorovateli**

> **Intuice:** Spekulární složka vytváří ten **lesklý odlesk** (highlight), který vidíte na lakovaných, kovových nebo mokrých površích. Na rozdíl od difúzní složky závisí na tom, odkud se díváte — když pohnete hlavou, odlesk se posune. Exponent `p` určuje, jak "ostrý" odlesk je: nízké p (5-10) = rozmazaný odlesk jako na plastiku, vysoké p (100-500) = ostrý bod jako na zrcadle.

**Celkem:**
```
L = La + Ld + Ls
```

#### Kompletní numerický příklad

Mějme povrch s těmito parametry:
- Barva povrchu (difúzní koeficient): **cd = (0.8, 0.2, 0.1)** (červený povrch)
- Spekulární koeficient: **cs = (1.0, 1.0, 1.0)** (bílý odlesk), cp = 0.5
- Intenzita světla: **I = 1.0**
- Ambientní intenzita: **Ia = 0.15**
- Úhel mezi normálou **n** a směrem ke světlu **l**: **60°** → cos(60°) = 0.5
- Úhel mezi normálou **n** a půlícím vektorem **h**: **30°** → cos(30°) ≈ 0.866
- Exponent lesku: **p = 32**

**Krok 1 — Ambientní složka:**
```
La = cd · Ia = (0.8, 0.2, 0.1) · 0.15 = (0.120, 0.030, 0.015)
```

**Krok 2 — Difúzní složka:**
```
Ld = cd · I · max(0, cos 60°)
   = (0.8, 0.2, 0.1) · 1.0 · 0.5
   = (0.400, 0.100, 0.050)
```

**Krok 3 — Spekulární složka:**
```
Ls = cp · cs · I · max(0, cos 30°)^32
   = 0.5 · (1.0, 1.0, 1.0) · 1.0 · (0.866)^32
   = 0.5 · (1.0, 1.0, 1.0) · 0.0095
   = (0.00475, 0.00475, 0.00475)
```
(Poznámka: 0.866^32 ≈ 0.0095 — i malá odchylka od ideálního odrazu velmi rychle snižuje spekulární příspěvek díky vysokému exponentu)

**Krok 4 — Celkový výsledek:**
```
L = La + Ld + Ls
  = (0.120 + 0.400 + 0.00475,  0.030 + 0.100 + 0.00475,  0.015 + 0.050 + 0.00475)
  = (0.525, 0.135, 0.070)
```

Výsledná barva je tmavě červená s drobným bílým nádechem od spekulárního odlesku.

### Stínování (shading)
| Metoda | Popis | Kvalita |
|---|---|---|
| Konstantní (flat) | jedna barva na trojúhelník | nejnižší |
| Gouraudovo | interpolace barvy z vrcholů | střední |
| Phongovo | interpolace normál, výpočet osvětlení per-pixel | nejvyšší |

> **Vizuální rozdíl v praxi:**
> - **Flat shading:** Každý trojúhelník má jednu konstantní barvu. Vidíte jasně **hranice každého trojúhelníku** — model vypadá hranatě, jako origami.
> - **Gouraudovo shading:** Barvy se plynule interpolují přes trojúhelník, takže přechody jsou hladké. Ale osvětlení se počítá jen ve vrcholech — pokud je spekulární odlesk (highlight) **uprostřed velkého trojúhelníku**, Gouraud ho úplně mine, protože žádný z vrcholů leží v "lesklé" oblasti.
> - **Phongovo shading:** Normály se interpolují a osvětlení se počítá **pro každý pixel zvlášť**. Odlesky jsou vždy na správném místě, i uprostřed trojúhelníku. Výpočetně nejnáročnější, ale výsledek je nejvěrnější.

---

## Globální zobrazovací metody

**Vs lokální (Phong):** Lokální bere v úvahu jen přímé světlo. Globální simuluje **interakce světla mezi objekty**.

| Metoda | Princip |
|---|---|
| Ray tracing | paprsky od pozorovatele, rekurzivní odrazy/lomy |
| Radiosity | energetický model, difúzní povrchy |
| Path tracing | Monte Carlo ray tracing, fotorealistické |

**Lokální vs Globální:**
- Lokální: rychlý, real-time
- Globální: pomalý (offline rendering), fotorealistický, zahrnuje odrazy, stíny, globální osvětlení

### Rasterizace vs Ray tracing — fundamentální rozdíl

> **Analogie:** Představte si místnost plnou předmětů a vy chcete nakreslit obraz toho, co vidíte oknem.
>
> **Rasterizace** = "Pro každý předmět v místnosti zjisti, do kterých pixelů okna se promítá." Vezmete každý trojúhelník a zjistíte, kam na obrazovce dopadne. Je to rychlé, protože GPU umí zpracovávat trojúhelníky paralelně, ale neumí snadno řešit odrazy a stíny.
>
> **Ray tracing** = "Pro každý pixel okna vystřelte paprsek a zjistěte, na jaký předmět narazí." Z každého pixelu vysíláte paprsek do scény a sledujete, co zasáhne. Když paprsek narazí na zrcadlo, vyšlete nový paprsek ve směru odrazu — tak přirozeně řešíte odrazy, lomy a stíny.
>
> Jinými slovy: rasterizace je **"object-order"** (procházíme objekty), ray tracing je **"image-order"** (procházíme pixely).

---

## Kontrolní otázky

**Q: Čím se liší kolmá a perspektivní projekce?**
Geometricky: kolmá = rovnoběžné paprsky, perspektivní = paprsky z jednoho bodu. Matematicky: kolmá = lineární, W=1; perspektivní = dělení z, W≠1.

**Q: Co je pohledový objem?**
Oblast prostoru viditelná kamerou — kvádr pro ortho, komolý jehlan (frustum) pro perspektivu. Parametry: near, far, fov, aspect.

**Q: Kde se nachází z-buffer v pipeline?**
V **rastrové** části (po rasterizaci, per-fragment operace).

**Q: Co je z-fighting a jak mu bráníme?**
Artefakt z dvou povrchů na stejné hloubce — z-buffer nedokáže rozlišit. Bráníme: zúžit frustum (near↑, far↓), polygon offset.

**Q: Jaký je hlavní rozdíl lokálního vs globálního modelu?**
Lokální (Phong) = pouze přímé světlo od zdrojů. Globální = interakce světla mezi objekty (odrazy, stíny, průsvit).

---

[[HOME|← Zpět domů]]
