# Kontrolní otázky — Všechny s odpověďmi

tags: #kontrolníOtázky #ústní #zkouška

> Kompletní sada kontrolních otázek ze všech kapitol, seřazeno dle tématu.

---

## Vykreslovací řetězec

**Jaké jsou hlavní kroky vykreslovacího řetězce?**
Geometrie → Vertex shader → Clipping → Mapování → Rasterizace → Pixel shader → Blending → Výstup do frame bufferu.

**Jaký je principiální rozdíl mezi vektorovou a rastrovou částí řetězce?**
Vektorová pracuje s vrcholy/primitivy a transformuje je ze 3D do 2D. Rastrová pracuje s fragmenty (pixely) a určuje jejich výslednou barvu.

**Co jsou vertex shader a fragment shader a čím se liší od pevných částí řetězce?**
Jsou to programovatelné procesory na GPU — uživatel definuje vlastní algoritmus. Pevné části mají napevno zakódované operace, nelze je měnit.

---

## Sítě a triangulace

**Geometrické vs topologické aspekty sítí:**
Geometrie = pozice vrcholů v prostoru (souřadnice). Topologie = jak jsou vrcholy propojeny (hrany, stěny).

**Přednosti trojúhelníkových sítí:**
- Vždy planární (3 body definují rovinu)
- Jednoduché pro rasterizaci a výpočet normál
- Standardní primitiv GPU

**Vlastnosti validní trojúhelníkové sítě:**
- Každá hrana sdílena max. dvěma trojúhelníky
- Konzistentní orientace (normály ven)
- Bez izolovaných vrcholů/hran

**Jak určíme normály v trojúhelníkové síti?**
Normála trojúhelníku: vektorový součin dvou hran: n = (B−A) × (C−A), pak normalizovat.
Normála vrcholu: průměr normál sousedních trojúhelníků.

**Triangularizace čtyřúhelníku — kolik možností?**
Dvě: podle jedné nebo druhé úhlopříčky.

**Kdy je vhodná adaptivní síť?**
Tam kde povrch má různou míru detailů — hustší síť kde je velké zakřivení, řidší tam kde je rovný.

**Catmull-Clark — pro jaké topologie?**
Čtyřúhelníky.

---

## 2D křivky

**Aproximační vs interpolační:**
Aproximační — neprochází řídicími body. Interpolační — prochází všemi řídicími body.

**Matice C — posun o 1 ve směru y?**
Přičíst 1 k d_y (poslední řádek, druhý sloupec matice C = absolutní člen y-složky).

**Kolik řídicích bodů má 2D Bézierova kubika? Kolik čísel?**
4 body, 4×2 = 8 čísel.

**Proč kubiky?**
Nejnižší stupeň s inflexním bodem. Dobrý kompromis flexibilita vs numerická stabilita.

**Jaké typy napojení dvou křivek znáte?**
C⁰ (dotek), C¹ (shodné tečné vektory), C² (shodné zakřivení); G¹ (kolineární tečny — slabší než C¹).

