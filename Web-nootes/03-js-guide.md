# Часть 3 — JavaScript: оживляем каталог

> **Учебный проект**: продолжаем [anime-catalog/](./project/anime-catalog/) — добавляем данные из JSON, фильтрацию, генерацию страниц  
> **Вопросы для самопроверки**: [questions/03-js-questions.md](./questions/03-js-questions.md)

---

## Содержание

1. [Что такое JavaScript и его роль в трёх слоях веба](#1-что-такое-javascript-и-его-роль)
2. [Типы данных и операторы](#2-типы-данных-и-операторы)
3. [Переменные, область видимости, замыкания](#3-переменные-область-видимости-замыкания)
4. [Функции и this](#4-функции-и-this)
5. [Массивы, объекты, современный синтаксис](#5-массивы-объекты-современный-синтаксис)
6. [DOM: найти, изменить, создать](#6-dom-найти-изменить-создать)
7. [События и делегирование](#7-события-и-делегирование)
8. [JSON как формат данных и «база данных»](#8-json-как-формат-данных-и-база-данных)
9. [fetch и асинхронность](#9-fetch-и-асинхронность)
10. [localStorage и состояние](#10-localstorage-и-состояние)
11. [Модули: import / export](#11-модули-import--export)
12. [Реактивность руками — предок useState](#12-реактивность-руками--предок-usestate)
13. [Шаблонизация: JSON → DOM](#13-шаблонизация-json--dom)
14. [Event Loop: почему JS «не тормозит»](#14-event-loop-почему-js-не-тормозит)
15. [Virtual DOM и зачем нужны фреймворки](#15-virtual-dom-и-зачем-нужны-фреймворки)
16. [Бандлеры: что делает Vite](#16-бандлеры-что-делает-vite)
17. [Обработка ошибок](#17-обработка-ошибок)
18. [DevTools: Console и Network](#18-devtools-console-и-network)
19. [Что JS-разработчику делать не нужно](#19-что-js-разработчику-делать-не-нужно)
20. [Проект: живой аниме-каталог](#20-проект-живой-аниме-каталог)

---

## 1. Что такое JavaScript и его роль

Из части 1 мы помним: три слоя веб-страницы. HTML — структура, CSS — внешний вид. JavaScript — **поведение и данные**.

```
┌──────────────────────────────────────────────────────────────────┐
│                  ЧТО ДЕЛАЕТ JS НА СТРАНИЦЕ                       │
│                                                                  │
│  Статика (HTML + CSS):         После JS:                         │
│  ─────────────────────         ─────────────────────────────     │
│  Карточки зашиты в HTML   →    Карточки генерируются из данных   │
│  Фильтр — просто форма    →    Фильтр мгновенно перебирает       │
│  Страница загружается     →    Данные догружаются без перезагр.  │
│  Admin — заглушка         →    Admin читает реальные данные      │
└──────────────────────────────────────────────────────────────────┘
```

JS — единственный язык программирования, который понимает браузер нативно. Он запускается прямо на устройстве пользователя, без сервера.

### Анатомия команды JS

```
  const    franchises    =    []
  ──────   ──────────         ──
  Ключевое Имя           Оператор Значение
  слово    переменной    присваив.

  fetchData  (  '/data/franchises.json'  )
  ─────────     ───────────────────────
  Функция   (   аргумент                )
  (вызов)
```

---

## 2. Типы данных и операторы

### Восемь типов

```js
// 7 примитивов:
'Attack on Titan'    // string
9.1                  // number
true                 // boolean
undefined            // undefined — переменная есть, значения нет
null                 // null — намеренное «ничего»
Symbol('id')         // symbol — уникальный ключ
9007199254740993n    // bigint — большие целые числа

// 1 объект:
{ title: 'AoT', rating: 9.1 }  // object (включает массивы, функции...)
```

### `null` vs `undefined` — практическое правило

```js
// undefined — JS сам проставляет «ничего не задано»
let rating;               // undefined (не задали)
obj.nonExistentProp;      // undefined (нет такого свойства)
function fn() {}; fn();   // undefined (функция ничего не вернула)

// null — программист ставит сам: «значение намеренно пустое»
const result = getUserOrNull();  // явно: «может быть пользователь, может быть ничего»

// Проверить оба сразу:
if (value == null) { /* value либо null, либо undefined */ }
```

### `===` всегда, `==` почти никогда

```js
'9.1' == 9.1   // true  — приведение типов, сюрпризы
'9.1' === 9.1  // false — строгое, предсказуемое

// Классика собеседований — почему это true:
[] == ![]      // true
// 1. ![] → false (массив truthy, отрицание = false)
// 2. [] == false → '' == false → 0 == 0 → true
// Мораль: не используйте ==
```

### Truthy и Falsy

В условии (`if`, `&&`, `||`, тернарный) JS приводит значение к boolean. **Falsy — ровно 7 значений:**

```js
false  0  -0  0n  ''  null  undefined  NaN

// Всё остальное — truthy, включая:
[]          // пустой массив — truthy!
{}          // пустой объект — truthy!
'0'         // строка '0' — truthy!
-1          // отрицательное число — truthy!
```

### Ловушки с числами

```js
0.1 + 0.2          // 0.30000000000000004 — IEEE 754, не баг JS
NaN === NaN        // false — единственное значение не равное себе
typeof null        // 'object' — историческая ошибка, не исправляют

Number.isNaN(NaN)  // true — правильная проверка
Number.isNaN('hi') // false — строгая, только для NaN
isNaN('hi')        // true — небезопасная, приводит тип

// Для рейтингов и денег — никогда не полагайся на точность float
const rating = 9.1;    // ок, точность достаточна
const price = 9.99;    // для денег → храни в целых: 999 центов
```

---

## 3. Переменные, область видимости, замыкания

### `const`, `let`, `var` — правило на всю жизнь

```
const  — по умолчанию. Нельзя переприсвоить.
let    — когда знаешь, что будешь переприсваивать.
var    — никогда. Только в чужом legacy-коде.
```

Почему `var` опасен:

```js
// var — область видимости ФУНКЦИЯ, а не блок
if (true) {
  var leak = 'утекло';
  let safe = 'в блоке';
}
console.log(leak);  // 'утекло' — var не уважает {брейсы}
console.log(safe);  // ReferenceError — let уважает

// var поднимается (hoisting) с undefined
console.log(x);  // undefined, не ошибка — сюрприз
var x = 5;

// let/const поднимаются тоже, но в TDZ — бросают ошибку
console.log(y);  // ReferenceError: Cannot access before initialization
let y = 5;       // ← TDZ (Temporal Dead Zone) до этой строки
```

### Замыкание — функция помнит своё окружение

Замыкание = функция + все переменные, видимые в **месте её объявления**.

```js
function makeRatingFilter(minRating) {
  // minRating живёт в замыкании — запомнится навсегда
  return function(franchise) {
    return franchise.rating >= minRating;
  };
}

const topFilter = makeRatingFilter(9.0);     // минимум 9.0
const goodFilter = makeRatingFilter(8.5);    // минимум 8.5

franchises.filter(topFilter);   // только с 9.0+
franchises.filter(goodFilter);  // только с 8.5+
```

**Зачем это знать:** debounce, throttle, фабрики функций, React Hooks — всё строится на замыканиях. Вопрос «почему `useEffect` видит старое значение?» — это замыкание захватило старое.

### Классический баг с `var` в цикле

```js
// ❌ var — одна переменная на всю функцию
for (var i = 0; i < 3; i++) {
  setTimeout(() => console.log(i), 100);
}
// Выведет: 3, 3, 3  — цикл уже кончился, i=3

// ✅ let — новое связывание на каждую итерацию
for (let i = 0; i < 3; i++) {
  setTimeout(() => console.log(i), 100);
}
// Выведет: 0, 1, 2
```

---

## 4. Функции и this

### Function declaration vs expression vs arrow

```js
// Declaration — поднимается целиком (можно вызвать до объявления)
sayHello();  // работает!
function sayHello() { console.log('hi'); }

// Expression — переменная поднимается, но не тело
greet();  // TypeError: greet is not a function
const greet = function() { console.log('hi'); };

// Arrow — нет своего this, нет arguments, нельзя с new
const double = x => x * 2;
const getUser = () => ({ name: 'Alice' });  // объект — в скобках!
```

### Четыре правила `this`

`this` — контекст вызова. Определяется **в момент вызова**, не объявления:

```js
// 1. new — this = новый объект
function User(name) { this.name = name; }
const u = new User('Alice');  // this = u

// 2. call/apply/bind — this = явно указанный объект
function greet() { return this.name; }
greet.call({ name: 'Bob' });      // 'Bob'
greet.apply({ name: 'Carol' });   // 'Carol'
const bound = greet.bind({ name: 'Dan' });
bound();                          // 'Dan'

// 3. Метод объекта — this = объект перед точкой
const obj = { name: 'Eve', greet() { return this.name; } };
obj.greet();  // 'Eve'

// 4. Обычный вызов — this = undefined (в strict mode) или window
const fn = obj.greet;  // скопировали метод — this потерялся!
fn();  // undefined
```

### Стрелочная функция не имеет своего `this`

```js
// ❌ Стрелка как метод — this не тот
const obj = {
  name: 'Alice',
  greet: () => console.log(this?.name)  // undefined!
};

// ✅ Стрелка как колбек внутри метода — берёт this метода
class Timer {
  seconds = 0;
  start() {
    setInterval(() => this.seconds++, 1000);  // this = Timer
  }
}
```

**Правило**: методы объекта/класса — обычные функции. Колбеки внутри методов — стрелочные.

---

## 5. Массивы, объекты, современный синтаксис

### Главные методы массивов

```js
const franchises = [
  { id: 'aot', title: 'Attack on Titan', rating: 9.1, status: 'completed' },
  { id: 'jjk', title: 'Jujutsu Kaisen',  rating: 8.6, status: 'ongoing' },
  { id: 'fma', title: 'FMA Brotherhood', rating: 9.1, status: 'completed' },
];

// map — преобразовать каждый элемент, вернуть новый массив
const titles = franchises.map(f => f.title);
// ['Attack on Titan', 'Jujutsu Kaisen', 'FMA Brotherhood']

// filter — оставить только подходящие
const topRated = franchises.filter(f => f.rating >= 9.0);
// [aot, fma]

// find — найти первый подходящий (или undefined)
const aot = franchises.find(f => f.id === 'aot');

// some — есть ли хотя бы один
const hasOngoing = franchises.some(f => f.status === 'ongoing');  // true

// every — все ли
const allRated = franchises.every(f => f.rating > 0);  // true

// reduce — сложить всё в одно значение
const avgRating = franchises.reduce((sum, f) => sum + f.rating, 0) / franchises.length;

// sort — отсортировать (МУТИРУЕТ, используйте toSorted)
const sorted = [...franchises].sort((a, b) => b.rating - a.rating);
// Или с ES2023:
const sorted = franchises.toSorted((a, b) => b.rating - a.rating);
```

> `map`, `filter`, `reduce` **не мутируют** исходный массив. `sort`, `reverse`, `splice` — **мутируют**. В React/Redux мутация = баг.

### Деструктуризация — читать как объявление

```js
// Объект — имена совпадают
const { title, rating } = franchise;

// Переименование
const { title: animeName, rating: score } = franchise;

// Дефолт если undefined
const { status = 'unknown' } = franchise;

// Вложенно
const { studio: { name: studioName } } = franchise;

// Rest — «остальное»
const { id, ...rest } = franchise;  // rest = { title, rating, ... }

// Массив
const [first, second, ...others] = franchises;
const [,, third] = franchises;  // пропуск через запятую

// В параметрах функции
function renderCard({ title, rating, status = 'unknown' }) {
  return `<h2>${title}</h2><span>${rating}</span>`;
}
```

### Spread и Optional chaining

```js
// Spread — скопировать и добавить
const updated = { ...franchise, rating: 9.5 };  // новый объект
const extended = [...franchises, newFranchise];  // новый массив

// Глубокая копия — structuredClone (нативно с 2022)
const deepCopy = structuredClone(franchise);

// Optional chaining — не ронять при null/undefined
const studio = franchise?.studio?.name;  // undefined если чего-то нет
const first = arr?.[0];                  // undefined если arr === null
const result = obj?.method?.();          // undefined если метода нет

// Nullish coalescing — дефолт только при null/undefined
const rating = franchise.rating ?? 'Нет оценки';
// vs || — дефолт при любом falsy (включая 0):
const count = data.count ?? 0;  // правильно: 0 — валидное значение
const name = data.name || 'Unknown';  // ок: пустое имя → Unknown
```

---

## 6. DOM: найти, изменить, создать

### Найти элемент

```js
// Один по CSS-селектору (современный стандарт)
const catalog = document.querySelector('.catalog__grid');
const firstCard = document.querySelector('.card');

// Все по CSS-селектору — возвращает NodeList
const cards = document.querySelectorAll('.card');
// NodeList — не массив! Нет .map()
const cardsArray = [...cards];  // превратить в массив

// Найти внутри элемента, а не всего документа
const title = card.querySelector('.card__title');
```

### Изменить элемент

```js
// Содержимое — textContent безопасно, innerHTML парсит HTML (XSS!)
card.textContent = 'Attack on Titan';  // только текст, безопасно
card.innerHTML = '<b>Attack on Titan</b>';  // парсит HTML — опасно с данными от юзера

// Атрибуты
card.setAttribute('data-id', 'aot');
card.getAttribute('data-id');          // 'aot'
card.dataset.id;                       // 'aot' — удобный доступ к data-*
card.dataset.rating = 9.1;            // установить data-rating="9.1"

// Классы
card.classList.add('card--featured');
card.classList.remove('card--loading');
card.classList.toggle('card--selected');        // переключить
card.classList.contains('card--featured');      // проверить
card.classList.replace('card--old', 'card--new');

// Стили (только для динамических значений — остальное через CSS)
card.style.setProperty('--custom-color', '#ff0000');
```

### Создать элемент

```js
// Создать и наполнить
const article = document.createElement('article');
article.className = 'card';
article.dataset.id = 'aot';

const title = document.createElement('h2');
title.className = 'card__title';
title.textContent = 'Attack on Titan';  // textContent — безопасно

article.appendChild(title);

// Вставить в DOM
document.querySelector('.catalog__grid').appendChild(article);

// Современные методы вставки
parent.append(el1, el2, 'text');    // в конец, принимает несколько
parent.prepend(el);                  // в начало
sibling.before(el);                  // перед соседом
sibling.after(el);                   // после соседа
oldEl.replaceWith(newEl);            // заменить
el.remove();                         // удалить себя

// Из HTML-строки — через template (безопаснее innerHTML на родителе)
const tpl = document.createElement('template');
tpl.innerHTML = `<article class="card"><h2>${title}</h2></article>`;
const node = tpl.content.firstElementChild.cloneNode(true);
parent.appendChild(node);
```

### Производительность DOM

```js
// ❌ Много вставок — много reflow
franchises.forEach(f => {
  catalog.appendChild(createCard(f));  // reflow каждый раз
});

// ✅ DocumentFragment — один reflow
const fragment = document.createDocumentFragment();
franchises.forEach(f => fragment.appendChild(createCard(f)));
catalog.appendChild(fragment);  // один reflow
```

---

## 7. События и делегирование

### Добавить и убрать обработчик

```js
// Добавить
function handleClick(event) {
  console.log(event.target);  // элемент где кликнули
}
button.addEventListener('click', handleClick);

// Убрать — нужна ссылка на ту же функцию
button.removeEventListener('click', handleClick);

// Один раз
button.addEventListener('click', handleClick, { once: true });

// AbortController — снять несколько сразу
const ctrl = new AbortController();
btn1.addEventListener('click', h1, { signal: ctrl.signal });
btn2.addEventListener('click', h2, { signal: ctrl.signal });
ctrl.abort();  // снимает оба
```

### `event.target` vs `event.currentTarget`

```js
list.addEventListener('click', e => {
  e.target          // элемент где реально кликнули (может быть вложенный)
  e.currentTarget   // элемент, на котором висит слушатель (= list)
});
```

### Делегирование — один обработчик вместо сотни

Вместо навешивания обработчика на каждую карточку — один на контейнер:

```js
const catalog = document.querySelector('.catalog__grid');

catalog.addEventListener('click', (event) => {
  // Найти ближайший .card от точки клика вверх по дереву
  const card = event.target.closest('.card');
  if (!card) return;  // клик был не по карточке

  const id = card.dataset.id;
  openFranchisePage(id);
});
```

**Преимущества делегирования:**
- Один обработчик вместо N
- Работает для **динамически добавленных** элементов — важно когда JS генерирует карточки из JSON
- Меньше памяти

### Всплытие событий

```
Клик на <span> внутри <a> внутри <li> внутри <ul>:

Вниз (capture):    document → html → body → ul → li → a → span
На цели (target):  span
Вверх (bubble):    span → a → li → ul → body → html → document
```

По умолчанию обработчики на bubble-фазе. Это значит: клик на `<span>` дойдёт и до `<a>` и до `<ul>`.

```js
event.stopPropagation();  // не пускать выше — редко нужно
event.preventDefault();   // отменить дефолт (сабмит формы, переход по ссылке)
```

---

## 8. JSON как формат данных и «база данных»

### Что такое JSON

JSON (JavaScript Object Notation) — текстовый формат обмена данными. Выглядит как JS-объект, но это просто строка с правилами.

```json
{
  "id": "aot",
  "title": "Attack on Titan",
  "rating": 9.1,
  "genres": ["action", "drama"],
  "studio": null,
  "ongoing": false
}
```

**Отличия от JS-объекта:**
- Только двойные кавычки `"` — одинарные не работают
- Нет комментариев
- Нет `undefined` — только `null`
- Нет функций
- Нет trailing comma
- Ключи всегда в кавычках

### Парсинг и сериализация

```js
// Строка → объект
const obj = JSON.parse('{"title":"AoT","rating":9.1}');
obj.title  // 'AoT'

// Объект → строка
const str = JSON.stringify({ title: 'AoT', rating: 9.1 });
// '{"title":"AoT","rating":9.1}'

// С форматированием (для чтения)
JSON.stringify(obj, null, 2);
/*
{
  "title": "AoT",
  "rating": 9.1
}
*/
```

### Что JSON.stringify теряет

```js
JSON.stringify({
  fn: () => {},          // функции → исчезают
  undef: undefined,      // undefined → исчезает
  date: new Date(),      // Date → строка "2024-01-15T..."
  big: 9007199254741n,   // BigInt → ошибка!
  sym: Symbol(),         // Symbol → исчезает
  inf: Infinity,         // Infinity → null
});
// '{"date":"2024-01-15T..."}'  — почти всё потеряно
```

### Структура данных каталога

Данные в JSON нужно **нормализовать** — разбить на связанные таблицы. Это предотвращает дублирование:

```
БЕЗ нормализации (плохо):
franchises.json содержит встроенные треки:
{
  "id": "aot",
  "title": "Attack on Titan",
  "tracks": [
    { "title": "Guren no Yumiya", "artist": "Linked Horizon" },
    ...
  ]
}
Проблема: если нужно обновить трек — надо найти его
в каждой франшизе, где он упоминается.

С нормализацией (правильно):
franchises.json:  { "id": "aot", "title": "...", ... }
music.json:       { "id": "aot-op1", "franchise_id": "aot", ... }
                                      ↑
                               ссылка на франшизу
```

```js
// Соединить данные из двух файлов (как SQL JOIN)
const aotTracks = music.filter(track => track.franchise_id === 'aot');

// Найти франшизу по треку
const franchise = franchises.find(f => f.id === track.franchise_id);
```

### Что такое базы данных — обзор

На этом курсе мы работаем с JSON-файлами как с «базой данных». Но в реальном мире:

```
┌────────────────────────────────────────────────────────────────┐
│              ХРАНИЛИЩА ДАННЫХ                                  │
│                                                                │
│  JSON-файлы (мы сейчас):                                       │
│  • Просто файл на диске                                        │
│  • Нет поиска, нет транзакций, нет одновременного доступа      │
│  • Подходит: конфиги, небольшие статичные данные               │
│                                                                │
│  localStorage / IndexedDB (браузер):                           │
│  • Хранится в браузере пользователя                            │
│  • Нет доступа с сервера                                       │
│  • localStorage: строки, ~5MB                                  │
│  • IndexedDB: структурированные данные, большие объёмы         │
│                                                                │
│  SQLite (файл-БД):                                             │
│  • Один файл, SQL запросы, транзакции                          │
│  • Для приложений среднего размера                             │
│  • Часть 4: мигрируем туда                                     │
│                                                                │
│  PostgreSQL / MySQL (серверная БД):                            │
│  • Полноценная СУБД, несколько подключений                     │
│  • Для продакшн-приложений                                     │
│                                                                │
│  MongoDB (документная БД):                                     │
│  • Хранит документы (похожи на JSON-объекты)                   │
│  • Гибкая схема — удобно для быстрых итераций                  │
└────────────────────────────────────────────────────────────────┘
```

---

## 9. fetch и асинхронность

### Почему нужна асинхронность

JS однопоточный. Если остановить поток на ожидание сети — страница зависнет. Асинхронный код говорит: «начни загружать, а я пока займусь другим. Как загрузится — скажи».

### `fetch` — загрузить данные по сети

```js
// Базовый GET-запрос
const response = await fetch('/data/franchises.json');
const franchises = await response.json();

// fetch НЕ кидает ошибку на 404/500 — только на сетевой сбой!
if (!response.ok) {
  throw new Error(`HTTP ${response.status}: ${response.statusText}`);
}
```

### `async/await` — асинхронный код как синхронный

```js
// async функция всегда возвращает Promise
async function loadCatalog() {
  try {
    // await приостанавливает функцию, но не поток
    const response = await fetch('/data/franchises.json');

    if (!response.ok) throw new Error(`Ошибка ${response.status}`);

    const franchises = await response.json();
    return franchises;
  } catch (error) {
    console.error('Не удалось загрузить каталог:', error);
    return [];  // безопасный дефолт
  }
}

// Вызов
const franchises = await loadCatalog();
```

### Параллельная загрузка

```js
// ❌ Последовательно — ждём каждый запрос
const franchises = await loadFranchises();  // 300ms
const music = await loadMusic();            // 300ms
// Итого: 600ms

// ✅ Параллельно — оба стартуют сразу
const [franchises, music] = await Promise.all([
  loadFranchises(),
  loadMusic(),
]);
// Итого: ~300ms
```

`Promise.all` — запустить всё, дождаться всего. Если один упал — весь упал.  
`Promise.allSettled` — запустить всё, дождаться всего, ошибки не прерывают.

---

## 10. localStorage и состояние

### Что такое состояние

**Состояние (state)** — данные, которые определяют, как выглядит UI в данный момент. Активные фильтры, список избранного, открыта ли модалка — всё это состояние.

```js
// Состояние каталога
const state = {
  franchises: [],         // все франшизы из JSON
  filters: {
    genres: [],           // выбранные жанры
    status: 'all',        // фильтр статуса
    minRating: 0,         // минимальный рейтинг
  },
  favorites: new Set(),   // id избранных
};
```

### localStorage — сохранить между сессиями

```js
// Сохранить
localStorage.setItem('favorites', JSON.stringify([...favorites]));

// Прочитать
const raw = localStorage.getItem('favorites');
const favorites = raw ? JSON.parse(raw) : [];

// Удалить
localStorage.removeItem('favorites');

// Очистить всё
localStorage.clear();
```

**Ограничения localStorage:**
- Только строки — объекты нужно `JSON.stringify`
- ~5MB на домен
- Синхронный — блокирует основной поток при больших данных
- Недоступен с сервера
- **Никогда не хранить пароли, токены, персональные данные**

---

## 11. Модули: import / export

### Зачем нужны модули

Без модулей — один огромный `app.js`, где всё в глобальной области видимости. Функция `renderCard` из одного разработчика случайно перезаписывает `renderCard` другого. Модули дают каждому файлу **свою область видимости**.

```html
<!-- Включить как модуль -->
<script type="module" src="/js/app.js"></script>
<!-- type="module" обязателен. Без него — нет import/export -->
```

### Named exports — несколько из одного файла

```js
// data.js
export async function loadFranchises() {
  const res = await fetch('/data/franchises.json');
  return res.json();
}

export async function loadMusic() {
  const res = await fetch('/data/music.json');
  return res.json();
}

export function filterFranchises(franchises, filters) {
  return franchises
    .filter(f => filters.status === 'all' || f.status === filters.status)
    .filter(f => f.rating >= filters.minRating)
    .filter(f => {
      if (!filters.genres.length) return true;
      return filters.genres.every(g => f.genres.includes(g));
    });
}
```

```js
// app.js
import { loadFranchises, loadMusic, filterFranchises } from './data.js';
// Расширение .js обязательно в браузере
```

### Default export — одна главная вещь из файла

```js
// render.js
export default function renderCard(franchise) {
  const article = document.createElement('article');
  // ...
  return article;
}

// app.js
import renderCard from './render.js';  // имя выбираем сами
```

### Что происходит в браузере без сборщика

```
app.js       → import { loadFranchises } from './data.js'
data.js      → import { filterFranchises } from './utils.js'
utils.js     → (нет импортов)

Браузер:
1. Загрузить app.js
2. Обнаружить import data.js → загрузить
3. Обнаружить import utils.js → загрузить
4. Выполнить в обратном порядке: utils → data → app
```

Без сборщика каждый `import` = отдельный HTTP-запрос. На больших проектах это медленно. Для продакшена нужен **бандлер** (см. раздел 16).

---

## 12. Реактивность руками — предок useState

### Проблема: данные и UI рассинхронизированы

Сейчас при изменении фильтра нужно вручную:
1. Обновить объект состояния
2. Вызвать функцию рендера
3. Не забыть ни одного связанного места

Если состояние используется в 5 местах — нужно 5 вызовов. Легко забыть один.

### Простой реактивный store

```js
// store.js — реактивное хранилище состояния
function createStore(initialState) {
  let state = { ...initialState };
  const listeners = new Set();

  return {
    // Прочитать текущее состояние
    getState() {
      return state;
    },

    // Обновить состояние и уведомить всех подписчиков
    setState(updates) {
      state = { ...state, ...updates };
      listeners.forEach(listener => listener(state));
    },

    // Подписаться на изменения
    subscribe(listener) {
      listeners.add(listener);
      // Вернуть функцию отписки
      return () => listeners.delete(listener);
    },
  };
}

// Создать store для каталога
export const catalogStore = createStore({
  franchises: [],
  filtered: [],
  filters: { genres: [], status: 'all', minRating: 0 },
  favorites: new Set(),
  loading: false,
});
```

```js
// app.js — подписаться на изменения и рендерить
import { catalogStore } from './store.js';
import { renderCatalog } from './render.js';

// Один раз подписаться — рендер будет вызываться автоматически
catalogStore.subscribe(state => {
  renderCatalog(state.filtered);
  updateFiltersUI(state.filters);
  updateFavoritesCount(state.favorites.size);
});

// Изменить состояние — UI обновится сам
document.querySelector('#filter-status').addEventListener('change', e => {
  const { filters } = catalogStore.getState();
  const newFilters = { ...filters, status: e.target.value };
  const newFiltered = filterFranchises(
    catalogStore.getState().franchises,
    newFilters
  );
  catalogStore.setState({ filters: newFilters, filtered: newFiltered });
});
```

**Это и есть реактивность.** При изменении данных UI обновляется автоматически. `useState` в React делает то же самое, просто автоматически определяет какие компоненты зависят от изменившихся данных.

---

## 13. Шаблонизация: JSON → DOM

### Паттерн: из массива данных в список элементов

```js
// render.js

// Создать одну карточку из данных
function createCard(franchise) {
  const article = document.createElement('article');
  article.className = 'card';
  article.dataset.id = franchise.id;
  article.dataset.rating = franchise.rating;
  article.dataset.status = franchise.status;
  article.dataset.genres = franchise.genres.join(',');

  // Заполнить структуру (не innerHTML — XSS)
  const cover = document.createElement('a');
  cover.className = 'card__cover-link';
  cover.href = `/pages/franchise/?id=${franchise.id}`;

  const img = document.createElement('img');
  img.className = 'card__cover';
  img.src = franchise.cover;
  img.alt = `${franchise.title} — постер`;
  img.width = 300;
  img.height = 450;
  img.loading = 'lazy';
  cover.appendChild(img);

  const body = document.createElement('div');
  body.className = 'card__body';

  const title = document.createElement('h2');
  title.className = 'card__title';
  const titleLink = document.createElement('a');
  titleLink.href = `/pages/franchise/?id=${franchise.id}`;
  titleLink.textContent = franchise.title;
  title.appendChild(titleLink);

  const subtitle = document.createElement('p');
  subtitle.className = 'card__subtitle';
  subtitle.textContent = franchise.original_title;

  const genres = document.createElement('ul');
  genres.className = 'card__genres';
  genres.setAttribute('aria-label', 'Жанры');
  franchise.genres.forEach(genre => {
    const li = document.createElement('li');
    li.className = 'genre-tag';
    li.textContent = genre;
    li.dataset.genre = genre;
    genres.appendChild(li);
  });

  const footer = document.createElement('footer');
  footer.className = 'card__footer';

  const rating = document.createElement('span');
  rating.className = 'card__rating';
  rating.setAttribute('aria-label', `Рейтинг ${franchise.rating} из 10`);
  rating.textContent = franchise.rating;

  const status = document.createElement('span');
  status.className = `card__status card__status--${franchise.status}`;
  status.textContent = franchise.status === 'ongoing' ? 'Выходит' : 'Завершено';

  footer.appendChild(rating);
  footer.appendChild(status);

  body.append(title, subtitle, genres, footer);
  article.append(cover, body);

  return article;
}

// Нарисовать весь каталог
export function renderCatalog(franchises) {
  const grid = document.querySelector('.catalog__grid');
  grid.innerHTML = '';  // очистить

  if (franchises.length === 0) {
    grid.innerHTML = '<p class="catalog__empty">Ничего не найдено</p>';
    return;
  }

  const fragment = document.createDocumentFragment();
  franchises.forEach(f => fragment.appendChild(createCard(f)));
  grid.appendChild(fragment);
}
```

### Использование `<template>` для HTML-шаблонов

Альтернатива — описать структуру карточки в HTML и клонировать:

```html
<!-- В index.html — шаблон не рендерится -->
<template id="card-template">
  <article class="card">
    <a class="card__cover-link">
      <img class="card__cover" loading="lazy" width="300" height="450">
    </a>
    <div class="card__body">
      <h2 class="card__title"><a></a></h2>
      <p class="card__subtitle"></p>
      <ul class="card__genres" aria-label="Жанры"></ul>
      <footer class="card__footer">
        <span class="card__rating"></span>
        <span class="card__status"></span>
      </footer>
    </div>
  </article>
</template>
```

```js
// В JS — клонировать и заполнить данными
function createCardFromTemplate(franchise) {
  const template = document.getElementById('card-template');
  const clone = template.content.cloneNode(true);  // true = глубокое клонирование

  const article = clone.querySelector('article');
  article.dataset.id = franchise.id;

  const link = clone.querySelector('.card__cover-link');
  link.href = `/pages/franchise/?id=${franchise.id}`;

  const img = clone.querySelector('.card__cover');
  img.src = franchise.cover;
  img.alt = `${franchise.title} — постер`;

  clone.querySelector('.card__title a').textContent = franchise.title;
  clone.querySelector('.card__subtitle').textContent = franchise.original_title;
  clone.querySelector('.card__rating').textContent = franchise.rating;

  // Жанры
  const genresList = clone.querySelector('.card__genres');
  franchise.genres.forEach(genre => {
    const li = document.createElement('li');
    li.className = 'genre-tag';
    li.textContent = genre;
    genresList.appendChild(li);
  });

  return clone;
}
```

**`<template>` — прямой предок JSX.** React делает то же самое: описывает структуру UI и заполняет данными. Только в JSX это выглядит как HTML в JS-коде, а компилятор превращает это в вызовы `createElement`.

---

## 14. Event Loop: почему JS «не тормозит»

### JS однопоточный, но не синхронный

```
┌────────────────────────────────────────────────────────────────┐
│                    EVENT LOOP                                  │
│                                                                │
│  Call Stack          Microtask Queue     Macrotask Queue       │
│  (выполняется        (Promise.then,      (setTimeout,          │
│   сейчас)            queueMicrotask)      setInterval,         │
│                                           UI-события)          │
│  ┌─────────┐         ┌──────────────┐    ┌─────────────────┐  │
│  │ app()   │         │ .then(log)   │    │ setTimeout(fn)  │  │
│  │ fetch() │         │ .then(render)│    │ click handler   │  │
│  └─────────┘         └──────────────┘    └─────────────────┘  │
│                                                                │
│  Цикл:                                                         │
│  1. Выполнить весь синхронный код (стек опустел)               │
│  2. Выполнить ВСЕ microtasks (до опустошения)                  │
│  3. (Браузер): отрендерить если нужно                          │
│  4. Взять ОДНУ macrotask → выполнить → перейти к шагу 2        │
└────────────────────────────────────────────────────────────────┘
```

### Почему microtasks перед macrotasks

```js
console.log('1');               // синхронно

setTimeout(() => console.log('2'), 0);  // macrotask

Promise.resolve().then(() => console.log('3'));  // microtask

console.log('4');               // синхронно

// Вывод: 1, 4, 3, 2
// Сначала весь синхронный код (1, 4)
// Потом все microtasks (3)
// Потом macrotasks (2)
```

### Практическое значение

```js
// Не блокировать UI при тяжёлых вычислениях:

// ❌ Зависает страница на ~секунду
function heavySync() {
  for (let i = 0; i < 1_000_000; i++) processItem(i);
}

// ✅ Разбить на чанки через setTimeout
async function heavyAsync(items) {
  const CHUNK = 100;
  for (let i = 0; i < items.length; i += CHUNK) {
    const chunk = items.slice(i, i + CHUNK);
    chunk.forEach(processItem);
    // «отпустить» поток — браузер успеет обновить UI
    await new Promise(r => setTimeout(r, 0));
  }
}
```

---

## 15. Virtual DOM и зачем нужны фреймворки

### Проблема ручного DOM

Наш каталог: при изменении фильтра мы делаем `grid.innerHTML = ''` и рисуем всё заново. Это работает для 20 карточек. При 2000 карточек — медленно. При частых обновлениях — мигание.

Умнее было бы: сравнить что было с тем что должно быть, и обновить **только изменившееся**.

### Virtual DOM — идея React

```
┌────────────────────────────────────────────────────────────────┐
│                    КАК РАБОТАЕТ VIRTUAL DOM                    │
│                                                                │
│  1. Описываем UI через объекты (виртуальные ноды):             │
│     { type: 'article', props: { className: 'card' },           │
│       children: [{ type: 'h2', ... }] }                        │
│                                                                │
│  2. Состояние меняется → создаётся НОВЫЙ виртуальный DOM       │
│                                                                │
│  3. Diff-алгоритм (reconciliation) сравнивает старый и новый   │
│     виртуальный DOM                                            │
│                                                                │
│  4. Только РЕАЛЬНЫЕ изменения применяются к DOM               │
│     («минимальная операция»)                                   │
│                                                                │
│  ИТОГ: разработчик думает «нарисуй весь UI»                    │
│        React думает «обнови только что изменилось»             │
└────────────────────────────────────────────────────────────────┘
```

### Что значит JSX

```jsx
// JSX (не работает в браузере напрямую)
const card = (
  <article className="card" data-id={franchise.id}>
    <h2 className="card__title">{franchise.title}</h2>
  </article>
);

// Компилируется (через Babel/Vite) в:
const card = React.createElement(
  'article',
  { className: 'card', 'data-id': franchise.id },
  React.createElement('h2', { className: 'card__title' }, franchise.title)
);
```

Это просто вызов функций. `<template>` и шаблонизация которую мы делали — прямой предок этого.

### Когда фреймворк не нужен

Для нашего каталога vanilla JS достаточен. Фреймворк нужен когда:

```
✓ Много взаимозависимых состояний (пользователь, корзина, фильтры, ...)
✓ Частые точечные обновления сложного UI
✓ Большая команда — фреймворк даёт общие соглашения
✗ Маркетинговый сайт
✗ Простой каталог с фильтрацией
✗ Форма обратной связи
```

---

## 16. Бандлеры: что делает Vite

### Проблема без бандлера

```js
// В браузере без бандлера:
import { renderCard } from './render.js';           // 1 запрос
import { loadFranchises } from './data.js';          // 1 запрос
import { createStore } from './store.js';            // 1 запрос
// + все их зависимости...

// 20 файлов = 20 HTTP-запросов
// На медленной связи — долго
```

### Что делает бандлер

```
┌────────────────────────────────────────────────────────────────┐
│                     VITE (бандлер)                             │
│                                                                │
│  DEV-режим (npm run dev):                                      │
│  • Не бандлит — отдаёт файлы как есть через HTTP/2             │
│  • Мгновенный Hot Module Replacement (HMR)                     │
│  • Изменил файл → только он перезагружается                    │
│                                                                │
│  PROD-режим (npm run build):                                   │
│  • Объединяет все импорты в один (или несколько) файл          │
│  • Минифицирует — удаляет пробелы, переименует переменные      │
│  • Tree-shaking — удаляет неиспользуемый код                   │
│  • Разделяет на чанки — vendor.js отдельно от app.js           │
│  • Добавляет хэши к именам файлов (cache busting)              │
└────────────────────────────────────────────────────────────────┘
```

### Почему `import` в браузере ≠ в Node

```js
// В Node.js (CommonJS):
const path = require('path');        // синхронный, блокирующий
module.exports = { myFunc };

// В Node.js (ES Modules):
import { readFile } from 'fs/promises';
export const myFunc = () => {};

// В браузере:
import './styles.css';               // так нельзя — CSS не JS
import logo from './logo.svg';       // и так нельзя нативно
// Это умеет только бандлер!
```

Бандлер обрабатывает все виды импортов и превращает их в то, что понимает браузер.

---

## 17. Обработка ошибок

### `try / catch / finally`

```js
try {
  const data = JSON.parse(invalidJson);     // может бросить SyntaxError
  processData(data);
} catch (error) {
  console.error('Ошибка парсинга:', error.message);
  showErrorMessage('Некорректный формат данных');
} finally {
  hideLoadingSpinner();  // выполнится ВСЕГДА — успех или ошибка
}
```

### Свои классы ошибок

```js
class NetworkError extends Error {
  constructor(message, status) {
    super(message);
    this.name = 'NetworkError';
    this.status = status;
  }
}

class DataError extends Error {
  constructor(message, field) {
    super(message);
    this.name = 'DataError';
    this.field = field;
  }
}

// Использование
async function loadFranchise(id) {
  const response = await fetch(`/data/franchises.json`);
  if (!response.ok) throw new NetworkError('Не удалось загрузить', response.status);

  const data = await response.json();
  const franchise = data.find(f => f.id === id);
  if (!franchise) throw new DataError(`Франшиза не найдена: ${id}`, 'id');

  return franchise;
}

// Обработка
try {
  const franchise = await loadFranchise('aot');
  renderFranchise(franchise);
} catch (error) {
  if (error instanceof NetworkError) {
    showBanner(`Ошибка сети: ${error.status}`);
  } else if (error instanceof DataError) {
    showNotFound(error.field);
  } else {
    console.error('Неизвестная ошибка:', error);
  }
}
```

### Никогда не глотайте ошибки молча

```js
// ❌ Самый опасный анти-паттерн
try {
  doSomething();
} catch (e) {}  // ошибка исчезла, баг будет невозможно найти

// ✅ Минимум — залогировать
try {
  doSomething();
} catch (e) {
  console.error('[loadCatalog]', e);
}
```

---

## 18. DevTools: Console и Network

### Console — больше чем `console.log`

```js
console.log('текст');              // обычный вывод
console.error('ошибка');           // красный, со стеком
console.warn('предупреждение');    // жёлтый
console.table(franchises);         // таблица из массива объектов
console.group('Загрузка данных');
console.log('franchises:', franchises.length);
console.log('music:', music.length);
console.groupEnd();
console.time('filter');
const result = filterFranchises(franchises, filters);
console.timeEnd('filter');         // filter: 2.3ms
console.dir(element);              // DOM-элемент как объект
```

### Network — отследить запросы

```
Что смотреть в Network:
─────────────────────────────────────────────────────────
Name    — имя файла/URL запроса
Status  — 200 (ок), 404 (не найдено), 500 (ошибка сервера)
Type    — fetch, xhr, js, css, img
Size    — размер (важно для оптимизации)
Time    — сколько ждали ответа

Как использовать:
• Фильтр Fetch/XHR — только API-запросы
• Отключить кеш: ☑ Disable cache (пока открыты DevTools)
• Имитировать медленный интернет: выпадалка "No throttling" → Slow 3G
• Кликнуть запрос → вкладка Response — увидеть данные
• Кликнуть запрос → вкладка Headers — увидеть заголовки
```

---

## 19. Что JS-разработчику делать не нужно

```
ЧТО ДЕЛАЕТ РАЗРАБОТЧИК:              ЧТО ДЕЛАЕТ КТО-ТО ДРУГОЙ:
────────────────────────              ──────────────────────────
Логика фильтрации                ←→   UX фильтров (дизайнер)
Работа с данными                 ←→   Структура БД (бэкенд/DBA)
DOM-манипуляции                  ←→   Серверный рендеринг (бэкенд)
Обработка форм                   ←→   Серверная валидация (бэкенд)

ЧТО НЕ НУЖНО ДЕЛАТЬ В JS:
──────────────────────────────────────────────────────────────
× innerHTML с пользовательскими данными — XSS
× Хранить пароли/токены в localStorage
× Использовать var вместо const/let
× Мутировать данные напрямую без нужды
× Писать синхронный тяжёлый код без чанкования
× document.write() — устаревший метод
× eval() — выполняет строку как код — опасно
```

---

## 20. Проект: живой аниме-каталог

Финальная сборка всех модулей в работающий каталог.

### `js/data.js` — загрузка и обработка данных

```js
// Загрузить данные с сервера
export async function loadData() {
  const [franchises, music] = await Promise.all([
    fetch('/data/franchises.json').then(r => r.json()),
    fetch('/data/music.json').then(r => r.json()),
  ]);
  return { franchises, music };
}

// Фильтрация — чистая функция (нет side effects)
export function filterFranchises(franchises, filters) {
  return franchises
    .filter(f => filters.status === 'all' || f.status === filters.status)
    .filter(f => f.rating >= (filters.minRating || 0))
    .filter(f => {
      if (!filters.genres?.length) return true;
      return filters.genres.every(g => f.genres.includes(g));
    })
    .toSorted((a, b) => {
      if (filters.sortBy === 'rating') return b.rating - a.rating;
      if (filters.sortBy === 'year') return b.year_start - a.year_start;
      return a.title.localeCompare(b.title, 'ru');
    });
}

// Статистика для дашборда
export function calcStats(franchises, music) {
  return {
    total: franchises.length,
    ongoing: franchises.filter(f => f.status === 'ongoing').length,
    completed: franchises.filter(f => f.status === 'completed').length,
    avgRating: (franchises.reduce((s, f) => s + f.rating, 0) / franchises.length).toFixed(1),
    totalTracks: music.length,
    openings: music.filter(t => t.type === 'opening').length,
    endings: music.filter(t => t.type === 'ending').length,
  };
}
```

### `js/store.js` — реактивное состояние

```js
function createStore(initial) {
  let state = structuredClone(initial);
  const listeners = new Set();

  return {
    getState: () => state,
    setState(patch) {
      state = { ...state, ...patch };
      listeners.forEach(fn => fn(state));
    },
    subscribe(fn) {
      listeners.add(fn);
      return () => listeners.delete(fn);
    },
  };
}

export const store = createStore({
  franchises: [],
  filtered: [],
  music: [],
  filters: { status: 'all', genres: [], minRating: 0, sortBy: 'title' },
  favorites: [],
  loading: true,
  error: null,
});
```

### `js/render.js` — DOM из данных

```js
import { store } from './store.js';

export function renderCatalog(franchises) {
  const grid = document.querySelector('.catalog__grid');

  if (!franchises.length) {
    grid.innerHTML = '<p class="catalog__empty">По вашему запросу ничего не найдено</p>';
    return;
  }

  const fragment = document.createDocumentFragment();
  const favorites = store.getState().favorites;

  franchises.forEach(franchise => {
    fragment.appendChild(createCard(franchise, favorites.includes(franchise.id)));
  });

  grid.replaceChildren(fragment);  // заменить всё содержимое
}

function createCard(franchise, isFavorite) {
  const tpl = document.getElementById('card-template');
  const clone = tpl.content.cloneNode(true);
  const article = clone.querySelector('article');

  article.dataset.id = franchise.id;
  article.dataset.status = franchise.status;
  article.dataset.genres = franchise.genres.join(',');
  if (isFavorite) article.classList.add('card--favorite');

  clone.querySelector('.card__cover-link').href =
    `/pages/franchise/?id=${franchise.id}`;
  clone.querySelector('.card__cover').src = franchise.cover;
  clone.querySelector('.card__cover').alt = `${franchise.title} — постер`;
  clone.querySelector('.card__title a').textContent = franchise.title;
  clone.querySelector('.card__title a').href =
    `/pages/franchise/?id=${franchise.id}`;
  clone.querySelector('.card__subtitle').textContent = franchise.original_title;
  clone.querySelector('.card__rating').textContent = franchise.rating;

  const statusEl = clone.querySelector('.card__status');
  statusEl.textContent = franchise.status === 'ongoing' ? 'Выходит' : 'Завершено';
  statusEl.className = `card__status card__status--${franchise.status}`;

  const genresList = clone.querySelector('.card__genres');
  franchise.genres.forEach(genre => {
    const li = document.createElement('li');
    li.className = 'genre-tag';
    li.dataset.genre = genre;
    li.textContent = genre;
    genresList.appendChild(li);
  });

  return clone;
}

export function updateStats(stats) {
  document.getElementById('stat-franchises').textContent = stats.total;
  document.getElementById('stat-tracks').textContent = stats.totalTracks;
  document.getElementById('stat-rating').textContent = stats.avgRating;
}
```

### `js/filter.js` — логика фильтров

```js
import { store } from './store.js';
import { filterFranchises } from './data.js';

export function initFilters() {
  // Фильтр по статусу (радио-кнопки)
  document.querySelectorAll('[name="status"]').forEach(radio => {
    radio.addEventListener('change', e => {
      const { franchises, filters } = store.getState();
      const newFilters = { ...filters, status: e.target.value };
      store.setState({
        filters: newFilters,
        filtered: filterFranchises(franchises, newFilters),
      });
    });
  });

  // Фильтр по жанрам (чекбоксы)
  document.querySelectorAll('[name="genre"]').forEach(cb => {
    cb.addEventListener('change', () => {
      const checked = [...document.querySelectorAll('[name="genre"]:checked')]
        .map(el => el.value);
      const { franchises, filters } = store.getState();
      const newFilters = { ...filters, genres: checked };
      store.setState({
        filters: newFilters,
        filtered: filterFranchises(franchises, newFilters),
      });
    });
  });

  // Сортировка
  document.querySelector('[name="sortBy"]')?.addEventListener('change', e => {
    const { franchises, filters } = store.getState();
    const newFilters = { ...filters, sortBy: e.target.value };
    store.setState({
      filters: newFilters,
      filtered: filterFranchises(franchises, newFilters),
    });
  });
}
```

### `js/favorites.js` — избранное с localStorage

```js
import { store } from './store.js';

const LS_KEY = 'anime-catalog-favorites';

// Загрузить из localStorage при старте
export function loadFavorites() {
  try {
    const raw = localStorage.getItem(LS_KEY);
    return raw ? JSON.parse(raw) : [];
  } catch {
    return [];
  }
}

// Сохранить при изменении
function saveFavorites(favorites) {
  try {
    localStorage.setItem(LS_KEY, JSON.stringify(favorites));
  } catch (e) {
    console.warn('Не удалось сохранить избранное:', e);
  }
}

export function toggleFavorite(id) {
  const { favorites } = store.getState();
  const newFavorites = favorites.includes(id)
    ? favorites.filter(f => f !== id)
    : [...favorites, id];

  store.setState({ favorites: newFavorites });
  saveFavorites(newFavorites);
}

export function initFavoriteButtons() {
  document.addEventListener('click', e => {
    const btn = e.target.closest('[data-action="toggle-favorite"]');
    if (!btn) return;
    const id = btn.closest('[data-id]')?.dataset.id;
    if (id) toggleFavorite(id);
  });
}
```

### `js/app.js` — точка входа

```js
import { loadData, filterFranchises, calcStats } from './data.js';
import { store } from './store.js';
import { renderCatalog, updateStats } from './render.js';
import { initFilters } from './filter.js';
import { loadFavorites, initFavoriteButtons } from './favorites.js';

async function init() {
  // Подписаться на изменения состояния — UI обновится автоматически
  store.subscribe(state => {
    if (!state.loading) {
      renderCatalog(state.filtered);
    }
  });

  // Загрузить данные
  try {
    const { franchises, music } = await loadData();
    const favorites = loadFavorites();
    const filtered = filterFranchises(franchises, store.getState().filters);
    const stats = calcStats(franchises, music);

    store.setState({ franchises, music, filtered, favorites, loading: false });
    updateStats(stats);
  } catch (error) {
    console.error('Ошибка инициализации:', error);
    store.setState({ loading: false, error: error.message });
    document.querySelector('.catalog__grid').innerHTML =
      '<p class="catalog__error">Не удалось загрузить каталог. Обновите страницу.</p>';
  }

  // Инициализировать взаимодействия
  initFilters();
  initFavoriteButtons();
}

init();
```

---

## Итого: что умеете после части 3

```
✓ Понимать типы данных и почему === важнее ==
✓ Писать на const/let, объяснить почему не var
✓ Объяснить замыкание с примером
✓ Загружать данные из JSON через fetch/async-await
✓ Генерировать DOM из данных (шаблонизация)
✓ Делегировать события
✓ Фильтровать данные через map/filter/reduce
✓ Строить реактивный store
✓ Работать с localStorage
✓ Писать модули (import/export)
✓ Объяснить Event Loop и порядок microtask/macrotask
✓ Понимать зачем нужны фреймворки и бандлеры
✓ Обрабатывать ошибки правильно

Следующий шаг — Часть 4: Fullstack.
Добавляем Node.js сервер, реальную БД, деплой.
```

---

> **Вопросы для самопроверки**: [questions/03-js-questions.md](./questions/03-js-questions.md) — 106 вопросов от типов данных до Virtual DOM
