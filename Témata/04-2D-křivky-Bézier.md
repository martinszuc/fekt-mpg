# 04 — 2D Křivky a Bézierovy křivky

tags: #téma4 #bezier #křivky #deCasteljau #zkouška

---

## Aproximační vs Interpolační křivky

| | Aproximační | Interpolační |
|---|---|---|
| Prochází řídicími body? | **NE** (pouze přibližuje) | **ANO** |
| Příklad | **Bézierovy křivky** | Catmull-Rom spline, Lagrange |
| Výhoda | hladší, předvídatelnější | prochází zadanými body |

**→ Bézierovy křivky jsou APROXIMAČNÍ.**

#### Intuitivní vysvětlení
Představte si, že máte v desce zabodnuté **4 špendlíky** (řídicí body):

- **Interpolační křivka** = ohebný drát, který musí projít **každým** špendlíkem. Drát se prohne a může vytvořit nepěkné tvary, protože je nucen přesně trefit všechny body.
- **Aproximační křivka** = gumička natažená kolem špendlíků. Gumička se k nim přibližuje, je jimi přitahována, ale nemusí se jich dotýkat (kromě prvního a posledního). Výsledný tvar je přirozenější a hladší.

Bézierova křivka funguje jako ta gumička — řídicí body ji „přitahují", ale ona prochází jen prvním (P₀) a posledním (P₃) bodem.

---

## Maticový tvar křivky

```
q(t) = T · C = [t³  t²  t  1] · C
```

kde matice `C` (4×2 pro 2D) obsahuje koeficienty polynomů.

**Posun křivky o 1 ve směru osy y:**
Změní se hodnota `d_y` (poslední řádek, druhý sloupec) — přičteme 1.
Protože `q(t) = ... + [1] · [d_x, d_y]`, tedy `d_y` je absolutní člen pro y-složku.

---

## Bézierova kubika

- **Stupeň:** 3 (kubika)
- **Počet řídicích bodů:** 4 (P₀, P₁, P₂, P₃)
- **Počet čísel (2D):** 4 × 2 = **8 čísel**
- **Počet čísel (3D):** 4 × 3 = **12 čísel**

### Proč kubiky?
- Stupeň 1 = úsečka, stupeň 2 = kvadratika (omezená flexibilita)
- Stupeň 3 = kubika: **nejnižší stupeň umožňující inflexní bod** (změnu zakřivení)
- Vyšší stupně: numericky nestabilní, těžko ovladatelné
- Kubika je ideální kompromis: dostatečná flexibilita + stabilita

### Bernsteinovy polynomy (váhové funkce)
Pro kubiku (n=3):
```
B₀,₃(t) = (1-t)³
B₁,₃(t) = 3t(1-t)²
B₂,₃(t) = 3t²(1-t)
B₃,₃(t) = t³
```
Vlastnosti: vždy ≥ 0, součet = 1 → **convex hull property** (křivka leží v konvexním obalu řídicích bodů).

![[04-bernstein.svg]]

#### Konvexní obal — intuice
Představte si, že řídicí body jsou **hřebíky v desce** a kolem nich natáhnete **gumičku** tak, aby obepínala všechny body zvenku — to je konvexní obal. Convex hull property říká, že **křivka nikdy nemůže uniknout z tohoto ohraničení**. Pokud všechny 4 body leží uvnitř obrazovky, pak i celá křivka leží uvnitř obrazovky. To je extrémně užitečné pro:
- **Detekci kolizí:** pokud se konvexní obaly dvou křivek nepřekrývají, křivky se určitě neprotínají.
- **Ořezávání:** pokud je konvexní obal mimo viditelnou oblast, křivku nemusíme kreslit.
- **Odhad polohy:** vždy víme, kde křivka přibližně leží.

---

## Algoritmus de Casteljau

Geometrická metoda výpočtu bodu na Bézierově křivce v čase t.

### Postup pro kubiku (4 body: P₀, P₁, P₂, P₃):

**Úroveň 1:** lineární interpolace sousedních bodů
```
P₀¹ = (1-t)·P₀ + t·P₁
P₁¹ = (1-t)·P₁ + t·P₂
P₂¹ = (1-t)·P₂ + t·P₃
```

**Úroveň 2:**
```
P₀² = (1-t)·P₀¹ + t·P₁¹
P₁² = (1-t)·P₁¹ + t·P₂¹
```

**Úroveň 3 (výsledek):**
```
q(t) = P₀³ = (1-t)·P₀² + t·P₁²
```

![[04-decasteljau.svg]]

### Kompletní numerický příklad: t = 0,5

**Řídicí body:** P₀ = (0, 0), P₁ = (1, 3), P₂ = (3, 3), P₃ = (4, 0)

Pro t = 0,5 je (1 − t) = 0,5, takže každý mezibod je prostě **střed** dvou bodů.

**Úroveň 1** (3 body):
```
P₀¹ = 0,5·(0,0) + 0,5·(1,3) = (0,5;  1,5)
P₁¹ = 0,5·(1,3) + 0,5·(3,3) = (2,0;  3,0)
P₂¹ = 0,5·(3,3) + 0,5·(4,0) = (3,5;  1,5)
```

**Úroveň 2** (2 body):
```
P₀² = 0,5·(0,5; 1,5) + 0,5·(2,0; 3,0) = (1,25;  2,25)
P₁² = 0,5·(2,0; 3,0) + 0,5·(3,5; 1,5) = (2,75;  2,25)
```

**Úroveň 3** (výsledný bod):
```
q(0,5) = 0,5·(1,25; 2,25) + 0,5·(2,75; 2,25) = (2,0;  2,25)
```

