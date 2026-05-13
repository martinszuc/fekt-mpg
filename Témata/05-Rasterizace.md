# 05 — Rasterizace

tags: #téma5 #rasterizace #bresenham #DDA #zkouška

---

## Co je rasterizace?
Převod vektorových (spojitých) geometrických objektů na **diskrétní pixely** rastrového displeje.

Představte si to jako kreslení na čtverečkový papír — máte dokonalou geometrickou čáru a musíte rozhodnout, které čtverečky (pixely) vybarvit, aby to vypadalo co nejlépe.

---

## Rasterizace úsečky

Řídicí osa: pro |m| ≤ 1 → osa x; pro |m| > 1 → osa y.

> **Proč volíme řídicí osu?** Abychom zaručili, že v každém kroku vykreslíme právě jeden pixel na řídicí ose. Kdyby směrnice |m| > 1 a řídicí osa byla x, přeskočili bychom pixely ve vertikálním směru a čára by měla díry.

### 1. DDA (Digital Differential Analyzer)
- Přímé numerické řešení z y = mx + b
- V každém kroku: x += 1, y += m, zaokrouhlit y
- **Nevýhoda:** používá reálnou aritmetiku (pomalé)
- **Výhoda:** jednoduchý

#### Numerický příklad DDA: úsečka z (2,1) do (7,4)

Spočteme parametry:
- Δx = 7 − 2 = 5, Δy = 4 − 1 = 3
- m = Δy/Δx = 3/5 = 0,6
- |m| ≤ 1 → řídicí osa je x

Začínáme v bodě (2, 1). V každém kroku zvýšíme x o 1 a přičteme m = 0,6 k y:

| Krok | x | y_přesné | y_zaokrouhlené | Vykreslený pixel |
|------|---|----------|----------------|------------------|
| 0    | 2 | 1,0      | 1              | **(2, 1)**       |
| 1    | 3 | 1,6      | 2              | **(3, 2)**       |
| 2    | 4 | 2,2      | 2              | **(4, 2)**       |
| 3    | 5 | 2,8      | 3              | **(5, 3)**       |
| 4    | 6 | 3,4      | 3              | **(6, 3)**       |
| 5    | 7 | 4,0      | 4              | **(7, 4)**       |

Vykreslené pixely: (2,1), (3,2), (4,2), (5,3), (6,3), (7,4).

> **Všimněte si:** V krocích 2 a 4 zůstává y_zaokrouhlené stejné jako v předchozím kroku — to jsou vodorovné segmenty čáry. Celý výpočet pracuje s reálným číslem y, které neustále roste o 0,6.

### 2. Bresenhamův algoritmus
- **Celočíselná** aritmetika → rychlejší
- Rozhodovací člen `p` určuje, zda y naroste

**Postup (pro 0 < m < 1, x₁ < x₂):**
```
k₁ = 2Δy
k₂ = 2(Δy − Δx)
p  = 2Δy − Δx   (inicializace)

Pro každý krok:
  x = x + 1
  if p > 0: y = y+1, p = p + k₂
  else:              p = p + k₁
  vykresli [x, y]
```

**Výsledek shodný s DDA, ale rychlejší.**

#### Numerický příklad Bresenham: úsečka z (2,1) do (7,4)

Parametry:
- Δx = 5, Δy = 3
- k₁ = 2·Δy = 6
- k₂ = 2·(Δy − Δx) = 2·(3 − 5) = −4
- p₀ = 2·Δy − Δx = 6 − 5 = 1

Začínáme vykreslením počátečního bodu (2, 1):

