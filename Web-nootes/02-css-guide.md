# Часть 2 — CSS: одеваем скелет

> **Учебный проект**: продолжаем [anime-catalog/](./project/anime-catalog/) — берём HTML из части 1 и стилизуем  
> **Вопросы для самопроверки**: [questions/02-css-questions.md](./questions/02-css-questions.md)

---

## Содержание

1. [Как браузер применяет стили: каскад и специфичность](#1-как-браузер-применяет-стили-каскад-и-специфичность)
2. [Box model — коробка внутри коробки](#2-box-model--коробка-внутри-коробки)
3. [Flexbox — раскладка по одной оси](#3-flexbox--раскладка-по-одной-оси)
4. [CSS Grid — двумерная сетка](#4-css-grid--двумерная-сетка)
5. [Позиционирование и z-index](#5-позиционирование-и-z-index)
6. [Переменные (Custom Properties)](#6-переменные-custom-properties)
7. [Адаптивность: media queries и container queries](#7-адаптивность-media-queries-и-container-queries)
8. [Анимации и переходы](#8-анимации-и-переходы)
9. [Методологии и парадигмы CSS-библиотек](#9-методологии-и-парадигмы-css-библиотек)
10. [Современный CSS: @layer, :has(), @container](#10-современный-css-layer-has-container)
11. [DevTools: вкладки Styles и Computed](#11-devtools-вкладки-styles-и-computed)
12. [Что CSS-разработчику делать не нужно](#12-что-css-разработчику-делать-не-нужно)
13. [Проект: стилизация аниме-каталога](#13-проект-стилизация-аниме-каталога)

---

## 1. Как браузер применяет стили: каскад и специфичность

### Три источника стилей

Прежде чем писать первый `.class { }` — нужно понять, откуда вообще берутся стили. У каждого элемента на странице их может быть много, и браузер должен решить, какой применить.

```
┌────────────────────────────────────────────────────────────────┐
│                 ИСТОЧНИКИ СТИЛЕЙ                               │
│                                                                │
│  1. Браузерные стили       (User Agent Stylesheet)             │
│     h1 { font-size: 2em; margin: 0.67em 0; }  ← дефолт Chrome │
│     a  { color: blue; text-decoration: underline; }            │
│                                                                │
│  2. Ваши стили             (Author Stylesheet)                 │
│     Всё что вы пишете в .css файлах                            │
│                                                                │
│  3. Пользовательские       (User Stylesheet)                   │
│     Пользователь задал свои стили в браузере                   │
│     (редко, но важно для accessibility)                        │
└────────────────────────────────────────────────────────────────┘
```

Поэтому в каждый проект добавляют CSS reset или normalize — небольшой файл, который обнуляет или выравнивает браузерные дефолты между Chrome, Firefox, Safari.

### Специфичность — вес селектора

Когда два правила претендуют на одно свойство — побеждает то, у которого **выше специфичность**. Считается тройкой `(A, B, C)`:

```
┌─────────────────────────────────────────────────────────────────┐
│  ТАБЛИЦА СПЕЦИФИЧНОСТИ                                          │
│                                                                 │
│  Тип               Пример               Вес                    │
│  ──────────────    ─────────────────    ──────────────          │
│  Универсальный     *                    (0, 0, 0)               │
│  Тег               p, h1, div           (0, 0, 1)               │
│  Класс             .card, .btn          (0, 1, 0)               │
│  Атрибут           [type="email"]       (0, 1, 0)               │
│  Псевдокласс       :hover, :first-child (0, 1, 0)               │
│  ID                #header              (1, 0, 0)               │
│  Inline style      style="color:red"    (1, 0, 0, 0) — выше всех│
│  !important        перебивает всё       — ядерное оружие        │
└─────────────────────────────────────────────────────────────────┘

Примеры:
  p                 → (0,0,1)
  .card             → (0,1,0)    ← побеждает p
  p.card            → (0,1,1)    ← побеждает .card
  #header .card     → (1,1,0)    ← побеждает p.card
```

Сравниваются слева направо: сначала A, потом B, потом C. `(1,0,0)` всегда побеждает `(0,99,99)`.

При **равной** специфичности побеждает правило, объявленное **позже** в коде.

### Почему id для стилей — плохая идея

`#header { color: red; }` имеет специфичность `(1,0,0)`. Перебить это можно только ещё одним id или `!important`. Используйте id для JS и якорей, для стилей — только классы.

### `!important` — симптом, не решение

```css
/* ❌ Гонка вооружений */
.btn { color: blue !important; }
.card .btn { color: red !important; }
#main .card .btn { color: green !important; }

/* ✅ Правильно: плоская специфичность, понятный порядок */
.btn { color: blue; }
.btn-danger { color: red; }   /* модификатор перебивает базовый */
```

Единственные законные случаи `!important`: utility-классы вроде `.visually-hidden` или принудительное переопределение сторонней библиотеки.

### Каскад и порядок подключения файлов

```html
<!-- Если два файла определяют одно свойство с одинаковой специфичностью -->
<link rel="stylesheet" href="base.css">      <!-- 1. подключён первым -->
<link rel="stylesheet" href="components.css"> <!-- 2. подключён вторым — побеждает -->
```

Поэтому порядок имеет значение. Стандартный:

```
1. reset.css      — обнуляем браузерные стили
2. variables.css  — CSS-переменные
3. base.css       — типографика, базовые теги
4. layout.css     — сетки страницы
5. components.css — карточки, кнопки, формы
6. pages/home.css — стили конкретной страницы
```

---

## 2. Box model — коробка внутри коробки

### Анатомия элемента

Каждый HTML-элемент в браузере — это прямоугольная коробка из четырёх слоёв:

```
┌─────────────────────────────────────────────┐
│                  margin                     │ ← прозрачный внешний отступ
│  ┌───────────────────────────────────────┐  │
│  │               border                 │  │ ← рамка
│  │  ┌─────────────────────────────────┐ │  │
│  │  │             padding             │ │  │ ← внутренний отступ,
│  │  │  ┌───────────────────────────┐  │ │  │   закрашен фоном
│  │  │  │         content           │  │ │  │ ← текст, картинка
│  │  │  └───────────────────────────┘  │ │  │
│  │  └─────────────────────────────────┘ │  │
│  └───────────────────────────────────────┘  │
└─────────────────────────────────────────────┘
```

**Ключевой вопрос**: если задать `width: 300px` — это ширина чего именно?

По умолчанию (`box-sizing: content-box`) — это ширина только **content**. К ней добавляются padding и border, и реальная ширина получается больше.

```css
/* content-box (по умолчанию, не используйте): */
.card { width: 300px; padding: 20px; border: 2px solid; }
/* Реальная ширина = 300 + 20*2 + 2*2 = 344px */

/* border-box (всегда используйте): */
*, *::before, *::after { box-sizing: border-box; }
.card { width: 300px; padding: 20px; border: 2px solid; }
/* Реальная ширина = 300px. Content уменьшается до 256px. */
```

**Это первая строка любого CSS-проекта** — `box-sizing: border-box` для всех элементов. Можно увидеть в каждом сброс-файле.

### Margin collapsing — самая неочевидная штука

Вертикальные margin между соседними блоками **схлопываются** — берётся только **больший**:

```html
<p style="margin-bottom: 30px">Абзац первый</p>
<p style="margin-top: 20px">Абзац второй</p>
<!-- Расстояние между ними = 30px, а не 50px -->
```

Это бывает удобно (текстовые абзацы не слипаются), но часто озадачивает. Схлопывания **не происходит** внутри flex и grid контейнеров — ещё одна причина их использовать.

### Margin vs padding — правило

- `padding` — пространство **внутри** компонента (между рамкой и содержимым)
- `margin` — пространство **между** компонентами

```css
/* Карточка: padding внутри, margin между карточками */
.card {
  padding: 24px;        /* воздух внутри карточки */
  margin-bottom: 16px;  /* расстояние до следующей карточки */
}
/* Но лучше gap вместо margin — см. раздел Flexbox/Grid */
```

### `display` — как элемент занимает место

```
block       → вся строка, начинается с новой (div, p, h1)
inline      → по размеру содержимого, в потоке текста (span, a)
             ← нельзя задать width/height ←
inline-block → в потоке текста, но можно задать размеры
flex        → flex-контейнер
grid        → grid-контейнер
none        → убрать из потока полностью (и из accessibility!)
contents    → сам исчезает, дети остаются
```

---

## 3. Flexbox — раскладка по одной оси

### Ментальная модель

Flexbox всегда работает по **одной оси**. Главная ось задаётся `flex-direction`:

```
flex-direction: row (default):
  Главная ось: →  (justify-content управляет)
  Поперечная:  ↓  (align-items управляет)

flex-direction: column:
  Главная ось: ↓  (justify-content управляет)
  Поперечная:  →  (align-items управляет)
```

Это одна из главных точек путаницы: **при `column` роли осей меняются**.

### Свойства контейнера

```css
.container {
  display: flex;

  /* Ось и перенос */
  flex-direction: row;          /* row | column | row-reverse | column-reverse */
  flex-wrap: nowrap;            /* nowrap | wrap — разрешить перенос строк */

  /* Распределение по ГЛАВНОЙ оси */
  justify-content: flex-start;
  /* flex-start | flex-end | center
     space-between — промежутки между элементами
     space-around  — промежутки вокруг каждого
     space-evenly  — все промежутки равны, включая края */

  /* Выравнивание по ПОПЕРЕЧНОЙ оси */
  align-items: stretch;
  /* stretch (default) — растянуть на всю высоту контейнера
     flex-start | flex-end | center
     baseline  — по базовой линии текста */

  /* Расстояния между элементами */
  gap: 16px;             /* между всеми
  gap: 16px 8px;         /* row-gap column-gap */
}
```

### Свойства элементов

```css
.item {
  /* Как расти/сжиматься */
  flex-grow: 0;       /* как делить СВОБОДНОЕ место (0 = не расти) */
  flex-shrink: 1;     /* как сжиматься при НЕХВАТКЕ (0 = не сжиматься) */
  flex-basis: auto;   /* начальный размер до расчёта */

  /* Шорткат: grow shrink basis */
  flex: 1;            /* = 1 1 0 → занять равную долю */
  flex: none;         /* = 0 0 auto → не менять размер */
  flex: 0 0 200px;    /* фиксированные 200px */

  /* Переопределить align-items для одного */
  align-self: center;

  /* Визуальный порядок (DOM не меняется!) */
  order: 0;
}
```

### Рецепты

```css
/* 1. Центрировать по обеим осям */
.center {
  display: flex;
  justify-content: center;
  align-items: center;
}

/* 2. Шапка: лого слева, навигация справа */
.header {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

/* 3. Sticky footer — main занимает весь остаток */
body {
  min-height: 100dvh;
  display: flex;
  flex-direction: column;
}
main { flex: 1; }

/* 4. Карточки с переносом, равной ширины */
.cards {
  display: flex;
  flex-wrap: wrap;
  gap: 16px;
}
.card { flex: 1 1 280px; }  /* минимум 280px, потом растёт */

/* 5. Кнопка прижата к правому краю */
.toolbar {
  display: flex;
  gap: 8px;
}
.toolbar .spacer { flex: 1; }  /* распорка */
```

### Главная ловушка: `min-width: auto`

По умолчанию flex-элемент **не сжимается меньше своего содержимого**. Текст или картинка вылезут за пределы:

```css
/* ❌ Текст вылезает, text-overflow не работает */
.card { display: flex; }
.card__title { overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }

/* ✅ Добавить min-width: 0 */
.card { display: flex; }
.card__title {
  min-width: 0;          /* ← вот это */
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}
```

---

## 4. CSS Grid — двумерная сетка

### Flex vs Grid — когда что

```
┌─────────────────────────────────────────────────────────────────┐
│  Задача                              │ Flexbox  │ Grid          │
│  ────────────────────────────────────┼──────────┼───────────    │
│  Меню в линию                        │    ✓     │               │
│  Карточки с переносом                │    ✓     │    ✓          │
│  Центрировать элемент                │    ✓     │    ✓          │
│  Дашборд (sidebar + main + footer)   │          │    ✓          │
│  Галерея с выравниванием по сетке    │          │    ✓          │
│  Форма: лейблы и поля в колонках     │          │    ✓          │
│  Карточка с кнопками всегда внизу    │          │    ✓          │
└─────────────────────────────────────────────────────────────────┘
Grid и Flex не конкуренты — часто Grid снаружи, Flex внутри ячейки.
```

### Базовый синтаксис

```css
.container {
  display: grid;
  grid-template-columns: 200px 1fr 1fr;  /* 3 колонки */
  grid-template-rows: auto 1fr auto;     /* 3 строки */
  gap: 16px;
}
```

**Единица `fr`** = fraction = «доля свободного места после фиксированных элементов»:

```css
grid-template-columns: 200px 1fr;        /* сайдбар фикс + остаток */
grid-template-columns: 1fr 2fr 1fr;      /* 25% | 50% | 25% */
grid-template-columns: repeat(4, 1fr);  /* 4 равные колонки */
```

### `minmax()` и `auto-fit` — адаптивность без media queries

```css
/* Сетка адаптируется сама: на широком — 4 колонки, на мобиле — 1 */
grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
```

Что происходит: «вмести столько колонок по 250px+, сколько влезет, остаток раздели поровну». Ни одной media query не нужно.

`auto-fit` vs `auto-fill`:
- `auto-fit` — растягивает существующие колонки на свободное место
- `auto-fill` — оставляет «пустые» невидимые колонки

В 99% случаев нужен `auto-fit`.

### Именованные области — самый читаемый layout

```css
.app {
  display: grid;
  grid-template-areas:
    "header  header  header"
    "sidebar main    aside"
    "footer  footer  footer";
  grid-template-columns: 220px 1fr 200px;
  grid-template-rows: auto 1fr auto;
  min-height: 100dvh;
}

/* Привязать элементы к зонам */
header  { grid-area: header; }
.sidebar { grid-area: sidebar; }
main    { grid-area: main; }
aside   { grid-area: aside; }
footer  { grid-area: footer; }
```

Поменять layout на мобиле — просто переопределить `grid-template-areas` в media query:

```css
@media (max-width: 768px) {
  .app {
    grid-template-areas:
      "header"
      "main"
      "aside"
      "footer";
    grid-template-columns: 1fr;
  }
}
```

### Размещение по линиям

Grid считает **линии**, а не ячейки. 3 колонки = 4 вертикальные линии (1, 2, 3, 4).

```css
.hero {
  grid-column: 1 / -1;    /* от первой до последней линии = вся ширина */
  grid-row: 2 / 4;        /* строки 2 и 3 */
}

.wide {
  grid-column: 1 / span 2;  /* начать с линии 1, занять 2 колонки */
}
```

### Рецепты

```css
/* 1. Карточки с кнопками всегда внизу */
.card {
  display: grid;
  grid-template-rows: auto 1fr auto;
  /* шапка — растягивается — кнопки */
}

/* 2. Форма с выровненными лейблами */
.form {
  display: grid;
  grid-template-columns: max-content 1fr;
  gap: 8px 16px;
  align-items: center;
}

/* 3. Центрировать одной строкой */
.parent {
  display: grid;
  place-items: center;  /* = align-items + justify-items */
}
```

### Ловушки

```css
/* ❌ fr не делит пространство если есть переполнение */
grid-template-columns: 1fr 1fr;
/* Если содержимое шире чем 50% — колонки будут неравными */

/* ✅ minmax(0, 1fr) — fr от нулевого минимума */
grid-template-columns: minmax(0, 1fr) minmax(0, 1fr);
```

---

## 5. Позиционирование и z-index

### Пять значений `position`

```
static   (default) — в обычном потоке. top/left/z-index игнорируются.

relative — в потоке, но можно сдвинуть через top/left/...
           Главный смысл: стать якорем для absolute-детей.

absolute — выпадает из потока. Позиционируется относительно
           ближайшего предка с position ≠ static.
           Если такого нет — относительно <html>.

fixed    — выпадает из потока. Позиционируется относительно
           viewport (окна браузера). Не скроллится.

sticky   — relative пока не достигнет top/left → тогда fixed.
           «Прилипает» при скролле.
```

### Типичные сценарии

```css
/* Тултип/бейдж поверх карточки */
.card          { position: relative; }    /* якорь */
.card__badge   {
  position: absolute;
  top: 8px; right: 8px;                   /* угол карточки */
}

/* Модалка поверх всего */
.modal-overlay {
  position: fixed;
  inset: 0;           /* = top:0; right:0; bottom:0; left:0 */
  background: rgba(0, 0, 0, 0.5);
  display: grid;
  place-items: center;
}

/* Sticky хедер */
header {
  position: sticky;
  top: 0;
  z-index: 100;
}
```

### Stacking context — почему z-index «не работает»

Это причина 90% багов с z-index. Главное правило:

> **`z-index` сравнивается только внутри одного stacking context. Между разными контекстами z-index не конкурирует.**

Что создаёт новый stacking context (элемент начинает «свою вселенную» z-index):

```css
position: relative/absolute/fixed/sticky + z-index ≠ auto
opacity: < 1
transform: любое значение, кроме none
filter: любое значение
will-change: transform (и другие)
isolation: isolate   ← современный, чистый способ
```

**Классический баг:**

```css
.modal   { position: fixed; z-index: 9999; }

/* где-то в коде */
.animated-hero { transform: scale(1.05); }

/* .modal внутри .animated-hero будет под другими элементами,
   потому что transform создал новый stacking context,
   и z-index: 9999 работает только внутри него */
```

**Решение** — не запускать z-index гонку, а использовать систему слоёв через CSS-переменные:

```css
:root {
  --z-base:    0;
  --z-raised:  10;
  --z-dropdown: 100;
  --z-sticky:  200;
  --z-modal:   1000;
  --z-toast:   1100;
}
```

### `transform` для анимаций, а не `top/left`

```css
/* ❌ Перемещение через left — вызывает reflow, дёргается */
.box { transition: left 300ms; }
.box:hover { left: 100px; }

/* ✅ Через transform — только GPU compositing, 60fps */
.box { transition: transform 300ms; }
.box:hover { transform: translateX(100px); }
```

---

## 6. Переменные (Custom Properties)

### Зачем нужны

Представьте: вы используете синий цвет `#3b82f6` в 40 местах. Дизайнер говорит «сделай чуть темнее». Без переменных — 40 правок. С переменными — одна строка.

### Синтаксис

```css
/* Объявление — всегда начинаются с -- */
:root {
  --color-primary: #3b82f6;
  --color-text: #111827;
  --space-md: 1rem;
  --radius: 8px;
}

/* Использование */
.btn {
  background: var(--color-primary);
  padding: var(--space-md);
  border-radius: var(--radius);
}

/* Fallback — значение по умолчанию если переменная не определена */
.btn { color: var(--color-text, #333); }
```

### Наследование и каскад

CSS-переменные **наследуются** как обычные свойства. Объявите в `:root` — доступны везде. Переопределите в компоненте — действует внутри него:

```css
:root {
  --card-bg: white;
  --card-text: #111;
}

.dark-theme {
  --card-bg: #1f2937;
  --card-text: #f9fafb;
}

.card {
  background: var(--card-bg);   /* в .dark-theme будет тёмный */
  color: var(--card-text);
}
```

### Дизайн-токены — система в одном файле

```css
:root {
  /* ─── Цвета ──────────────────────────────── */
  --color-bg:        #ffffff;
  --color-surface:   #f9fafb;
  --color-border:    #e5e7eb;
  --color-text:      #111827;
  --color-text-muted:#6b7280;
  --color-primary:   #3b82f6;
  --color-primary-hover: #2563eb;
  --color-danger:    #ef4444;
  --color-success:   #22c55e;

  /* ─── Типографика ────────────────────────── */
  --font-sans:   'Inter', system-ui, sans-serif;
  --font-mono:   'JetBrains Mono', monospace;
  --text-sm:     0.875rem;
  --text-base:   1rem;
  --text-lg:     1.125rem;
  --text-xl:     1.25rem;
  --text-2xl:    1.5rem;
  --text-4xl:    2.25rem;

  /* ─── Отступы (шкала ×4) ─────────────────── */
  --space-1:  0.25rem;   /*  4px */
  --space-2:  0.5rem;    /*  8px */
  --space-3:  0.75rem;   /* 12px */
  --space-4:  1rem;      /* 16px */
  --space-6:  1.5rem;    /* 24px */
  --space-8:  2rem;      /* 32px */
  --space-12: 3rem;      /* 48px */
  --space-16: 4rem;      /* 64px */

  /* ─── Радиусы ────────────────────────────── */
  --radius-sm:  4px;
  --radius-md:  8px;
  --radius-lg:  12px;
  --radius-xl:  16px;
  --radius-full: 9999px;

  /* ─── Тени ───────────────────────────────── */
  --shadow-sm:  0 1px 2px rgba(0,0,0,0.05);
  --shadow-md:  0 4px 6px rgba(0,0,0,0.07);
  --shadow-lg:  0 10px 15px rgba(0,0,0,0.10);

  /* ─── Z-index ────────────────────────────── */
  --z-dropdown: 100;
  --z-sticky:   200;
  --z-modal:    1000;
}
```

### Тёмная тема

```css
:root { --color-bg: #fff; --color-text: #111; }

@media (prefers-color-scheme: dark) {
  :root { --color-bg: #111; --color-text: #eee; }
}

/* Ручной переключатель */
[data-theme="dark"] { --color-bg: #111; --color-text: #eee; }
```

### Из JavaScript

```js
// Читать
const primary = getComputedStyle(document.documentElement)
  .getPropertyValue('--color-primary').trim();

// Записать (меняет на всём сайте)
document.documentElement.style.setProperty('--color-primary', '#10b981');
```

---

## 7. Адаптивность: media queries и container queries

### Mobile-first — почему именно так

**Базовые стили — для мобилы.** Media queries добавляют изменения для больших экранов:

```css
/* ✅ Mobile-first */
.card { padding: 12px; }
@media (min-width: 768px) { .card { padding: 24px; } }

/* ❌ Desktop-first — работает, но хуже */
.card { padding: 24px; }
@media (max-width: 767px) { .card { padding: 12px; } }
```

Причина: на мобиле «меньше всего» — меньше колонок, меньше padding, проще. Проще начать с минимума и добавлять.

### Стандартные брейкпоинты

```css
/* Мобила — базовые стили, без запроса */

@media (min-width: 640px)  { /* sm: большой телефон, маленький планшет */ }
@media (min-width: 768px)  { /* md: планшет */ }
@media (min-width: 1024px) { /* lg: десктоп */ }
@media (min-width: 1280px) { /* xl: широкий десктоп */ }
```

> Брейкпоинты должны ставиться **там, где дизайн ломается**, а не под конкретные устройства.

### Адаптивность без media queries

В современном CSS многое адаптируется само:

```css
/* Сетка без единой media query */
.catalog {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
  gap: 20px;
}

/* Плавная типографика */
h1 { font-size: clamp(1.5rem, 1rem + 3vw, 3rem); }
/*                     min     preferred    max  */

/* Flex с переносом */
.tags { display: flex; flex-wrap: wrap; gap: 8px; }
```

### Container queries — революция для компонентов

Media queries реагируют на **размер окна**. Container queries — на **размер контейнера**, в котором живёт компонент.

```css
/* Объявить контейнер */
.card-wrapper {
  container-type: inline-size;
  container-name: card;  /* необязательно */
}

/* Стили для карточки в зависимости от её контейнера */
@container card (min-width: 400px) {
  .card {
    display: flex;         /* широкий контейнер → горизонтальный layout */
    gap: 16px;
  }
  .card__cover {
    width: 120px;
    flex-shrink: 0;
  }
}
```

**Почему это важно:** одна и та же карточка в сайдбаре (200px) будет вертикальной, а на главной странице (600px) — горизонтальной. Без media queries и без дублирования кода.

Поддержка: все современные браузеры с конца 2022.

### Адаптивные картинки

```css
/* Картинка не вылезает за родителя */
img {
  max-width: 100%;
  height: auto;
  display: block;
}
```

```html
<!-- Разные размеры для разных экранов -->
<img
  src="/img/cover-800.jpg"
  srcset="/img/cover-400.jpg 400w,
          /img/cover-800.jpg 800w,
          /img/cover-1600.jpg 1600w"
  sizes="(max-width: 640px) 100vw,
         (max-width: 1024px) 50vw,
         300px"
  alt="...">
```

---

## 8. Анимации и переходы

### `transition` — плавное изменение по событию

```css
.btn {
  background: var(--color-primary);
  transform: translateY(0);
  transition: background 200ms ease, transform 200ms ease;
}

.btn:hover {
  background: var(--color-primary-hover);
  transform: translateY(-2px);
}
```

Синтаксис: `transition: <свойство> <длительность> <функция> <задержка>`

> **Не пишите `transition: all`** — это анимирует всё включая то, что меняется неожиданно. Перечисляйте конкретные свойства.

### `animation` — последовательность кадров

```css
@keyframes fadeSlideIn {
  from {
    opacity: 0;
    transform: translateY(16px);
  }
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

.card {
  animation: fadeSlideIn 300ms ease-out;
}

/* Поочерёдное появление карточек */
.card:nth-child(1) { animation-delay: 0ms; }
.card:nth-child(2) { animation-delay: 100ms; }
.card:nth-child(3) { animation-delay: 200ms; }
```

`animation-fill-mode: forwards` — остаётся в последнем кадре после окончания.

### Производительность: только transform и opacity

Браузер рисует страницу в три фазы:

```
Layout  → Paint  → Composite (GPU)
тяжело    средне   быстро
```

Анимируйте только то, что работает на уровне **Composite**:
- `transform` — translate, scale, rotate, skew
- `opacity`

Всё остальное (`width`, `height`, `left`, `top`, `margin`, `padding`, `background-color`) вызывает Layout или Paint и **будет дёргаться** на слабых устройствах.

```css
/* ❌ Дёргается: left вызывает reflow */
@keyframes slide { from { left: -100%; } to { left: 0; } }

/* ✅ 60fps: translateX только compositing */
@keyframes slide { from { transform: translateX(-100%); } to { transform: translateX(0); } }
```

### Timing functions

```
ease         — быстрый старт, мягкий конец (default)
ease-out     — медленный конец (самый природный для появления)
ease-in      — медленный старт (для исчезновения)
linear       — равномерно (спиннеры, прогресс)
cubic-bezier — своя кривая (cubic-bezier.com)
```

### Уважение к настройкам пользователя

**Обязательно** для сайтов с анимациями:

```css
@media (prefers-reduced-motion: reduce) {
  *, *::before, *::after {
    animation-duration: 0.01ms !important;
    transition-duration: 0.01ms !important;
    animation-iteration-count: 1 !important;
  }
}
```

---

## 9. Методологии и парадигмы CSS-библиотек

### Проблема, которую все решают

CSS глобальный. `.card` в одном файле влияет на весь сайт. В большом проекте:
- разработчики не знают какие классы уже есть
- никто не решается удалить «лишний» стиль
- специфичность растёт, перебивать всё сложнее

### BEM — naming-конвенция

```html
<article class="card card--featured">
  <img class="card__cover" src="..." alt="...">
  <div class="card__body">
    <h2 class="card__title">Attack on Titan</h2>
    <p class="card__genre card__genre--action">Экшн</p>
    <a class="card__btn card__btn--primary">Подробнее</a>
  </div>
</article>
```

```
block             — самостоятельный компонент: card
block__element    — часть компонента: card__title
block--modifier   — вариант: card--featured
element--modifier — вариант части: card__btn--primary
```

**Плюсы:** просто, работает везде без сборщика, понятен без CSS-знаний.  
**Минусы:** имена длинные, с глубокой вложенностью (`card__body__header__icon`) — некрасиво.  
**Когда брать:** чистый HTML/CSS без фреймворков, наш проект.

### Утилитарный CSS (идея Tailwind)

```html
<!-- Стили прямо в HTML, каждый класс = одно CSS-свойство -->
<article class="p-6 rounded-lg shadow-md bg-white hover:shadow-lg transition-shadow">
  <h2 class="text-xl font-bold text-gray-900 mb-2">Attack on Titan</h2>
  <span class="inline-block px-2 py-1 text-xs bg-red-100 text-red-700 rounded">Экшн</span>
</article>
```

**Главная идея:** не придумывать имена классов — самая затратная задача в CSS.  
**Плюсы:** быстрая разработка, консистентная дизайн-система.  
**Минусы:** «загрязнённый» HTML, нужно выучить утилиты.  
**Когда брать:** в части 4 упомянем как «куда расти».

### CSS Modules — изоляция на уровне файлов

```css
/* Button.module.css */
.btn { background: var(--color-primary); }
.primary { font-weight: bold; }
```

```js
import styles from './Button.module.css';
// Классы: styles.btn = "Button_btn__a3f1", styles.primary = "Button_primary__b2e2"
```

Классы получают уникальные суффиксы — конфликтов нет.  
**Когда брать:** React/Vue компонентный подход, части 3–4.

### Три парадигмы — сравнение

```
┌──────────────┬──────────────────────┬────────────────────────────┐
│              │  Где живут стили      │  Изоляция                 │
├──────────────┼──────────────────────┼────────────────────────────┤
│ BEM          │  .css файлы          │  Соглашение по именам      │
│ CSS Modules  │  .module.css файлы   │  Компилятор (уникальные id)│
│ Tailwind     │  Прямо в HTML        │  Нет — все утилиты общие   │
│ CSS-in-JS    │  JS-файлы            │  Runtime (генерируемые id) │
└──────────────┴──────────────────────┴────────────────────────────┘
```

**Для нашего проекта (части 1–3):** BEM + CSS custom properties.

---

## 10. Современный CSS: `@layer`, `:has()`, `@container`

### `@layer` — конец войн специфичности

Позволяет задать **приоритет между группами стилей** явно, независимо от специфичности:

```css
/* Объявить порядок слоёв */
@layer reset, base, components, utilities;

@layer reset {
  *, *::before, *::after { box-sizing: border-box; margin: 0; }
}

@layer base {
  a { color: var(--color-primary); }
}

@layer components {
  .btn { padding: 8px 16px; background: var(--color-primary); }
}

@layer utilities {
  .text-center { text-align: center; }
}
```

Стили из более **позднего** слоя побеждают ранние **независимо от специфичности**. Можно смешивать со сторонними библиотеками без `!important`:

```css
@layer reset, vendor, base, components, utilities;
@layer vendor { @import url('bootstrap.css'); }
/* bootstrap больше не может перебить ваши компоненты */
```

### `:has()` — «родительский» селектор

Двадцать лет CSS-разработчики мечтали о способе стилизовать родителя по состоянию дочернего. Теперь он есть:

```css
/* Карточка, внутри которой есть картинка */
.card:has(img) { padding: 0; }

/* Форма с хотя бы одним невалидным полем */
form:has(:invalid) .submit-btn {
  opacity: 0.5;
  pointer-events: none;
}

/* Body при открытой модалке */
body:has(.modal[open]) { overflow: hidden; }

/* Навигационный элемент с активной ссылкой */
.nav-item:has(> .active) { background: var(--color-surface); }
```

Поддержка: все современные браузеры с конца 2023.

### `:is()` и `:where()`

```css
/* Старый способ */
header h1, header h2, header h3 { color: var(--color-primary); }

/* :is() — специфичность = максимум из аргументов */
header :is(h1, h2, h3) { color: var(--color-primary); }

/* :where() — специфичность всегда 0 (легко перебить) */
:where(h1, h2, h3) { line-height: 1.2; }
```

`:where()` идеален для reset-стилей: легко переопределить в любом месте.

---

## 11. DevTools: вкладки Styles и Computed

### Styles — все правила применённые к элементу

```
Что видно в Styles:
─────────────────────────────────────────────────────────────
Все CSS-правила для выбранного элемента, от специфичного
к менее специфичному. Перечёркнутые — перебиты другими.

Что делать:
• Нажать на значение — редактировать прямо в браузере
• Нажать на файл:строку — открыть в Sources
• Чекбоксы слева — включить/выключить правило
• + в углу — добавить новое правило
• :hov кнопка — принудительно применить :hover/:focus
```

### Computed — итоговые значения

```
Computed показывает финальные вычисленные значения:
• var(--space-4) → 16px
• 1fr → 400px
• inherit → 14px (от родителя)
• auto → 200px (вычислено браузером)
```

Полезно когда `var()` или `calc()` не дают ожидаемого результата.

### Проверить контрастность

В Chrome DevTools при клике на цветовой чип в Styles → открывается пикер с автоматической проверкой контраста по WCAG AA/AAA. Красный треугольник = не прошло.

---

## 12. Что CSS-разработчику делать не нужно

```
ЧТО ДЕЛАЕТ РАЗРАБОТЧИК:           ЧТО ДЕЛАЕТ КТО-ТО ДРУГОЙ:
────────────────────────           ──────────────────────────
Реализовать дизайн в коде    ←→   Создать дизайн (дизайнер)
Выбрать CSS-архитектуру      ←→   UX-решения (UX-специалист)
Настроить сборщик стилей     ←→   CI/CD (DevOps)

ЧТО НЕ НУЖНО ДЕЛАТЬ В CSS:
──────────────────────────────────────────────────────────────
× Анимировать width/height      — используй transform
× Писать inline style=""        — только для динамики из JS
× Ставить z-index: 9999         — система токенов
× Копировать блок стилей вместо переменной
× Верстать «под Chrome» без проверки Safari/Firefox
× Фиксировать отступы через магические числа (margin: 37px)
  вместо переменных (margin: var(--space-8))
```

---

## 13. Проект: стилизация аниме-каталога

Берём HTML из части 1 и добавляем стили. Файлы создаются в порядке подключения.

### `css/reset.css`

```css
/* modern-normalize subset */
*, *::before, *::after {
  box-sizing: border-box;
}

* {
  margin: 0;
  padding: 0;
}

html {
  font-size: 100%;
  -webkit-text-size-adjust: 100%;
}

body {
  line-height: 1.5;
  -webkit-font-smoothing: antialiased;
}

img, picture, video, canvas, svg {
  display: block;
  max-width: 100%;
}

input, button, textarea, select {
  font: inherit;  /* формы не наследуют шрифт без этого */
}

p, h1, h2, h3, h4, h5, h6 {
  overflow-wrap: break-word;
}

ul[role="list"], ol[role="list"] {
  list-style: none;
}
```

### `css/variables.css`

```css
:root {
  /* ─── Цвета: тёмная тема по умолчанию ────── */
  --color-bg:           #0f0f0f;
  --color-surface:      #1a1a1a;
  --color-surface-hover:#242424;
  --color-border:       #2a2a2a;
  --color-text:         #f0f0f0;
  --color-text-muted:   #888;
  --color-primary:      #e8613c;  /* аниме-оранжевый */
  --color-primary-hover:#d4522e;
  --color-accent:       #c084fc;  /* фиолетовый акцент */
  --color-rating:       #fbbf24;  /* золотой для рейтинга */
  --color-ongoing:      #34d399;  /* зелёный — выходит */
  --color-completed:    #94a3b8;  /* серый — завершено */

  /* ─── Типографика ────────────────────────── */
  --font-sans:    'Inter', system-ui, -apple-system, sans-serif;
  --font-display: 'Bebas Neue', Impact, sans-serif;
  --text-xs:   0.75rem;
  --text-sm:   0.875rem;
  --text-base: 1rem;
  --text-lg:   1.125rem;
  --text-xl:   1.25rem;
  --text-2xl:  1.5rem;
  --text-3xl:  1.875rem;
  --text-4xl:  2.25rem;

  /* ─── Отступы ────────────────────────────── */
  --space-1:  0.25rem;
  --space-2:  0.5rem;
  --space-3:  0.75rem;
  --space-4:  1rem;
  --space-6:  1.5rem;
  --space-8:  2rem;
  --space-12: 3rem;
  --space-16: 4rem;

  /* ─── Форма ──────────────────────────────── */
  --radius-sm:   4px;
  --radius-md:   8px;
  --radius-lg:   12px;
  --radius-xl:   20px;
  --radius-full: 9999px;
  --shadow-card: 0 4px 20px rgba(0,0,0,0.4);
  --shadow-hover:0 8px 32px rgba(0,0,0,0.6);

  /* ─── Размеры ────────────────────────────── */
  --max-width:    1280px;
  --header-height: 64px;
  --sidebar-width: 240px;
  --card-min-width:220px;

  /* ─── Z-index ────────────────────────────── */
  --z-sticky:  100;
  --z-dropdown:200;
  --z-modal:   1000;

  /* ─── Переходы ───────────────────────────── */
  --transition-fast:   150ms ease;
  --transition-base:   250ms ease;
  --transition-slow:   400ms ease;
}
```

### `css/layout.css`

```css
/* ─── Базовый body ───────────────────────── */
body {
  font-family: var(--font-sans);
  font-size: var(--text-base);
  color: var(--color-text);
  background: var(--color-bg);
  min-height: 100dvh;
  display: flex;
  flex-direction: column;
}

/* ─── Шапка ──────────────────────────────── */
.site-header {
  height: var(--header-height);
  background: var(--color-surface);
  border-bottom: 1px solid var(--color-border);
  position: sticky;
  top: 0;
  z-index: var(--z-sticky);

  display: flex;
  align-items: center;
  gap: var(--space-6);
  padding: 0 var(--space-6);
}

.site-header__logo {
  font-family: var(--font-display);
  font-size: var(--text-xl);
  color: var(--color-primary);
  text-decoration: none;
  letter-spacing: 0.05em;
  flex-shrink: 0;
}

/* ─── Основная навигация ─────────────────── */
.site-nav {
  flex: 1;
}

.site-nav__list {
  display: flex;
  gap: var(--space-1);
  align-items: center;
}

.site-nav__link {
  padding: var(--space-2) var(--space-3);
  border-radius: var(--radius-md);
  color: var(--color-text-muted);
  text-decoration: none;
  font-size: var(--text-sm);
  font-weight: 500;
  transition: color var(--transition-fast), background var(--transition-fast);
}

.site-nav__link:hover,
.site-nav__link[aria-current="page"] {
  color: var(--color-text);
  background: var(--color-surface-hover);
}

/* ─── Главная область ────────────────────── */
main {
  flex: 1;
  padding: var(--space-8) var(--space-6);
  max-width: var(--max-width);
  width: 100%;
  margin: 0 auto;
}

/* ─── Подвал ─────────────────────────────── */
.site-footer {
  background: var(--color-surface);
  border-top: 1px solid var(--color-border);
  padding: var(--space-8) var(--space-6);
  color: var(--color-text-muted);
  font-size: var(--text-sm);
  text-align: center;
}
```

### `css/components.css`

```css
/* ─── Каталог: сетка ─────────────────────── */
.catalog__grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(var(--card-min-width), 1fr));
  gap: var(--space-6);
  margin-top: var(--space-6);
}

/* ─── Карточка франшизы ──────────────────── */
.card {
  background: var(--color-surface);
  border: 1px solid var(--color-border);
  border-radius: var(--radius-xl);
  overflow: hidden;
  container-type: inline-size;  /* для container queries */

  display: grid;
  grid-template-rows: auto 1fr;

  transition: transform var(--transition-base),
              box-shadow var(--transition-base),
              border-color var(--transition-base);
}

.card:hover {
  transform: translateY(-4px);
  box-shadow: var(--shadow-hover);
  border-color: var(--color-primary);
}

/* Постер */
.card__cover-link { display: block; aspect-ratio: 2 / 3; overflow: hidden; }
.card__cover {
  width: 100%;
  height: 100%;
  object-fit: cover;
  transition: transform var(--transition-slow);
}
.card:hover .card__cover { transform: scale(1.05); }

/* Тело карточки */
.card__body {
  padding: var(--space-4);
  display: grid;
  grid-template-rows: auto auto 1fr auto;
  gap: var(--space-2);
}

.card__title {
  font-size: var(--text-lg);
  font-weight: 700;
  line-height: 1.2;
}

.card__title a {
  color: var(--color-text);
  text-decoration: none;
  transition: color var(--transition-fast);
}
.card__title a:hover { color: var(--color-primary); }

.card__subtitle {
  font-size: var(--text-xs);
  color: var(--color-text-muted);
}

/* Жанры */
.card__genres {
  display: flex;
  flex-wrap: wrap;
  gap: var(--space-1);
}

.genre-tag {
  font-size: var(--text-xs);
  padding: 2px var(--space-2);
  border-radius: var(--radius-full);
  border: 1px solid var(--color-border);
  color: var(--color-text-muted);
  transition: border-color var(--transition-fast), color var(--transition-fast);
}
.genre-tag:hover {
  border-color: var(--color-primary);
  color: var(--color-primary);
  cursor: pointer;
}

/* Футер карточки */
.card__footer {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-top: var(--space-2);
}

.card__rating {
  font-weight: 700;
  color: var(--color-rating);
  font-size: var(--text-sm);
}
.card__rating::before { content: "★ "; }

.card__status {
  font-size: var(--text-xs);
  padding: 2px var(--space-2);
  border-radius: var(--radius-full);
}
.card__status--ongoing {
  background: color-mix(in srgb, var(--color-ongoing) 15%, transparent);
  color: var(--color-ongoing);
}
.card__status--completed {
  background: color-mix(in srgb, var(--color-completed) 15%, transparent);
  color: var(--color-completed);
}

/* ─── Container query: широкая карточка ─── */
@container (min-width: 360px) {
  .card { grid-template-rows: 1fr; grid-template-columns: 120px 1fr; }
  .card__cover-link { aspect-ratio: auto; height: 100%; }
}

/* ─── Форма поиска ───────────────────────── */
.search-form {
  display: flex;
  gap: var(--space-2);
  margin-left: auto;
}

.search-form__input {
  padding: var(--space-2) var(--space-3);
  background: var(--color-bg);
  border: 1px solid var(--color-border);
  border-radius: var(--radius-full);
  color: var(--color-text);
  font-size: var(--text-sm);
  width: 220px;
  transition: border-color var(--transition-fast), width var(--transition-base);
}
.search-form__input:focus {
  outline: none;
  border-color: var(--color-primary);
  width: 280px;
}

/* ─── Кнопки ─────────────────────────────── */
.btn {
  display: inline-flex;
  align-items: center;
  gap: var(--space-2);
  padding: var(--space-2) var(--space-4);
  border-radius: var(--radius-md);
  font-size: var(--text-sm);
  font-weight: 500;
  border: 1px solid transparent;
  cursor: pointer;
  text-decoration: none;
  transition: background var(--transition-fast),
              transform var(--transition-fast),
              box-shadow var(--transition-fast);
}
.btn:active { transform: scale(0.97); }

.btn--primary {
  background: var(--color-primary);
  color: white;
}
.btn--primary:hover { background: var(--color-primary-hover); }

.btn--ghost {
  background: transparent;
  color: var(--color-text-muted);
  border-color: var(--color-border);
}
.btn--ghost:hover {
  background: var(--color-surface-hover);
  color: var(--color-text);
}

/* ─── Фильтры ────────────────────────────── */
.filters {
  background: var(--color-surface);
  border: 1px solid var(--color-border);
  border-radius: var(--radius-lg);
  padding: var(--space-4);
  display: flex;
  flex-wrap: wrap;
  gap: var(--space-4);
  align-items: flex-end;
  margin-bottom: var(--space-6);
}

.filters__group legend {
  font-size: var(--text-xs);
  color: var(--color-text-muted);
  font-weight: 600;
  text-transform: uppercase;
  letter-spacing: 0.08em;
  margin-bottom: var(--space-2);
}

.filters__options {
  display: flex;
  flex-wrap: wrap;
  gap: var(--space-2);
}

/* ─── Focus visible ──────────────────────── */
:focus-visible {
  outline: 2px solid var(--color-primary);
  outline-offset: 3px;
  border-radius: var(--radius-sm);
}

/* ─── Reduced motion ─────────────────────── */
@media (prefers-reduced-motion: reduce) {
  *, *::before, *::after {
    animation-duration: 0.01ms !important;
    transition-duration: 0.01ms !important;
  }
}
```

### Подключение в index.html

```html
<head>
  ...
  <link rel="stylesheet" href="/css/reset.css">
  <link rel="stylesheet" href="/css/variables.css">
  <link rel="stylesheet" href="/css/layout.css">
  <link rel="stylesheet" href="/css/components.css">
  <link rel="stylesheet" href="/css/pages/home.css">
</head>
```

---

## Итого: что умеете после части 2

```
✓ Объяснить каскад и специфичность и победить любой CSS-конфликт
✓ Написать box-sizing reset и объяснить зачем
✓ Сверстать layout через Flexbox и Grid
✓ Создать адаптивную сетку без media queries
✓ Использовать CSS custom properties как дизайн-систему
✓ Написать плавные анимации на transform/opacity
✓ Понимать разницу BEM / CSS Modules / Tailwind
✓ Использовать container queries для компонентов
✓ Дебажить стили в DevTools

Следующий шаг — Часть 3: JavaScript.
Оживляем каталог: данные из JSON, фильтрация, генерация страниц.
```

---

> **Вопросы для самопроверки**: [questions/02-css-questions.md](./questions/02-css-questions.md) — 90 вопросов от box model до container queries
