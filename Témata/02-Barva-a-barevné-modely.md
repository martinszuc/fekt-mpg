# 02 — Světlo, Barva a Barevné modely

tags: #téma2 #barva #RGB #zkouška

---

## Čípky a tyčinky

> ⚠️ **Opakovaná otázka z písemky:** "Proč ve tmě špatně rozeznáváme barvy?"

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

**→ MCQ odpověď na otázku "Jakých čípků je nejméně?": b) citlivé na modré odstíny**

---

## Barevné modely

### RGB (aditivní míchání)
- Červená + Zelená = **Žlutá (Y)**
- Červená + Modrá = **Purpurová (M)**
- Zelená + Modrá = **Azurová (C)**
- R + G + B = **Bílá**
- Absence všech = **Černá**

**→ MCQ: R + G = Žlutá (a)**

### CMY / CMYK (subtraktivní — tisk)
- Doplněk RGB: C = 1-R, M = 1-G, Y = 1-B
- K (Key/Black) přidán pro kvalitnější černou a úsporu inkoustu

### HSB (Hue, Saturation, Brightness)
- H = odstín (0–360°, úhel na barevném kruhu)
- S = sytost (0–100%)
- B = jas (0–100%)
- Intuitivnější pro designéry

### YUV / YCbCr
- Y = luma (jas), Cb/Cr = chrominance (barva)
- Používá se v TV, JPEG, video
- Umožňuje podvzorkování chrominance (oko méně citlivé na barvu než jas)

### Stupně šedi
Konverze z RGB: `Y = 0.299·R + 0.587·G + 0.114·B`

---

## Gama korekce
- Monitory mají nelineární vztah vstup→jas (mocninová funkce γ ≈ 2.2)
- Gama korekce linearizuje výstup
- Bez korekce: tmavé oblasti vypadají příliš tmavě

---

## Měření světla
- **Světelný tok** (lumen) — celková energie záření
- **Osvětlenost** (lux) — tok na plochu
- **Jas** (cd/m²) — co vnímáme

---

## Kontrolní otázky

**Q: Proč ve tmě špatně rozeznáváme barvy?**
Ve tmě pracují převážně tyčinky, které jsou citlivé jen na intenzitu světla, nikoli na vlnovou délku (barvu). Čípky (barevné receptory) vyžadují dostatek světla.

**Q: Jakých čípků je na lidské sítnici nejméně?**
Těch citlivých na **modré** odstíny (S-čípky).

**Q: Jaká barva vznikne smícháním R a G v RGB?**
**Žlutá (Y).**

---

[[HOME|← Zpět domů]]
