# 02 — Světlo, Barva a Barevné modely

tags: #téma2 #barva #RGB #zkouška

---

## Čípky a tyčinky

> **Opakovaná otázka z písemky:** "Proč ve tmě špatně rozeznáváme barvy?"

Lidská sítnice obsahuje dva druhy fotoreceptorů:

| | Čípky (cones) | Tyčinky (rods) |
|---|---|---|
| Počet | ~6 milionů | ~120 milionů |
| Citlivost | vysoké světlo | nízké světlo (skotopické vidění) |
| Vnímají barvy? | **ANO** | **NE** (pouze jas) |
| Typy | S (modrá), M (zelená), L (červená) | jeden typ |

**Odpověď:** Ve tmě (šeru) jsou aktivní převážně tyčinky, které nevnímají barvy — reagují pouze na intenzitu světla. Proto rozeznáváme jen odstíny šedi.

### Distribuce čípků
- **Nejméně** čípků citlivých na **modrou (S)**
- Nejvíce na zelenou (M) a červenou (L)

**MCQ odpověď na otázku "Jakých čípků je nejméně?": b) citlivé na modré odstíny**

---

## Barevné modely

### RGB (aditivní míchání)

![[02-aditivni-michani.svg]]

Aditivní míchání znamená, že **přidáváním světla** se barva zesvětluje. Začínáme od tmy (černé) a přidáváme barevné složky.

**Reálný příklad — jevištní reflektory:**
Představte si tmavé divadelní jeviště se třemi reflektory — červeným, zeleným a modrým:
- Zapnete jen **červený** reflektor → vidíte červené světlo
- Přidáte **zelený** reflektor na stejné místo → červená + zelená = **žluté** světlo na jevišti
- Přidáte ještě **modrý** → všechny tři dohromady = **bílé** světlo
- Pokud všechny zhasnete → **tma (černá)**

Stejný princip funguje v monitoru — každý pixel se skládá ze tří miniaturních LED/subpixelů (R, G, B), jejichž intenzita se sčítá.

**Kombinace:**
- Červená + Zelená = **Žlutá (Y)**
- Červená + Modrá = **Purpurová (M)**
- Zelená + Modrá = **Azurová (C)**
- R + G + B = **Bílá**
- Absence všech = **Černá**

**MCQ: R + G = Žlutá (a)**

![[rgb-cube.png]]

---

### CMY / CMYK (subtraktivní míchání — tisk)

![[02-subtraktivni-michani.svg]]

**Princip:** Inkousty/pigmenty **pohlcují** (odečítají) část bílého světla — to co se odrazí, vidíme jako barvu. Začínáme od bílého papíru (odráží všechno světlo) a přidáváním inkoustu světlo ubíráme.

**Reálný příklad — míchání vodových barev:**
- Bílý papír odráží všechno světlo → vidíme bílou
- Namalujete **cyan** (azurovou) barvou → pohlcuje červenou složku, odráží zelenou + modrou
- Přes cyan namalujete **žlutou** → žlutá pohlcuje modrou → zbývá jen zelená → vidíte **zelenou**
- Přidáte ještě **magentu** → ta pohlcuje zelenou → všechno pohlceno → teoreticky **černá**

| Inkoust | Pohlcuje | Odráží |
|---|---|---|
| Cyan (C) | červenou | zelenou + modrou |
| Magenta (M) | zelenou | červenou + modrou |
| Yellow (Y) | modrou | červenou + zelenou |

**Převod z RGB** (hodnoty normalizované 0–1):
- C = 1 − R
- M = 1 − G
- Y = 1 − B

> Příklad: čistá červená RGB(1, 0, 0) → C=0, M=1, Y=1 — tedy magenta + žlutá bez cyanu.

**Proč K (Key/Black)?**
- Teoreticky C + M + Y = černá, ale v praxi dává **špinavou hnědou** (inkousty nejsou ideální filtry)
- Černý inkoust je **levnější** než míchat tři barvy dohromady
- Výpočet K: `K = min(C, M, Y)`, poté se CMY redukují: `C' = (C − K) / (1 − K)`

**CMY = subtraktivní doplněk RGB. Každý CMY inkoust odečítá jednu RGB složku z bílého světla.**

---

### Podrobný příklad: Převod RGB(200, 100, 50) → CMY → CMYK