| Krok | p (před) | p > 0? | Akce                | x | y | Vykreslený pixel | p (po)    |
|------|----------|--------|---------------------|---|---|------------------|-----------|
| 0    | —        | —      | počátek             | 2 | 1 | **(2, 1)**       | p₀ = 1    |
| 1    | 1        | ANO    | y+1, p += k₂ = −4  | 3 | 2 | **(3, 2)**       | 1−4 = −3  |
| 2    | −3       | NE     | p += k₁ = 6        | 4 | 2 | **(4, 2)**       | −3+6 = 3  |
| 3    | 3        | ANO    | y+1, p += k₂ = −4  | 5 | 3 | **(5, 3)**       | 3−4 = −1  |
| 4    | −1       | NE     | p += k₁ = 6        | 6 | 3 | **(6, 3)**       | −1+6 = 5  |
| 5    | 5        | ANO    | y+1, p += k₂ = −4  | 7 | 4 | **(7, 4)**       | 5−4 = 1   |

Výsledek: (2,1), (3,2), (4,2), (5,3), (6,3), (7,4) — **shodný s DDA!**

![[05-bresenham.svg]]

#### Proč je Bresenham rychlejší než DDA?

| Operace      | DDA                          | Bresenham                 |
|-------------|------------------------------|---------------------------|
| Sčítání     | y += 0.6 (float)             | p += 6 nebo p += −4 (int) |
| Násobení    | žádné                        | žádné                     |
| Porovnání   | zaokrouhlení (float → int)   | p > 0? (int porovnání)   |
| Typ dat     | **float (reálná čísla)**     | **int (celá čísla)**      |

Klíč: Bresenham nahradil **reálné sčítání a zaokrouhlování** dvěma celočíselnými operacemi (sčítání + porovnání). Na procesoru je celočíselné sčítání výrazně rychlejší než práce s plovoucí řádovou čárkou, navíc odpadá zaokrouhlování.

### 3. Mid-point algoritmus
- Pracuje s **implicitním** tvarem přímky: f(x,y) = y − mx − b = 0
- Výsledek shodný s Bresenhamem

### Přerušovaná čára
Stejný algoritmus, ale pixel se vykreslí jen pokud na příslušné pozici vzoru (pattern) je 1.

---

## Rasterizace kružnice

Využívá **8-násobnou symetrii** — stačí spočítat 1/8 a zrcadlit.
Podmínka: počítáme v oktantu kde x ≤ y.

Implicitní rovnice: `x² + y² − r² = 0`

### Proč 8-násobná symetrie?

Kružnice je symetrická podle os x, y a obou diagonál. Pokud spočítáme bod (x, y) na kružnici v prvním oktantu (úsek od 0° do 45°), získáme automaticky dalších 7 bodů:

**Příklad:** Spočítáme bod **(3, 9)** na kružnici → okamžitě získáme 8 bodů:

| Symetrie          | Bod      |
|-------------------|----------|
| Původní           | (3, 9)   |
| Prohodit x,y      | (9, 3)   |
| Zrcadlit x        | (−3, 9)  |
| Prohodit, zrcadlit| (−9, 3)  |
| Zrcadlit y        | (3, −9)  |
| Prohodit, zrcadlit| (9, −3)  |
| Zrcadlit obě      | (−3, −9) |
| Prohodit, zrcadlit| (−9, −3) |

> Jedno vyhodnocení algoritmu → 8 pixelů! To znamená 8× méně výpočtů.

![[05-symetrie-kruznice.svg]]

### Midpoint algoritmus pro kružnici
```
Inicializace: (x, y) = (0, r), p = 1 − r

if p < 0:  nový bod (x+1, y),   p_new = p + 2x + 3
if p >= 0: nový bod (x+1, y-1), p_new = p + 2(x−y) + 5
```

#### Numerický příklad: kružnice r = 10

Inicializace: (x, y) = (0, 10), p = 1 − 10 = −9

Počítáme body v prvním oktantu (dokud x ≤ y):

