# 01 — Vykreslovací řetězec

tags: #téma1 #pipeline #zkouška

---

## Analogie: montážní linka v továrně

Vykreslovací řetězec si lze představit jako **montážní linku v továrně**. Na začátku máte surovinu (3D geometrii — vrcholy, trojúhelníky), a na konci sjíždí hotový výrobek (2D obraz na monitoru). Mezi tím materiál prochází řadou specializovaných stanic:

1. **Příprava dílů** (vstup geometrie) — ze skladu přijdou surové díly
2. **Tvarování** (vertex shader) — díly se ohnou, otočí a přesunou na správné místo
3. **Zjemnění** (tessellation) — hrubé díly se rozřežou na jemnější kousky
4. **Kontrola kvality** (clipping) — díly, které trčí mimo výrobek, se oříznou
5. **Lakovna** (rasterizace + pixel shader) — každý pixel se obarví, otexturuje, osvětlí
6. **Balení** (frame buffer) — hotový obraz se uloží a pošle na displej

Každá stanice dělá jednu specifickou věc a předává výsledek další — přesně jako GPU zpracovává grafiku.

![[01-pipeline.svg]]

---

## Hlavní kroky řetězce

```
Geometrie (3D) → Vektorová část → Rasterizace → Rastrová část → Obraz (2D)
```

### Vektorová část (geometry pipeline)
Pracuje s **vrcholy (vertices)** a **primitivy** (trojúhelníky, úsečky, body).

Kroky:
1. **Vstup geometrie** — data vrcholů a primitiv
2. **Vertex shader** — transformace vrcholů (World → View → Projection)
3. **Tessellation** — dělení primitiv na jemnější geometrii
4. **Ořezání (clipping)** — odstranění objektů mimo pohledový objem
5. **Mapování** — převod do screen space (souřadnice obrazovky)

### Rastrová část (raster pipeline)
Pracuje s **fragmenty/pixely**.

Kroky:
1. **Rasterizace trojúhelníků** — určení, které pixely patří trojúhelníku
2. **Pixel shader** (fragment shader) — výpočet barvy každého pixelu
3. **Texture blending / Fog blending**
4. **Průhlednost, maska, dither**
5. **Vykreslení** do frame bufferu

---

## Co dělá každý shader — na příkladu rotující krychle

Představte si, že vykreslujete **rotující texturovanou krychli** (6 stěn, 12 trojúhelníků, textura dřeva):

### Vertex shader
Spustí se **jednou pro každý vrchol** krychle (8 vrcholů). Jeho úkoly:

- **Model transformace:** Aplikuje rotaci krychle — pokud se krychle točí kolem osy Y o 45 stupňů, vertex shader vynásobí pozici každého vrcholu rotační maticí.
- **View transformace:** Přesune scénu tak, aby kamera byla v počátku — jako byste místo kamery posunuli celý svět.
- **Projection transformace:** Aplikuje perspektivní projekci — vzdálenější vrcholy se přiblíží ke středu obrazu (efekt zmenšování do dálky).

Konkrétně: vrchol krychle na pozici `(1, 1, 1)` projde třemi maticovými násobeními a skončí třeba na pozici `(0.3, 0.7)` v obrazovce.

### Tessellation (volitelný krok)
Pokud je zapnutý, dělí primitivy na jemnější geometrii **přímo na GPU**. U krychle to není typicky potřeba, ale u terénu nebo organických modelů je to klíčové (viz sekce níže).

### Geometry shader (volitelný)
Může generovat nové primitivy z existujících — např. z bodů vytvořit trojúhelníky pro systém částic (particle system).

### Pixel (fragment) shader
Spustí se **jednou pro každý pixel**, který trojúhelník pokrývá. Jeho úkoly:

- **Texturování:** Pro daný pixel zjistí UV souřadnice a přečte barvu z textury dřeva.
- **Osvětlení:** Spočítá, jak moc světlo dopadá na daný bod povrchu (např. Phongův model — ambientní + difuzní + spekulární složka).
- **Výsledná barva:** Kombinuje texturu a osvětlení: `barva = textura * osvětlení`.