**Krok 1: Normalizace RGB do rozsahu 0–1**
- R = 200 / 255 = **0.784**
- G = 100 / 255 = **0.392**
- B = 50 / 255 = **0.196**

**Krok 2: Převod na CMY**
- C = 1 − R = 1 − 0.784 = **0.216**
- M = 1 − G = 1 − 0.392 = **0.608**
- Y = 1 − B = 1 − 0.196 = **0.804**

Kontrola: nízká hodnota C dává smysl — barva je do oranžova/červena, takže cyan (který pohlcuje červenou) je málo.

**Krok 3: Převod CMY → CMYK (vytažení černé složky)**
- K = min(C, M, Y) = min(0.216, 0.608, 0.804) = **0.216**
- C' = (C − K) / (1 − K) = (0.216 − 0.216) / (1 − 0.216) = **0.000**
- M' = (M − K) / (1 − K) = (0.608 − 0.216) / (1 − 0.216) = 0.392 / 0.784 = **0.500**
- Y' = (Y − K) / (1 − K) = (0.804 − 0.216) / (1 − 0.216) = 0.588 / 0.784 = **0.750**

**Výsledek:** CMYK = (0.000, 0.500, 0.750, 0.216) = **(0%, 50%, 75%, 22%)**

Interpretace: tiskárna nepoužije cyan vůbec, nanese 50% magenty, 75% žluté a 22% černé.

---

### HSB (Hue, Saturation, Brightness)

![[02-hsb-valec.svg]]

HSB je **intuitivní** barevný model — popisuje barvu tak, jak o ní přemýšlíme v běžné řeči:

- **H (Hue / Odstín)** = "jaká barva?" — úhel na barevném kruhu (0–360 stupňů)
  - 0° = červená
  - 60° = žlutá
  - 120° = zelená
  - 180° = cyan
  - 240° = modrá
  - 300° = magenta
  - 360° = opět červená (kruh)

- **S (Saturation / Sytost)** = "jak moc sytá?" — 0% = šedá, 100% = plná barva
  - Představte si, že přidáváte bílou barvu do červené: S=100% je čistě červená, S=50% je růžová, S=0% je šedá.

- **B (Brightness / Jas)** = "jak moc světlá?" — 0% = černá, 100% = plný jas
  - Jako byste ztlumili stmívač — barva zůstane stejná, jen je tmavší.

**Praktický příklad — jak najít pastelově modrou:**
1. Nastavíte H = 210° (modrá)
2. Snížíte S na 40% (méně sytá → pastelová)
3. Zvýšíte B na 90% (světlá)
→ Výsledek: jemná pastelově modrá.

**Proč je HSB užitečný?**
- V RGB je těžké "ztmavit" barvu — musíte měnit všechny tři složky proporcionálně
- V HSB stačí snížit B (jas) a barva se ztmaví přirozeně
- Designéři používají HSB pro tvorbu barevných palet: stačí měnit H a mít stejnou S a B

---

### YUV / YCbCr

- **Y** = luma (jas) — kolik světla bod vyzařuje
- **Cb** = chrominance modrá (odchylka od šedé směrem k modré/žluté)
- **Cr** = chrominance červená (odchylka od šedé směrem k červené/cyan)

Používá se v **TV, JPEG, MPEG video**.

**Proč je podvzorkování chrominance (chroma subsampling) možné?**

Lidské oko má **mnohem více tyčinek** (vnímají jas) než čípků (vnímají barvu). Navíc čípky jsou soustředěny v malé oblasti (fovea), zatímco tyčinky pokrývají široké zorné pole. Důsledek:

- Oko je **velmi citlivé na změny jasu** (detekuje hrany, kontury, textury)
- Oko je **méně citlivé na změny barvy** — barevný detail můžeme "rozmazat" a nikdo si toho nevšimne

Proto formáty jako JPEG a video kodeky ukládají jasovou složku (Y) v plném rozlišení, ale barevné složky (Cb, Cr) v **polovičním nebo čtvrtinovém** rozlišení:

| Schéma | Význam | Úspora |
|---|---|---|
| 4:4:4 | plné rozlišení barvy i jasu | žádná (referenční) |
| 4:2:2 | barva má poloviční horizontální rozlišení | ~33% dat |
| 4:2:0 | barva má poloviční rozlišení v obou osách | ~50% dat |

