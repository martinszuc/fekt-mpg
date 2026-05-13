# 07 — Promítání a Osvětlování

tags: #téma7 #projekce #Phong #osvětlování #zbuffer #zkouška

---

## Promítání

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

---

## Řešení viditelnosti

### Z-buffer
- V rastrové části pipeline
- Paměť hloubky: pro každý pixel uchovává nejmenší z-souřadnici dosud vykresleného fragmentu
- Nový fragment překreslí pixel jen pokud má **menší z** (je blíže)
- Uložení: **normalizované** hloubky v rozsahu [0, 1], nelineárně (více přesnosti blízko kamery)

### Z-fighting
- **Co je:** Dva povrchy na (skoro) stejné hloubce → z-buffer nedokáže rozlišit, který je před kterým → blikající artefakt
- **Proč:** Omezená přesnost z-bufferu (typicky 24 bitů) + nelineární distribuce přesnosti
- **Jak zabránit:** 
  - Nastavit `near` co nejvyšší a `far` co nejnižší (užší frustum)
  - Polygon offset (GL_POLYGON_OFFSET_FILL)
  - Nevrstvit objekty na stejné z-vzdálenosti

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

### Tři složky oddrazu:

**1. Ambientní (ambient):**
```
La = cd · Ia
```
Rovnoměrné pozadí, žádný specifický zdroj.

**2. Difúzní (diffuse) — Lambertův zákon:**
```
Ld = cd · I · max(0, ⟨n, l⟩)
```
- n = normála povrchu, l = směr ke světlu
- Závisí na úhlu dopadu světla, **nezávisí na pozorovateli**

**3. Spekulární (specular) — Blinnova aproximace:**
```
Ls = cp·cs · I · max(0, ⟨n, h⟩)^p
```
- h = půlící vektor mezi l a v (view direction)
- p = exponent lesku (vyšší → ostřejší odlesk)
- Závisí na **pozorovateli**

**Celkem:**
```
L = La + Ld + Ls
```

### Stínování (shading)
| Metoda | Popis | Kvalita |
|---|---|---|
| Konstantní | jedna barva na trojúhelník | nejnižší |
| Gouraudovo | interpolace barvy z vrcholů | střední |
| Phongovo | interpolace normál, výpočet osvětlení per-pixel | nejvyšší |

---

## Globální zobrazovací metody

**Vs lokální (Phong):** Lokální bere v úvahu jen přímé světlo. Globální simuluje **interakce světla mezi objekty**.

| Metoda | Princip |
|---|---|
| Ray tracing | paprsky od pozorovatele, rekurzivní odrazy/lomy |
| Radiosity | energetický model, difúzní povrchy |
| Path tracing | Monte Carlo ray tracing, fotorealikstické |

**Lokální vs Globální:**
- Lokální: rychlý, real-time
- Globální: pomalý (offline rendering), fotorealistický, zahrnuje odrazy, stíny, globální osvětlení

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
