# 06 — 3D Pláty, Transformace, Homogenní souřadnice

tags: #téma6 #bikubika #transformace #homogennísouřadnice #subdivision #zkouška

---

## Parametrický popis 3D plochy

```
q(u, v) = [x(u,v), y(u,v), z(u,v)]
u, v ∈ [0, 1]
```
Dva parametry → 2D plocha v 3D prostoru.

### Normálový vektor plochy v bodě
- Kolmý na tečnou rovinu plochy v daném bodě
- Vypočítá se jako vektorový součin parciálních derivací:
```
n = ∂q/∂u × ∂q/∂v
```
- Vyjadřuje **orientaci povrchu** (směr "ven" z tělesa)
- Využití: osvětlování (Phong model), stínování

---

## Bézierův bikubický plát

- Tenzorový součin dvou kubických Bézierových křivek
- **Počet řídicích bodů:** 4 × 4 = **16 bodů**
- **Počet čísel:** 16 × 3 = **48 čísel** (3D souřadnice)

**→ MCQ odpověď: f) 16 (48)**

### Maticový tvar
```
q(u,v) = U · Mᴮ · G · Mᴮᵀ · Vᵀ
```
kde:
- U = [u³ u² u 1], V = [v³ v² v 1]
- Mᴮ = Bézierova bázová matice
- G = matice 16 řídicích bodů (4×4×3)

---

## Napojení plátů

**C¹-spojité napojení dvou plátů:**
- Sdílení **celé hrany** (4 body musí být shodné)
- Navíc: přilehlé řady řídicích bodů musí být kolineární s hranou (tečné vektory ve směru v musí odpovídat)

---

## Subdivision

> ⚠️ **Otázka z písemky:** "Jaká je hlavní myšlenka subdivision?"

**Hlavní myšlenka:**
Opakovaným dělením křivek/plátů vzniká posloupnost bodů, která **konverguje** ke hladkému povrchu.

**Klíčová vlastnost:**
Posloupnost bodů z opakovaného dělení tvoří **Cauchyovskou posloupnost** — konverguje. Výsledná křivka/plocha je limitou tohoto procesu.

### Subdivision křivky (de Casteljau)
- Dělením v t=1/2 vzniknou dva podúseky
- Oba mají 4 řídicí body odvozené z mezivýpočtů
- Opakováním → aproximace původní Bézierovy křivky rovnými úsečkami

### Catmull-Clark Subdivision (pro pláty)
- **Určen pro čtyřúhelníkové sítě**
- Každé dělení přidá nové vrcholy (face point, edge point, vertex point)
- Po jednom dělení jsou všechny plošky čtyřúhelníky
- Konverguje k hladkému B-spline povrchu

**→ MCQ: Pro jaké topologie je Catmull-Clark? c) čtyřúhelníky**

---

## Homogenní souřadnice

**Definice:** Bod (x, y, z) v 3D → homogenní souřadnice (X, Y, Z, W) kde:
```
x = X/W,  y = Y/W,  z = Z/W    (W ≠ 0)
```
Typicky W = 1 → (x, y, z, 1).

**Proč?**
- Umožňují vyjádřit **posunutí jako maticové násobení** (jinak by nešlo)
- Perspektivní projekce = jednoduché maticové násobení

---

## Geometrické transformace (4×4 matice)

### Posunutí (Translation)
```
T = | 1  0  0  tx |
    | 0  1  0  ty |
    | 0  0  1  tz |
    | 0  0  0  1  |
```

### Rotace kolem osy z (o úhel θ)
```
Rz = | cos θ  -sin θ  0  0 |
     | sin θ   cos θ  0  0 |
     | 0       0      1  0 |
     | 0       0      0  1 |
```

### Změna měřítka (Scale)
```
S = | sx  0   0   0 |
    | 0   sy  0   0 |
    | 0   0   sz  0 |
    | 0   0   0   1 |
```

### Středová souměrnost v rovině xy (zrcadlení osy z)
```
M = | 1  0   0   0 |
    | 0  1   0   0 |
    | 0  0  -1   0 |
    | 0  0   0   1 |
```

### Osová/středová souměrnost
- Středová souměrnost (bod) = S(-1, -1, -1)
- Souměrnost podle roviny xy: sz = -1

---

## Skládání transformací

- Matice násobíme: `M = M₁ · M₂ · M₃ · ...`
- **Záleží na pořadí!** Maticové násobení není komutativní obecně.
- **Výjimka:** rotace kolem stejné osy, stejné změny měřítka (tyto komutují).

**Příklad kdy záleží:** Rotace + Posunutí (jiný výsledek než Posunutí + Rotace)
**Příklad kdy nezáleží:** Rotace kolem osy z o 30° a pak o 45° = 75° (komutují rotace kolem stejné osy)

---

## Graf scény (Scene Graph)

- Hierarchická stromová struktura objektů scény
- Transformace se dědí: potomek = transformace rodiče × vlastní transformace
- Efektivní pro animace (pohyb robota = pohyb celé větve stromu)

---

## Kontrolní otázky

**Q: Kolik řídicích bodů má Bézierův bikubický plát? Kolik čísel?**
16 bodů, 16 × 3 = **48 čísel**.

**Q: Co vyjadřuje normálový vektor plochy?**
Směr kolmý na tečnou rovinu povrchu v daném bodě. Určuje orientaci povrchu, používá se pro výpočet osvětlení.

**Q: Co musí být splněno pro C¹ napojení dvou plátů?**
Sdílení hrany (4 bodů) a kolinearita přilehlých řad řídicích bodů s hranou.

**Q: Pro jaké topologie je Catmull-Clark?**
**Čtyřúhelníkové** sítě.

**Q: Záleží obecně na pořadí transformací?**
Ano, záleží — matice obecně nekomutují. Výjimka: rotace kolem stejné osy.

---

[[HOME|← Zpět domů]]
