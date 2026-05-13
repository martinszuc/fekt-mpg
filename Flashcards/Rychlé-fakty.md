# Rychlé fakty — Flashcards

tags: #flashcards #opakování

> Formát: **Otázka** → *Odpověď*. Zakryj odpovědi a zkoušej.

---

## Barva & Obraz

**Jakých čípků je nejméně?** → *Modrých (S-čípky)*

**R + G v RGB = ?** → *Žlutá (Yellow)*

**R + B = ?** → *Purpurová (Magenta)*

**G + B = ?** → *Azurová (Cyan)*

**Šedotónový obraz = f(? proměnných)?** → *Dvou (x, y)*

**Video signál = f(? proměnných)?** → *Tří (x, y, t)*

**Gama korekce — proč?** → *Monitor má nelineární odpověď, korekce ji linearizuje*

**YCbCr — co je Y?** → *Luma (jas)*

---

## Křivky

**Bézier = aproximační nebo interpolační?** → *Aproximační*

**Kubika — počet řídicích bodů?** → *4*

**Kubika 2D — počet čísel?** → *8 (4×2)*

**Kubika 3D — počet čísel?** → *12 (4×3)*

**Bikubický plát — řídicích bodů?** → *16 (4×4)*

**Bikubický plát — čísel (3D)?** → *48 (16×3)*

**Proč kubiky v CG?** → *Nejnižší stupeň s inflexním bodem, dobrý kompromis*

**C¹ vs G¹ — rozdíl?** → *C¹: shodné tečné vektory (i délka). G¹: kolineární (jen směr)*

**De Casteljau — k čemu?** → *Výpočet bodu na křivce + subdivision*

---

## Rasterizace

**DDA vs Bresenham — hlavní rozdíl?** → *DDA: reálná aritmetika. Bresenham: celočíselná, rychlejší*

**Kružnice — kolik symetrií?** → *8 (oktanty)*

**Elipsa — kolik symetrií?** → *4 (kvadranty)*

**Barycentrické souřadnice těžiště?** → *α = β = γ = 1/3*

---

## Transformace & Projekce

**Homogenní souřadnice — proč?** → *Umožňují posunutí jako maticové násobení*

**Záleží na pořadí transformací?** → *Ano (obecně), výjimka: rotace kolem stejné osy*

**Kolmá projekce — geometricky?** → *Rovnoběžné paprsky kolmé na průmětnu*

**Perspektivní projekce — matematicky?** → *Dělení z-souřadnicí: x'=x·d/z*

**Pohledový objem — ortho tvar?** → *Kvádr*

**Pohledový objem — perspektiva tvar?** → *Komolý jehlan (frustum)*

---

## Osvětlování & Textury

**Tři složky Phongova modelu?** → *Ambientní, difúzní, spekulární*

**Difúzní složka závisí na pozorovateli?** → *NE (závisí na normále a světle)*

**Spekulární složka závisí na pozorovateli?** → *ANO (používá vektor pohledu v)*

**MIP-mapping — proč?** → *Prevence aliasingu u vzdálených textur*

**MIP-mapping paměťová cena?** → *~33% navíc*

**Trilineární interpolace — co interpoluje?** → *Bilineárně ve 2 MIP úrovních + lineárně mezi nimi*

---

## GPU & Paralelní

**SIMD = ?** → *Single Instruction, Multiple Data*

**Vertex shader — vstup/výstup?** → *1 vertex → 1 transformovaný vertex*

**Pixel shader — vstup/výstup?** → *1 fragment → 1 barva (RGBA)*

**Datový typ shaderů?** → *128-bit quad-vector: 4× float32 [X,Y,Z,W]*

**Z-buffer — kde v pipeline?** → *Rastrová část (per-fragment)*

**Z-fighting — co způsobuje?** → *Omezená přesnost z-bufferu u koplanárních povrchů*

**Catmull-Clark — pro jaké sítě?** → *Čtyřúhelníkové*

---

[[HOME|← Zpět domů]]