| Krok | x | y  | p   | p < 0? | Akce               | Nový bod   |
|------|---|----|-----|--------|---------------------|------------|
| 0    | 0 | 10 | −9  | ANO    | p += 2·0+3 = 3     | **(1, 10)**|
| 1    | 1 | 10 | −6  | ANO    | p += 2·1+3 = 5     | **(2, 10)**|
| 2    | 2 | 10 | −1  | ANO    | p += 2·2+3 = 7     | **(3, 10)**|
| 3    | 3 | 10 | 6   | NE     | p += 2(3−10)+5 = −9| **(4, 9)** |
| 4    | 4 | 9  | −3  | ANO    | p += 2·4+3 = 11    | **(5, 9)** |
| 5    | 5 | 9  | 8   | NE     | p += 2(5−9)+5 = −3 | **(6, 8)** |
| 6    | 6 | 8  | 5   | NE     | p += 2(6−8)+5 = 1  | **(7, 7)** |

V kroku 6 dosáhli jsme x = y = 7, takže končíme (bod na diagonále). Každý z těchto 7 bodů generuje 8 symetrických bodů, celkem 56 pixelů z pouhých 7 výpočtů!

---

## Rasterizace elipsy

- Symetrie pouze 4× (4 kvadranty)
- Nutno **přepnout řídicí osu** v bodě zlomu (kde tečna = −45°)
- Implicitní rovnice: `b²x² + a²y² = a²b²`

### Midpoint pro elipsu — dvě fáze:
- Fáze 1 (řídicí osa x): `pi+1 = pi + b²(2xi + 1)` (p ≤ 0) nebo `pi + b²(2xi+1) − 2a²yi` (p > 0)
- Fáze 2: přepínáme osu, symetrický postup

---

## Rasterizace trojúhelníku

### Barycentrické souřadnice
Bod P uvnitř trojúhelníku ABC vyjádříme jako:
```
P = α·A + β·B + γ·C
kde α + β + γ = 1, α,β,γ ≥ 0
```

- **Těžiště trojúhelníka:** α = β = γ = 1/3
- Využití: interpolace barvy, normál, UV souřadnic přes trojúhelník

#### Geometrický význam

Barycentrické souřadnice α, β, γ odpovídají **poměrům ploch** podtrojúhelníků:

- α = plocha(PBC) / plocha(ABC) — „jak daleko je P od A"
- β = plocha(APC) / plocha(ABC) — „jak daleko je P od B"
- γ = plocha(ABP) / plocha(ABC) — „jak daleko je P od C"

> **Intuice:** Čím blíže je P k vrcholu A, tím větší je α (a menší β, γ). Když P leží přímo na A, pak α=1, β=γ=0.

![[05-barycentricke.svg]]

#### Numerický příklad: trojúhelník A=(0,0), B=(4,0), C=(0,3), bod P=(1,1)

**Krok 1:** Spočítáme plochu celého trojúhelníku ABC:
```
S_ABC = ½ |x_A(y_B − y_C) + x_B(y_C − y_A) + x_C(y_A − y_B)|
      = ½ |0·(0−3) + 4·(3−0) + 0·(0−0)|
      = ½ |0 + 12 + 0|
      = 6
```

**Krok 2:** Spočítáme plochy podtrojúhelníků:
```
S_PBC = ½ |x_P(y_B − y_C) + x_B(y_C − y_P) + x_C(y_P − y_B)|
      = ½ |1·(0−3) + 4·(3−1) + 0·(1−0)|
      = ½ |−3 + 8 + 0|
      = 5/2

S_APC = ½ |x_A(y_P − y_C) + x_P(y_C − y_A) + x_C(y_A − y_P)|
      = ½ |0·(1−3) + 1·(3−0) + 0·(0−1)|
      = ½ |0 + 3 + 0|
      = 3/2

S_ABP = ½ |x_A(y_B − y_P) + x_B(y_P − y_A) + x_P(y_A − y_B)|
      = ½ |0·(0−1) + 4·(1−0) + 1·(0−0)|
      = ½ |0 + 4 + 0|
      = 2
```

**Krok 3:** Barycentrické souřadnice:
```
α = S_PBC / S_ABC = (5/2) / 6 = 5/12 ≈ 0,417
β = S_APC / S_ABC = (3/2) / 6 = 3/12 = 1/4 = 0,250
γ = S_ABP / S_ABC = 2 / 6     = 2/6  = 1/3 ≈ 0,333
```