**Výsledek: q(0,5) = (2,0; 2,25)**

### Příklad: t = 3/4

Pro t = 3/4, tedy (1-t) = 1/4:
- **Úroveň 1:** každé `Pᵢ¹ = (1/4)·Pᵢ + (3/4)·Pᵢ₊₁`
- **Úroveň 2:** každé `Pᵢ² = (1/4)·Pᵢ¹ + (3/4)·Pᵢ₊₁¹`
- **Úroveň 3:** `q = (1/4)·P₀² + (3/4)·P₁²`

### Využití de Casteljau — Subdivision
Body z mezivýpočtů definují **dvě poloviny** křivky:
- Levá polovina: P₀, P₀¹, P₀², q(t)
- Pravá polovina: q(t), P₁², P₂¹, P₃

---

## Napojení křivek

### Parametrické (Cⁿ) napojení
Shoda derivací v uzlovém bodě:
- **C⁰:** q₁(1) = q₂(0) — křivky se dotýkají
- **C¹:** q₁'(1) = q₂'(0) — shodné tečné vektory (stejný směr i délka)
- **C²:** q₁''(1) = q₂''(0) — shodná zakřivení

### Geometrické (Gⁿ) napojení
Mírnější podmínka — pouze **směr** tečny musí souhlasit, ne délka:
- **G¹:** tečné vektory jsou kolineární (souběžné), ale mohou mít různé délky

![[04-napojeni.svg]]

#### Vizuální příklady napojení

- **C⁰ (pozice):** Dvě křivky se setkávají v jednom bodě, ale mají **různé směry** — vzniká **ostrý roh**. Jako když zlomíte drát — konce se dotýkají, ale každý míří jinam. Příklad: lomená čára, písmeno „V".

- **G¹ (geometrická spojitost):** Křivky sdílejí **stejný směr tečny** v místě napojení — přechod vypadá hladce. Ale rychlost „průjezdu" se může skokově změnit. Představte si auto, které projíždí zatáčkou plynule, ale na hranici úseků náhle zrychlí nebo zpomalí. Lidské oko rozdíl nevidí, ale matematicky se liší délka tečného vektoru.

- **C¹ (parametrická spojitost):** Tečné vektory jsou zcela **shodné** — stejný směr **i délka**. Auto projíždí zatáčkou konstantní rychlostí bez jakéhokoli trhnutí. To je nejsilnější podmínka pro hladké napojení kubik.

**Rozdíl C¹ vs G¹:**
- C¹ → G¹ (silnější podmínka implikuje slabší)
- G¹ ≠ C¹ (G¹ nevyžaduje stejnou délku tečného vektoru)

---

## Racionální Bézierovy křivky

- Každý řídicí bod má váhu wᵢ
- `q(t) = Σ wᵢ·Pᵢ·Bᵢ,ₙ(t) / Σ wᵢ·Bᵢ,ₙ(t)`
- **Výhoda:** lze přesně vyjádřit kuželosečky (kružnice, elipsy, paraboly)
- Pokud wᵢ = 0 → bod má nulový vliv (jako by neexistoval)

#### Proč standardní Bézier nedokáže přesně vykreslit kružnici?
Bézierova křivka je **polynomická** — je definována polynomy v parametru t. Kružnice je ale definována rovnicí x² + y² = r², což je **algebraická křivka**, nikoli polynomická parametrická křivka. Matematicky lze dokázat, že žádný polynom nemůže přesně popsat kružnici (pokus o aproximaci vždy dává mírně „splácklý" tvar).

**Řešení — racionální Bézier s váhami:**
Váhy wᵢ přidávají do vzorce dělení, čímž se z polynomu stane **racionální funkce** (podíl polynomů). Racionální funkce už umí přesně vyjádřit kružnici. Například čtvrtkruh lze zapsat pomocí 3 řídicích bodů s váhami w₀ = 1, w₁ = 1/√2 ≈ 0,707, w₂ = 1. Váha w₁ < 1 "přitáhne" křivku blíže ke středu a vytvoří přesný kruhový oblouk.

## NURBS
- Non-Uniform Rational B-Splines
- **Výhody oproti Bézier:**
  1. Lokální editace (změna jednoho bodu neovlivní celou křivku)
  2. Libovolně dlouhé křivky bez explicitního napojování segmentů

#### Lokální kontrola — konkrétní příklad
Představte si NURBS křivku s 10 řídicími body tvořící profil automobilu. Pokud v Bézierově křivce pohnete jedním řídicím bodem (např. bodem P₅ u střechy), změní se tvar **celé** křivky — od kapoty po kufr. U NURBS pohnete bodem P₅ a změní se **jen okolní úsek** křivky (přibližně oblast ovlivněná body P₃ až P₇). Zbytek křivky zůstane naprosto beze změny. To je důvod, proč se NURBS používá v CAD systémech — designér může lokálně doladit tvar bez obav, že tím zničí celý model.

---

## Kontrolní otázky

**Q: Jsou Bézierovy křivky aproximační nebo interpolační?**
**Aproximační** — neprochází řídicími body (jen prvním a posledním).

**Q: Kolik řídicích bodů má 2D Bézierova kubika a kolik čísel?**
4 body, 4 × 2 = **8 čísel**.

**Q: Proč se používají kubiky?**
Nejnižší stupeň umožňující inflexní bod, dobrý kompromis mezi flexibilitou a numerickou stabilitou.

**Q: Který parametr v C změnit, aby se křivka posunula o 1 ve směru y?**
Přičíst 1 k hodnotě `d_y` (absolutní člen y-složky, poslední řádek, druhý sloupec matice C).

---

[[HOME|← Zpět domů]]
