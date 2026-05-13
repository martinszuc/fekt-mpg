# 06 — 3D Pláty, Transformace, Homogenní souřadnice

tags: #téma6 #bikubika #transformace #homogennísouřadnice #subdivision #zkouška

---

## Parametrický popis 3D plochy

```
q(u, v) = [x(u,v), y(u,v), z(u,v)]
u, v ∈ [0, 1]
```
Dva parametry → 2D plocha v 3D prostoru.

> **Intuice:** Představte si, že máte čtvercový kus gumy s souřadnicemi u a v (obě od 0 do 1). Funkce q(u,v) říká, kam se každý bod gumy „namapuje" v 3D prostoru. Tažením za různá místa gumy můžete vytvořit libovolný tvar povrchu.

### Normálový vektor plochy v bodě
- Kolmý na tečnou rovinu plochy v daném bodě
- Vypočítá se jako vektorový součin parciálních derivací:
```
n = ∂q/∂u × ∂q/∂v
```
- Vyjadřuje **orientaci povrchu** (směr "ven" z tělesa)
- Využití: osvětlování (Phong model), stínování

#### Příklad výpočtu vektorového součinu

Mějme dva vektory v tečné rovině:
```
∂q/∂u = (1, 0, 0)    — tečný vektor ve směru u
∂q/∂v = (0, 1, 0)    — tečný vektor ve směru v
```

Vektorový součin (cross product):
```
n = ∂q/∂u × ∂q/∂v

n_x = u_y · v_z − u_z · v_y = 0·0 − 0·1 = 0
n_y = u_z · v_x − u_x · v_z = 0·0 − 1·0 = 0
n_z = u_x · v_y − u_y · v_x = 1·1 − 0·0 = 1

n = (0, 0, 1)
```

Výsledek: normála míří ve směru osy z, tedy **povrch v tomto bodě leží v rovině xy**. To dává smysl — oba tečné vektory leží v rovině xy, normála je k ní kolmá.

> **Mnemotechnická pomůcka** pro vektorový součin — „cyklická permutace": x→y→z→x. Složka x se počítá z y a z, složka y z z a x, složka z z x a y.

---

## Bézierův bikubický plát

- Tenzorový součin dvou kubických Bézierových křivek
- **Počet řídicích bodů:** 4 × 4 = **16 bodů**
- **Počet čísel:** 16 × 3 = **48 čísel** (3D souřadnice)

**→ MCQ odpověď: f) 16 (48)**

### Intuice: tenzorový součin

Tenzorový součin si lze představit jako **„tažení jedné křivky podél druhé"**:

1. Vezměte kubickou Bézierovu křivku definovanou 4 body
2. Každý z těchto 4 bodů „roztáhněte" do další kubické křivky (4 body)
3. Vznikne mřížka 4×4 = 16 řídicích bodů

Matematicky: bod na povrchu se spočte jako:
```
q(u,v) = Σᵢ Σⱼ Pᵢⱼ · Bᵢ(u) · Bⱼ(v)
```
kde Bᵢ jsou Bernsteinovy polynomy. Parametr u „jede" jedním směrem, v druhým.

> **Prakticky:** Pokud zafixujete v = konst., dostanete Bézierovu křivku ve směru u. A naopak. Celá plocha je „rodina křivek" parametrizovaná druhým parametrem.

![[06-bezier-plat.svg]]

### Jak ovlivňuje řídicí bod tvar povrchu?

Každý ze 16 řídicích bodů ovlivňuje povrch **lokálně** — nejvíc v okolí svého parametrického místa:

- **Rohové body** (P₀₀, P₃₀, P₀₃, P₃₃): povrch jimi přímo prochází (interpolace)
- **Vnitřní body** (P₁₁, P₁₂, P₂₁, P₂₂): ovlivňují „bouli" uprostřed plochy
- **Hraniční body** (P₁₀, P₂₀, ...): ovlivňují tvar okrajových křivek

**Příklad:** Pokud posunete bod P₁₁ nahoru (ve směru z), plocha se v okolí rohu P₀₀ „vyboří" nahoru. Čím dále od P₁₁, tím menší vliv. Rohové body zůstanou na místě — posun vnitřního bodu nezmění pozici rohů.

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

#### Příklad Catmull-Clark: krychle

Krychle má 6 čtyřúhelníkových stěn, 12 hran a 8 vrcholů.

**Po jedné iteraci Catmull-Clark:**
1. **Face points:** Na každé stěně vznikne 1 nový bod (průměr 4 rohů) → 6 nových bodů
2. **Edge points:** Na každé hraně vznikne 1 nový bod (průměr konců hrany + sousedních face points) → 12 nových bodů
3. **Vertex points:** Původní vrcholy se přepočítají (vážený průměr s okolím) → 8 upravených bodů

Výsledek: **24 čtyřúhelníků** (každá původní stěna se rozdělí na 4), povrch se viditelně zaoblí.

Po 2–3 iteracích je krychle téměř k nerozeznání od koule (ale topologicky je stále polyedrální).

> **Proč to funguje:** Každá iterace „zprůměruje" ostré rohy s okolím, čímž se hrany zaoblují. Matematicky to konverguje k B-spline povrchu (C² spojitost všude kromě mimořádných vrcholů).

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

> **Intuice:** Bez homogenních souřadnic je posunutí (translace) **afinní** operace: p' = p + t. Nejde ji zapsat jako násobení maticí 3×3. Přidáním čtvrté souřadnice W=1 můžeme translaci „schovat" do matice 4×4, a tak všechny transformace (rotace, škálování, posunutí) unifikovat do jednoho násobení maticí.

