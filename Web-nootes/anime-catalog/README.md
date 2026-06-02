# Anime Catalog — учебный проект

Сквозной проект через все 4 части курса.
Каждая часть добавляет новый слой поверх предыдущего.

---

## Структура

```
anime-catalog/
│
├── index.html                  ← Главная: сетка франшиз
│
├── pages/
│   ├── franchise/
│   │   └── index.html          ← Страница одной франшизы (Naruto, AoT...)
│   ├── titles/
│   │   └── index.html          ← Список сезонов/тайтлов франшизы
│   ├── music/
│   │   └── index.html          ← Опенинги, эндинги, каверы
│   └── admin/
│       └── index.html          ← Дашборд (заглушка → реальные данные в части 4)
│
├── css/
│   ├── reset.css               ← modern-normalize
│   ├── variables.css           ← CSS custom properties (цвета, отступы, шрифты)
│   ├── layout.css              ← Сетки и общие layout-паттерны
│   ├── components.css          ← Карточки, кнопки, навигация
│   └── pages/
│       ├── home.css
│       ├── franchise.css
│       ├── music.css
│       └── admin.css
│
├── js/
│   ├── data.js                 ← Загрузка JSON, работа с данными
│   ├── render.js               ← Шаблонизация: JSON → DOM-элементы
│   ├── filter.js               ← Фильтрация каталога
│   ├── router.js               ← Простой роутер (часть 3)
│   └── admin.js                ← Логика дашборда
│
├── data/
│   ├── franchises.json         ← Список франшиз с метаданными
│   ├── titles.json             ← Тайтлы по франшизам
│   ├── characters.json         ← Персонажи
│   └── music.json              ← Опенинги, эндинги, каверы
│
└── assets/
    ├── img/
    │   ├── covers/             ← Постеры франшиз
    │   └── characters/         ← Арты персонажей
    └── fonts/
```

---

## Что добавляет каждая часть курса

| Часть | Что делаем |
|---|---|
| **1 — HTML** | Разметка всех страниц. Семантика, формы, доступность. Без стилей — голый скелет |
| **2 — CSS** | Стилизация. Переменные, Flexbox, Grid, адаптив, анимации карточек |
| **3 — JS** | Данные из JSON, фильтрация, генерация страниц, localStorage, admin-заглушка |
| **4 — Fullstack** | Node.js API, реальная БД, авторизация в admin, деплой |

---

## Данные

Используем реальные аниме-франшизы: Attack on Titan, Demon Slayer, Fullmetal Alchemist,
Naruto, One Piece, Jujutsu Kaisen, Vinland Saga, Chainsaw Man.

Данные хранятся в JSON — в частях 1-3 это «база данных».
В части 4 JSON мигрирует в SQLite → потом объясняем PostgreSQL/MongoDB.