**De Casteljau pro t=3/4 — viz [[Matematika/Klíčové-vzorce#De Casteljau]]**

---

## Racionální Bézierovy křivky / NURBS

**Čím se liší racionální Bézierovy od obyčejných?**
Každý řídicí bod má váhu wᵢ. Výsledná křivka je podíl dvou polynomů.

**Vliv vah:**
Vyšší váha → křivka se více přibližuje k danému řídicímu bodu. wᵢ = 0 → bod nemá vliv.

**Výhoda NURBS oproti Bézier:**
Lokální editace (změna jednoho bodu neovlivní celou křivku) + neomezená délka bez explicitního napojování.

---

## Rasterizace

**Co je rasterizace?**
Převod vektorových objektů na diskrétní pixely rastrového displeje.

**Algoritmy pro úsečku:**
DDA (reálná aritmetika, jednoduché), Bresenham (celočíselný, rychlejší), Mid-point (implicitní tvar).

**Úsečka m=1/4 — jak dopadne rasterizace?**
Pro každé 4 kroky v x nastane 1 krok v y (pohyb diagonálně jednou za čtyři kroky).

**Bresenham identicky pro všechny směrnice?**
Ne — závisí na quadrantu a směrnici, nutné symetrizovat.

**Rasterizace přerušované čáry:**
Stejný algoritmus, ale pixel se vykreslí jen pokud vzor (pattern) na dané pozici = 1.

**Rasterizace silné čáry — problémy:**
Nepěkné spoje v rozích, různá tloušťka při různých úhlech.

**Rasterizace kružnice:**
8-násobná symetrie → stačí 1/8, midpoint algoritmus s celými čísly.

**Rasterizace elipsy:**
4-násobná symetrie (kvadranty), dvě fáze s přepnutím řídicí osy v bodě zlomu (tečna = −45°).

**Neadaptivní vs adaptivní rasterizace Bézier:**
Neadaptivní: rovnoměrné dělení t. Adaptivní: rekurzivní subdivision, dělí kde je zakřivení.

**Barycentrické souřadnice — co jsou, využití, těžiště:**
Trojice (α,β,γ), α+β+γ=1. Popisuje polohu bodu vůči vrcholům trojúhelníka. Použití: interpolace hodnot (barvy, UV). Těžiště: α=β=γ=1/3.

---

## 3D pláty

**Parametrický popis obecné 3D plochy:**
q(u,v) = [x(u,v), y(u,v), z(u,v)], u,v ∈ [0,1].

**Co vyjadřuje normálový vektor?**
Směr kolmý na tečnou rovinu povrchu. Vypočte se jako ∂q/∂u × ∂q/∂v.

**Maticový tvar bikubické plochy:**
q(u,v) = U · Mᴮ · G · Mᴮᵀ · Vᵀ. G = 16 řídicích bodů, Mᴮ = Bézierova báze.

**Kolik řídicích bodů — Bézierova bikubická plocha?**
16 bodů.

**C¹ napojení dvou plátů:**
Sdílení celé hrany (4 body) + kolinearita přilehlých řad řídicích bodů s hranou.

**Výhody NURBS plátů oproti Bézier:**
1. Lokální editace. 2. Přesná reprezentace kuželoseček.

**Polygonizace plátu a subdivision:**
Subdivision dělí plát opakovaně na menší → limitní plocha = hladký povrch. Polygonizace = aproximace hladkého plátu trojúhelníkovou sítí.

**Výhody trojúhelníků vs čtyřúhelníků při polygonizaci:**
Trojúhelníky jsou vždy planární, jednodušší pro GPU.

---

## 3D geometrické transformace

**Homogenní souřadnice — definice a proč:**
(x,y,z,w), kartézské = (X/W, Y/W, Z/W). Umožňují vyjádřit posunutí jako maticové násobení.

**Tvar rovnice posunutí:**
Viz [[Matematika/Klíčové-vzorce#Posunutí]].

**Záleží na pořadí transformací?**
Obecně ano (matice nekomutují). Výjimka: rotace kolem stejné osy, uniform scale.

---

## Projektivní transformace

**Kolmá vs perspektivní projekce:**
Kolmá: rovnoběžné paprsky, lineární, zachovává vzdálenosti. Perspektivní: paprsky z jednoho bodu, nelineární (dělení z), vzdálené objekty menší.

**Pohledový objem:**
Oblast viditelná kamerou. Pro ortho = kvádr. Pro perspektivu = frustum. Parametry: near, far, fov, aspect.

**Ořezové roviny:**
Near a far plane — fragment mimo ně se nezobrazí (ořez).

---

## Viditelnost

**Z-buffer — kde v pipeline?**
V rastrové části (per-fragment operace, po rasterizaci).

**Jak jsou uloženy hloubky v z-bufferu?**
Normalizované hodnoty v [0,1], nelineárně — více přesnosti poblíž near plane.

**Z-fighting — co je, proč, jak bránit:**
Blikající artefakt u dvou překrývajících se povrchů. Příčina: omezená přesnost z-bufferu. Řešení: zúžit frustum (near↑, far↓), polygon offset.

---

[[HOME|← Zpět domů]]