Pokud trojúhelník pokrývá 10 000 pixelů, pixel shader se spustí 10 000krát — proto je to nejnáročnější část pipeline.

---

## Tessellation podrobněji

Tessellation slouží k **dynamickému zjemnění geometrie**. Místo toho, abyste posílali na GPU miliony trojúhelníků, pošlete hrubý model a GPU ho sám rozdělí.

### Příklad: terénní mapa

1. Začnete s **hrubou mřížkou** — terén jako 4 velké čtverce (= 8 trojúhelníků).
2. Tessellator každý trojúhelník **rozřeže na menší** — např. úroveň 4 znamená 4x4 = 16 menších trojúhelníků z jednoho.
3. **Displacement shader** pak posune nové vrcholy nahoru/dolů podle výškové mapy — z ploché mřížky vzniknou hory a údolí.

**Proč ne rovnou poslat detailní model?**
- Blízké objekty potřebují hodně detailů, vzdálené stačí hrubé — tessellation umožňuje **Level of Detail (LOD)** dynamicky.
- Šetří přenos dat z CPU → GPU (posíláte hrubý model + pravidla, ne miliony trojúhelníků).

### Tessellation pipeline se skládá ze tří fází:
1. **Hull shader** — rozhodne, na kolik dílů se má primitiv rozdělit (tessellation factor)
2. **Tessellator** (pevná funkce) — provede samotné dělení
3. **Domain shader** — umístí nové vrcholy (např. podle displacement mapy)

---

## Ořezání (Clipping) podrobněji

Clipping je proces, při kterém se **odstraní geometrie mimo viditelný objem** (view frustum = komolý jehlan, který představuje to, co kamera "vidí").

### Příklad: trojúhelník napůl mimo obrazovku

Představte si trojúhelník, jehož dva vrcholy jsou na obrazovce a jeden je vlevo mimo:

- **Před clippingem:** trojúhelník ABC, kde bod A je mimo viditelný prostor.
- **Clipping:** algoritmus najde dva průsečíky hran AB a AC s hranicí viditelného objemu. Označme je P a Q.
- **Po clippingu:** místo jednoho trojúhelníku ABC máme nový trojúhelník PBQ a trojúhelník PQC (nebo jiné rozdělení) — jen tu část, která je viditelná.

**Cohen-Sutherland algoritmus** (pro 2D úsečky):
- Každému koncovému bodu přiřadí 4bitový kód (nahoře/dole/vlevo/vpravo)
- Pokud oba kódy = 0000 → úsečka je celá uvnitř (trivial accept)
- Pokud AND kódů ≠ 0 → úsečka je celá mimo (trivial reject)
- Jinak → úsečka se musí oříznout

---

## Konkrétní příklad: Co se stane krok po kroku při vykreslení jednoho texturovaného trojúhelníku?

Mějme trojúhelník s vrcholy A, B, C a texturou cihel. Projdeme celý pipeline:

### 1. Vstup geometrie (Input Assembly)
- GPU přečte ze paměti tři vrcholy: A(0,0,0), B(1,0,0), C(0.5,1,0)
- Ke každému vrcholu patří: pozice, normála, UV souřadnice textury
- Např.: A má UV=(0,0), B má UV=(1,0), C má UV=(0.5,1)

### 2. Vertex shader
- Každý vrchol se transformuje:
  - **Model matice:** posune trojúhelník na pozici (3, 0, -5) ve světě
  - **View matice:** transformuje do prostoru kamery
  - **Projection matice:** aplikuje perspektivu
- Výstup: trojúhelník v **clip space** (souřadnice od -1 do 1)
- UV souřadnice projdou beze změny dál

### 3. Clipping
- GPU zkontroluje, zda trojúhelník zasahuje mimo viditelný objem
- Pokud ano → ořízne ho a vytvoří nové trojúhelníky
- Pokud je celý uvnitř → projde bez změny

### 4. Viewport transformace
- Clip space (-1 do 1) se převede na **pixel souřadnice** (0 do 1920, 0 do 1080)
- Trojúhelník teď má pozice v pixelech na obrazovce