Příklad: u videa 1920x1080 ve schématu 4:2:0 má Y složka 1920x1080, ale Cb a Cr mají jen 960x540.

---

### Stupně šedi
Konverze z RGB: `Y = 0.299*R + 0.587*G + 0.114*B`

Koeficienty nejsou stejné — zelená má největší váhu, protože lidské oko je na zelenou **nejcitlivější** (nejvíc čípků typu M). Modrá má nejmenší váhu (nejméně S-čípků).

---

## Gama korekce

![[02-gamma-krivka.svg]]

Monitory mají **nelineární vztah** mezi vstupním signálem a skutečným jasem — řídí se mocninovou funkcí:

```
jas_výstup = signál^γ
```

kde typicky **gamma = 2.2** pro běžné monitory.

### Proč to je problém?

Bez korekce: pokud pošlete na monitor signál 0.5 (50% vstup), očekáváte 50% jasu. Ale monitor zobrazí:

```
jas = 0.5^2.2 = 0.5^2.2 ≈ 0.218
```

To je jen **21.8% jasu** — obrázek vypadá **výrazně tmavší**, než by měl. Tmavé oblasti jsou neúměrně tmavé, barevné přechody vypadají nerovnoměrně.

### Jak gama korekce funguje?

Před odesláním na monitor se signál **předkoriguje** inverzní funkcí:

```
signál_korigovaný = signál^(1/γ) = signál^(1/2.2) ≈ signál^0.4545
```

Monitor pak aplikuje svoji gamma:

```
výsledný_jas = (signál^(1/γ))^γ = signál^1 = signál
```

Korekce se navzájem vyruší a výsledek je **lineární** — 50% vstup = 50% jas.

### Konkrétní příklad s čísly

| Vstupní hodnota | Bez korekce (jas = x^2.2) | S korekcí (jas = x) |
|---|---|---|
| 0.00 | 0.000 (černá) | 0.000 |
| 0.25 | 0.047 (téměř černá!) | 0.250 |
| 0.50 | 0.218 (moc tmavé) | 0.500 |
| 0.75 | 0.530 | 0.750 |
| 1.00 | 1.000 (bílá) | 1.000 |

Bez korekce je polovina škály (0–0.5) zmačkána do pouhých 22% jasu — proto tmavé scény ve hrách bez správné gamma korekce vypadají jako nerozlišitelná tma.

---

## Měření světla
- **Světelný tok** (lumen) — celková energie záření
- **Osvětlenost** (lux) — tok na plochu
- **Jas** (cd/m2) — co vnímáme

---

## Kontrolní otázky

**Q: Proč ve tmě špatně rozeznáváme barvy?**
Ve tmě pracují převážně tyčinky, které jsou citlivé jen na intenzitu světla, nikoli na vlnovou délku (barvu). Čípky (barevné receptory) vyžadují dostatek světla.

**Q: Jakých čípků je na lidské sítnici nejméně?**
Těch citlivých na **modré** odstíny (S-čípky).

**Q: Jaká barva vznikne smícháním R a G v RGB?**
**Žlutá (Y).**

**Q: Převeďte RGB(200, 100, 50) na CMYK.**
R=0.784, G=0.392, B=0.196 → CMY=(0.216, 0.608, 0.804) → K=0.216, CMYK=(0%, 50%, 75%, 22%).

**Q: Proč je možné podvzorkovat chrominanci a ne luminanci?**
Lidské oko je mnohem citlivější na změny jasu (jas vnímají jak tyčinky, tak čípky) než na změny barvy (jen čípky). Proto lze barevnou informaci uložit v nižším rozlišení bez viditelné ztráty kvality.

**Q: Pokud je gamma monitoru 2.2 a vstupní signál je 0.5, jaký je skutečný jas?**
jas = 0.5^2.2 = 0.218, tedy jen asi 22% maximálního jasu místo očekávaných 50%.

**Q: Jaký je rozdíl mezi aditivním a subtraktivním mícháním barev?**
Aditivní (RGB): přidáváním světla se zesvětluje, mícháním všech vzniká bílá. Používá se v monitorech.
Subtraktivní (CMY): přidáváním pigmentu se ztmavuje, mícháním všech vzniká (teoreticky) černá. Používá se v tisku.

---

[[HOME|← Zpět domů]]