**Ověření:** α + β + γ = 5/12 + 3/12 + 4/12 = 12/12 = 1 ✓
**Ověření polohy:** P = α·A + β·B + γ·C = 5/12·(0,0) + 1/4·(4,0) + 1/3·(0,3) = (0+1+0, 0+0+1) = (1, 1) ✓

> Všechny tři souřadnice jsou kladné → bod P leží uvnitř trojúhelníku.

#### Pravidla pro hrany trojúhelníků (Edge rules)

Při rasterizaci se trojúhelníky často dotýkají hranami. Bod ležící přesně na sdílené hraně by se bez ošetření vykreslil **dvakrát** — jednou pro každý trojúhelník. To způsobuje:
- **Artefakty v průhlednosti** (pixel je napůl průhledný, ale vykreslí se dvakrát)
- **Z-fighting** (nedeterministicky se zobrazí jeden nebo druhý trojúhelník)

**Řešení — top-left rule (pravidlo levé-horní hrany):**
Pixel na hraně se vykreslí pouze pokud hrana je:
- **Horní (top):** vodorovná hrana, která jde zleva doprava
- **Levá (left):** hrana, která jde směrem dolů

Tím se zaručí, že každý pixel patří právě jednomu trojúhelníku, i na sdílených hranách. GPU toto pravidlo implementují hardwarově.

### Postup rasterizace trojúhelníku
1. Najdi bounding box trojúhelníku
2. Pro každý pixel v bbox: spočítej barycentrické souřadnice
3. Pokud α,β,γ ≥ 0 → pixel je uvnitř → vykresli (s interpolovanou barvou)

---

## Rasterizace Bézierovy křivky

### Neadaptivní
- Rozděl interval [0,1] na N stejných dílků
- Spočítej body q(t) pro t = 0, 1/N, 2/N, ...
- Spoj úsečkami
- Nevýhoda: v rovných místech zbytečně mnoho bodů, v křivolakých málo

### Adaptivní
- Rekurzivní subdivision (de Casteljau)
- Dělíme segment, dokud je dostatečně rovný (kontrola flatness)
- Výhoda: efektivnější, rovnoměrnější kvalita

---

## Kontrolní otázky

**Q: Jaké algoritmy pro rasterizaci úsečky znáte?**
DDA (reálná aritmetika, jednoduchý), Bresenham (celočíselný, rychlejší), Mid-point (implicitní tvar přímky).

**Q: Proč Bresenham nepracuje identicky pro všechny směrnice?**
Algoritmus v základní formě platí jen pro 0 < m < 1, x₁ < x₂. Pro jiné směrnice nutno symetrizovat (prohodit osy, znaménka).

**Q: Co jsou barycentrické souřadnice a jaké má těžiště?**
Trojice (α,β,γ), α+β+γ=1, vyjadřuje polohu bodu vůči vrcholům trojúhelníka. Těžiště: α=β=γ=1/3.

**Q: Jaký je rozdíl neadaptivní vs adaptivní rasterizace Bézierovy křivky?**
Neadaptivní: rovnoměrné dělení parametru t. Adaptivní: rekurzivní subdivision, dělí jen tam kde je zakřivení velké.

**Q: Proč se u kružnice využívá 8-násobná symetrie?**
Kružnice je symetrická podle obou os i obou diagonál. Z jednoho spočítaného bodu (x,y) získáme 8 bodů záměnou a změnou znaménka souřadnic. Tím se výpočet zrychlí 8×.

**Q: Co řeší edge rule při rasterizaci trojúhelníků?**
Zabraňuje dvojímu vykreslení pixelu ležícího na sdílené hraně dvou sousedních trojúhelníků. Pixel se přiřadí tomu trojúhelníku, jehož hrana je „horní" nebo „levá".

---

[[HOME|← Zpět domů]]