### 5. Rasterizace
- GPU zjistí, které pixely leží uvnitř trojúhelníku
- Pro každý takový pixel vytvoří **fragment** s interpolovanými hodnotami:
  - Interpolovaná UV souřadnice (pomocí barycentrických souřadnic)
  - Interpolovaná normála
  - Interpolovaná hloubka (pro Z-buffer)

### 6. Pixel (fragment) shader
- Pro každý fragment (dejme tomu jich je 5000):
  - Přečte barvu z textury cihel na pozici UV → např. červenohnědá (180, 80, 60)
  - Spočítá osvětlení z normály a pozice světla → např. difuzní faktor 0.7
  - Výsledná barva: (180*0.7, 80*0.7, 60*0.7) = (126, 56, 42)

### 7. Výstupní operace (Output Merger)
- **Z-test:** porovná hloubku fragmentu s hodnotou v Z-bufferu. Pokud je fragment blíž ke kameře, zapíše se; jinak se zahodí (je za jiným objektem).
- **Blending:** pokud je trojúhelník poloprůhledný, smíchá se s tím, co už je v bufferu.
- Výsledná barva se zapíše do **frame bufferu**.

### 8. Zobrazení
- Frame buffer se pošle na monitor a trojúhelník je vidět jako texturovaná plocha.

---

## Principiální rozdíl vektorové vs rastrové části

| | Vektorová | Rastrová |
|---|---|---|
| Vstup | vrcholy, primitiva | fragmenty |
| Doména | 3D prostor → 2D | 2D pixelová mřížka |
| Klíčová operace | transformace | interpolace, texturování |
| Shader | vertex shader | pixel/fragment shader |
| Počet spuštění | jednou na vrchol | jednou na pixel |

---

## Pevné vs programovatelné části

- **Pevné (fixed function):** starší GPU, nelze změnit chování (rasterizace, clipping, Z-test — ty jsou pevné i dnes)
- **Programovatelné:** vertex shader a pixel shader jsou dnes plně uživatelsky programovatelné (od DirectX 8+)
- Tessellation shadery přibyly v DirectX 11, compute shader umožňuje obecné výpočty na GPU

**Moderní pipeline (DirectX 11+) má tyto programovatelné stupně:**
1. Vertex Shader
2. Hull Shader (tessellation control)
3. Domain Shader (tessellation evaluation)
4. Geometry Shader
5. Pixel/Fragment Shader
6. Compute Shader (mimo grafický pipeline)

---

## Kontrolní otázky

**Q: Jaké jsou hlavní kroky vykreslovacího řetězce?**
Geometrie → vertex shader → clipping → rasterizace → pixel shader → výstup.

**Q: Jaký je principiální rozdíl mezi vektorovou a rastrovou částí?**
Vektorová pracuje s vrcholy a primitivy ve 3D → 2D transformaci; rastrová pracuje s pixely a určuje jejich barvu.

**Q: Co jsou vertex a fragment shader a čím se liší od pevných částí?**
Jsou to programovatelné procesory na GPU — uživatel sám definuje algoritmus transformace/stínování. Pevné části měly napevno zakódované operace.

**Q: Co je tessellation a k čemu slouží?**
Tessellation dynamicky dělí hrubou geometrii na jemnější přímo na GPU. Slouží k úspoře přenosu dat a dynamickému LOD — blízké objekty mají víc detailů, vzdálené méně.

**Q: Proč se provádí clipping?**
Aby se nemusely rasterizovat trojúhelníky (nebo jejich části), které nejsou vidět — šetří to výpočetní výkon a zabraňuje artefaktům mimo obrazovku.

**Q: Co dělá pixel shader a proč je výpočetně nejnáročnější?**
Spustí se pro každý pixel pokrytý geometrií — počítá texturování, osvětlení a výslednou barvu. Protože pixelů je řádově miliony (např. 1920x1080 = 2 miliony), je to nejčastěji volaná část pipeline.

---

[[HOME|← Zpět domů]]