---

## Geometrické transformace (4×4 matice)

### Posunutí (Translation)
```
T = | 1  0  0  tx |
    | 0  1  0  ty |
    | 0  0  1  tz |
    | 0  0  0  1  |
```

#### Příklad: Posunutí bodu (3, 2, 1) o vektor (5, 0, −1)

```
| 1  0  0  5 |   | 3 |   | 1·3 + 0·2 + 0·1 + 5·1 |   | 8 |
| 0  1  0  0 | · | 2 | = | 0·3 + 1·2 + 0·1 + 0·1 | = | 2 |
| 0  0  1 -1 |   | 1 |   | 0·3 + 0·2 + 1·1 + (-1)·1| = | 0 |
| 0  0  0  1 |   | 1 |   | 0·3 + 0·2 + 0·1 + 1·1 | = | 1 |
```

Výsledek: bod **(8, 2, 0)** — posunuli jsme o 5 v x, o 0 v y, o −1 v z.

### Rotace kolem osy z (o úhel θ)
```
Rz = | cos θ  -sin θ  0  0 |
     | sin θ   cos θ  0  0 |
     | 0       0      1  0 |
     | 0       0      0  1 |
```

#### Příklad: Rotace bodu (1, 0, 0) kolem osy z o 90°

cos 90° = 0, sin 90° = 1:
```
| 0  -1  0  0 |   | 1 |   | 0·1 + (-1)·0 + 0·0 + 0 |   | 0 |
| 1   0  0  0 | · | 0 | = | 1·1 +   0·0  + 0·0 + 0 | = | 1 |
| 0   0  1  0 |   | 0 |   | 0·1 +   0·0  + 1·0 + 0 | = | 0 |
| 0   0  0  1 |   | 1 |   | 0·1 +   0·0  + 0·0 + 1 | = | 1 |
```

Výsledek: bod **(0, 1, 0)** — bod na ose x se otočil na osu y, což odpovídá rotaci o 90° proti směru hodinových ručiček v rovině xy.

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

### Konkrétní příklad: pořadí transformací záleží!

Mějme bod P = (1, 0, 0) a dvě transformace:
- **R:** rotace kolem osy z o 90°
- **T:** posunutí o (5, 0, 0)

![[06-transformace.svg]]

#### Varianta 1: Nejdříve rotace, pak posunutí (T · R · P)

1. Rotace: (1,0,0) → (0,1,0)
2. Posunutí o (5,0,0): (0,1,0) → **(5, 1, 0)**

#### Varianta 2: Nejdříve posunutí, pak rotace (R · T · P)

1. Posunutí: (1,0,0) → (6,0,0)
2. Rotace o 90°: (6,0,0) → **(0, 6, 0)**

**Výsledky jsou různé:** (5,1,0) vs. (0,6,0)!

> **Proč?** Rotace se vždy provádí kolem počátku souřadnic. Pokud nejdříve posuneme bod daleko od počátku a pak rotujeme, bod opíše velký oblouk. Pokud nejdříve rotujeme a pak posuneme, posun proběhne ve „starém" směru.

> **Pravidlo čtení:** Matice se násobí zprava doleva: M = T · R znamená „nejdříve R, pak T". Transformace nejbližší bodu (vpravo) se aplikuje první.

---

## Graf scény (Scene Graph)

- Hierarchická stromová struktura objektů scény
- Transformace se dědí: potomek = transformace rodiče × vlastní transformace
- Efektivní pro animace (pohyb robota = pohyb celé větve stromu)

### Příklad: Robotická ruka

Představme si robotickou ruku s klouby:

```
Svět
 └─ Robot [T: pozice robota v místnosti]
     └─ Tělo
         └─ Rameno [R: rotace v ramenním kloubu]
             └─ Předloktí [R: rotace v lokti]
                 └─ Ruka [R: rotace v zápěstí]
```

![[06-scene-graph.svg]]

**Jak to funguje:**
- Každý uzel má svou **lokální transformaci** (rotace kloubu, pozice)
- **Globální transformace** uzlu = součin všech transformací od kořene:

```
M_ruka = T_robot · R_rameno · R_předloktí · R_zápěstí
```

**Výhoda:** Když otočíme rameno, automaticky se pohne předloktí i ruka (protože jsou potomci). Stačí změnit jednu matici R_rameno a celá větev stromu se přepočítá.

**Animace:** Pro simulaci mávání rukou stačí periodicky měnit úhel R_rameno. Všechny potomky (předloktí, ruka) se pohybují přirozeně s ním — stejně jako v reálném těle.

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

**Q: Co je tenzorový součin dvou křivek?**
Plocha vznikne „tažením" jedné křivky podél druhé. Pro Bézierův plát: bod q(u,v) se počítá jako součin Bernsteinových polynomů v u a v, vážený 4×4 řídicími body.

**Q: Proč používáme homogenní souřadnice?**
Aby bylo možné vyjádřit posunutí (translaci) jako násobení maticí 4×4. Bez nich by translace vyžadovala sčítání, ne násobení, a nedala by se skládat s rotacemi/škálováním do jedné matice.

**Q: Jak funguje graf scény u hierarchických objektů?**
Každý uzel má lokální transformaci. Globální transformace uzlu je součin transformací od kořene ke uzlu. Změna rodiče automaticky ovlivní všechny potomky.

---

[[HOME|← Zpět domů]]
