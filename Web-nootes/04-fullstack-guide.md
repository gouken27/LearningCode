# Часть 4 — Fullstack: собираем всё вместе

> **Учебный проект**: финальная сборка [anime-catalog/](./project/anime-catalog/) — добавляем Node.js сервер, реальную БД, авторизацию в admin, деплой  
> **Вопросы для самопроверки**: [questions/04-fullstack-questions.md](./questions/04-fullstack-questions.md)

---

## Содержание

1. [Что такое сервер, бэкенд и фронтенд](#1-что-такое-сервер-бэкенд-и-фронтенд)
2. [Компиляция, интерпретация, парсинг — что происходит с кодом](#2-компиляция-интерпретация-парсинг)
3. [Установка Node.js на разные системы](#3-установка-nodejs)
4. [Node.js — JS вне браузера](#4-nodejs--js-вне-браузера)
5. [npm, package.json, node_modules](#5-npm-packagejson-node_modules)
6. [HTTP: как браузер разговаривает с сервером](#6-http-как-браузер-разговаривает-с-сервером)
7. [Что такое API и REST](#7-что-такое-api-и-rest)
8. [Express.js: минимальный сервер](#8-expressjs-минимальный-сервер)
9. [Middleware: конвейер обработки запросов](#9-middleware-конвейер-обработки-запросов)
10. [Базы данных: от JSON до PostgreSQL](#10-базы-данных-от-json-до-postgresql)
11. [SQL: основы языка запросов](#11-sql-основы-языка-запросов)
12. [Аутентификация и авторизация](#12-аутентификация-и-авторизация)
13. [Переменные окружения и конфигурация](#13-переменные-окружения-и-конфигурация)
14. [CORS и безопасность API](#14-cors-и-безопасность-api)
15. [Сборка: структура fullstack-проекта с Vite](#15-сборка-структура-fullstack-проекта)
16. [Деплой: Netlify, Railway, переменные на проде](#16-деплой)
17. [DevTools: вкладка Network в глубину](#17-devtools-network-в-глубину)
18. [Что fullstack-джуну делать не нужно](#18-что-fullstack-джуну-делать-не-нужно)
19. [Проект: полная сборка аниме-каталога](#19-проект-полная-сборка)

---

## 1. Что такое сервер, бэкенд и фронтенд

### Где живёт код

До сих пор весь наш код выполнялся в браузере на компьютере пользователя. Теперь добавляется вторая сторона — сервер.

```
┌────────────────────────────────────────────────────────────────────┐
│                  ДВЕ СТОРОНЫ ВЕБ-ПРИЛОЖЕНИЯ                        │
│                                                                    │
│  ФРОНТЕНД                          БЭКЕНД                          │
│  (клиент)                          (сервер)                        │
│  ────────────────────              ──────────────────────          │
│  Браузер пользователя              Компьютер в дата-центре         │
│  HTML, CSS, JavaScript             Node.js, Python, Go, Java...    │
│                                                                    │
│  Что делает:                       Что делает:                     │
│  • Рисует интерфейс                • Хранит данные в БД            │
│  • Реагирует на клики              • Проверяет права доступа       │
│  • Отправляет запросы              • Обрабатывает бизнес-логику     │
│  • Показывает результаты           • Отвечает на запросы           │
│                                                                    │
│  Виден пользователю                Скрыт от пользователя           │
│  Код можно открыть в DevTools      Код закрыт                      │
│                                                                    │
│  ══════════════════════════════════════════════════════════════    │
│                         HTTP-запросы                               │
│  Браузер ──── GET /api/franchises ─────────────────► Сервер        │
│  Браузер ◄─── { franchises: [...] } ───────────────── Сервер       │
└────────────────────────────────────────────────────────────────────┘
```

### Что такое сервер

Сервер — это программа (или компьютер), которая **постоянно ждёт запросов** и отвечает на них. Физически это может быть:

- Обычный компьютер в вашей сети
- Виртуальная машина в облаке (AWS, Google Cloud, DigitalOcean)
- Контейнер Docker
- Serverless-функция (запускается только при запросе)

В нашем случае сервер — это **Node.js-процесс**, который слушает на порту (например, `3000`) и отвечает на HTTP-запросы.

### Фронтенд vs бэкенд: что разработчику обязательно знать о другой стороне

```
ФРОНТЕНД разработчик знает о бэкенде:
  ✓ Как устроен HTTP-запрос (метод, URL, заголовки, тело)
  ✓ Что такое REST API и как с ним работать
  ✓ Форматы ответа (JSON, статус-коды)
  ✓ Что такое CORS и почему он мешает
  ✓ Базовую структуру БД (чтобы понять что можно просить от API)

БЭКЕНД разработчик знает о фронтенде:
  ✓ Как браузер отправляет запросы
  ✓ Что фронтенд ожидает в ответе (формат, структура)
  ✓ Базовую безопасность (XSS, CSRF)
  ✓ CORS — потому что именно бэкенд его настраивает

FULLSTACK — умеет и то, и другое на базовом уровне.
```

---

## 2. Компиляция, интерпретация, парсинг

Прежде чем запускать код — компьютер должен его понять. Вот как это происходит.

### Парсинг — «разобрать текст по правилам»

**Парсер (parser)** — программа, которая читает текст и строит из него структурированное представление. Браузер парсит HTML → строит DOM. V8 парсирует JS → строит AST.

```
Исходный текст:           После парсинга (AST):
──────────────            ──────────────────────────
const x = 2 + 3;   →     VariableDeclaration
                            VariableDeclarator
                              Identifier: x
                              BinaryExpression: +
                                Literal: 2
                                Literal: 3
```

**AST (Abstract Syntax Tree)** — абстрактное синтаксическое дерево. Именно с ним работают компиляторы, линтеры, транспиляторы, Prettier.

### Компиляция vs интерпретация

```
┌────────────────────────────────────────────────────────────────┐
│                                                                │
│  КОМПИЛЯЦИЯ                    ИНТЕРПРЕТАЦИЯ                   │
│  ──────────────────────        ──────────────────────────────  │
│  Код → (компилятор) → машинный Код выполняется строка за       │
│  код → запустить               строкой специальной программой  │
│                                (интерпретатором)               │
│  Примеры: C, C++, Go, Rust     Примеры: Python, Ruby,          │
│                                ранний JS                       │
│  + Быстрее при выполнении      + Проще отлаживать              │
│  + Оптимизирован под железо    + Не нужна сборка               │
│  - Нужно компилировать         - Медленнее                     │
│    под каждую ОС                                               │
│                                                                │
│  JIT-КОМПИЛЯЦИЯ (Just-In-Time) — современный подход            │
│  ──────────────────────────────────────────────────────────    │
│  Интерпретирует + компилирует «горячие» участки на лету        │
│  Примеры: V8 (JS в Chrome и Node), JVM (Java), .NET (C#)       │
│  + Скорость близка к компилируемым языкам                      │
│  + Не нужна ручная компиляция                                  │
└────────────────────────────────────────────────────────────────┘
```

### Что происходит с JavaScript в V8

```
Ваш .js файл
     ↓
  Парсинг → AST
     ↓
  Ignition (интерпретатор) → выполняет байткод
     ↓
  Profiler отслеживает «горячие» функции (вызываются часто)
     ↓
  TurboFan (JIT-компилятор) → компилирует горячие функции
     ↓
  Машинный код → выполняется напрямую на CPU
```

Именно поэтому JS ускоряется со временем выполнения — чем дольше работает, тем больше скомпилировано.

### Транспиляция — особый вид компиляции

**Транспилятор** переводит из одного языка/версии в другой:

```
TypeScript  ──────────────────────────────►  JavaScript
JSX (React) ──────────────────────────────►  JavaScript
ES2024 JS   ──────────────────────────────►  ES5 JS (для старых браузеров)
SCSS        ──────────────────────────────►  CSS
```

Инструменты: Babel (JS→JS), tsc (TS→JS), Vite внутри использует esbuild.

---

## 3. Установка Node.js

### Что такое Node.js (кратко)

Node.js — это **среда выполнения JavaScript вне браузера**. Внутри — движок V8 (тот же что в Chrome) + библиотеки для работы с файловой системой, сетью, процессами.

### Способы установки

**Рекомендованный способ для всех платформ — nvm (Node Version Manager)**

nvm позволяет держать несколько версий Node и переключаться между ними. Критично для реальных проектов — один требует Node 18, другой Node 20.

---

#### macOS / Linux

```bash
# 1. Установить nvm
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.7/install.sh | bash

# 2. Перезапустить терминал или выполнить:
source ~/.bashrc   # bash
source ~/.zshrc    # zsh (macOS по умолчанию)

# 3. Установить LTS-версию Node (Long Term Support — стабильная)
nvm install --lts

# 4. Сделать её дефолтной
nvm alias default node

# 5. Проверить
node --version   # v22.x.x
npm --version    # 10.x.x
```

---

#### Windows — вариант 1: nvm-windows (рекомендуется)

```powershell
# 1. Скачать nvm-setup.exe с:
#    https://github.com/coreybutler/nvm-windows/releases
# 2. Установить (требует права администратора)

# 3. В PowerShell (от администратора):
nvm install lts
nvm use lts

# 4. Проверить
node --version
npm --version
```

---

#### Windows — вариант 2: официальный установщик (проще, но без версионирования)

```
1. Зайти на https://nodejs.org
2. Скачать "LTS" версию (не Current)
3. Запустить .msi-установщик
4. Что устанавливается:
   ✓ node.exe       — сам Node.js
   ✓ npm            — менеджер пакетов
   ✓ npm в PATH     — команды доступны в любом терминале
   ✓ (опционально) Chocolatey и build tools для нативных модулей
5. Проверить в PowerShell:
   node --version
   npm --version
```

---

#### Windows — вариант 3: через winget (Windows Package Manager)

```powershell
winget install OpenJS.NodeJS.LTS
# Перезапустить терминал после установки
node --version
```

---

#### Linux (Ubuntu/Debian) — альтернатива nvm

```bash
# Через NodeSource (официальный репозиторий для Ubuntu)
curl -fsSL https://deb.nodesource.com/setup_lts.x | sudo -E bash -
sudo apt-get install -y nodejs

# Или через snap
sudo snap install node --classic --channel=lts/stable
```

---

### Что такое LTS vs Current

```
LTS (Long Term Support) — чётные версии: 18, 20, 22
  • Поддерживается 3 года
  • Стабильная, проверенная
  • Для продакшена и обучения — берите LTS

Current — нечётные версии: 19, 21, 23
  • Новейшие функции
  • Короткая поддержка
  • Для экспериментов
```

### Проверка установки и первый запуск

```bash
# Версии
node --version     # v22.11.0
npm --version      # 10.9.0

# Запустить JS-файл
node hello.js

# Интерактивный REPL (Read-Eval-Print Loop) — консоль Node
node
> 2 + 2
4
> console.log('hello')
hello
> .exit

# Запустить скрипт из package.json
npm run dev
npm start
npm test
```

---

## 4. Node.js — JS вне браузера

### Что есть в Node, чего нет в браузере

```
┌────────────────────────────────────────────────────────────────┐
│                 NODE.JS vs БРАУЗЕР                             │
│                                                                │
│  Есть в обоих:          Только Node:       Только браузер:     │
│  ────────────           ────────────       ────────────────    │
│  V8 движок              fs (файлы)         DOM                 │
│  setTimeout/interval    path               window              │
│  Promise, async/await   http/https         document            │
│  ES Modules             process            navigator           │
│  fetch (Node 18+)       Buffer             localStorage        │
│  console                child_process      fetch (ранее)       │
│  crypto                 os, cluster        history             │
│                         npm-экосистема     cookies             │
└────────────────────────────────────────────────────────────────┘
```

### `process` — информация о текущем процессе

```js
process.env.NODE_ENV       // 'development' или 'production'
process.env.PORT           // порт из переменных окружения
process.argv               // аргументы командной строки
process.cwd()              // текущая рабочая директория
process.exit(0)            // завершить процесс (0 = успех)
process.exit(1)            // завершить с ошибкой
```

### Работа с файловой системой (fs)

```js
import { readFile, writeFile, readdir } from 'fs/promises';

// Прочитать JSON-файл
const raw = await readFile('./data/franchises.json', 'utf-8');
const franchises = JSON.parse(raw);

// Записать
await writeFile('./data/franchises.json', JSON.stringify(franchises, null, 2));

// Список файлов
const files = await readdir('./data');
// ['franchises.json', 'music.json', 'characters.json']
```

### Модульная система Node

```js
// ESM (современный стандарт — используем его)
import express from 'express';
import { readFile } from 'fs/promises';
export const PORT = 3000;

// В package.json нужно:
// "type": "module"

// CommonJS (старый, но встречается повсюду)
const express = require('express');
module.exports = { PORT: 3000 };
```

---

## 5. npm, package.json, node_modules

### npm — менеджер пакетов

npm (Node Package Manager) — это два в одном:
1. Инструмент командной строки для установки пакетов
2. Реестр (registry) с 2+ млн пакетов на npmjs.com

```bash
# Инициализировать проект (создаст package.json)
npm init -y

# Установить пакет (добавится в dependencies)
npm install express

# Установить только для разработки (не пойдёт на прод)
npm install --save-dev nodemon

# Установить конкретную версию
npm install express@4.18.2

# Удалить
npm uninstall express

# Обновить все пакеты
npm update

# Запустить скрипт из package.json
npm run dev
npm run build
npm start
npm test  # сокращение от npm run test
```

### Анатомия `package.json`

```json
{
  "name": "anime-catalog-backend",
  "version": "1.0.0",
  "type": "module",

  "scripts": {
    "dev":   "nodemon src/server.js",
    "start": "node src/server.js",
    "build": "echo 'No build step for backend'"
  },

  "dependencies": {
    "express":      "^4.18.2",
    "better-sqlite3": "^9.4.3",
    "bcryptjs":     "^2.4.3",
    "jsonwebtoken": "^9.0.2",
    "cors":         "^2.8.5",
    "dotenv":       "^16.3.1"
  },

  "devDependencies": {
    "nodemon": "^3.0.2"
  },

  "engines": {
    "node": ">=18.0.0"
  }
}
```

```
dependencies    — нужны для работы в продакшене
                  express, cors, database driver...

devDependencies — только для разработки, не ставятся на прод
                  nodemon (автоперезапуск), jest (тесты), eslint...

Версии:
  "^4.18.2"   — caret: 4.x.x (минорные и патч-обновления)
  "~4.18.2"   — tilde: 4.18.x (только патч-обновления)
  "4.18.2"    — точная версия
  "*"         — любая (опасно)
```

### `node_modules` и `package-lock.json`

```
node_modules/
  Папка с установленными пакетами.
  Может весить 500MB+.
  НИКОГДА не коммитить в Git — добавить в .gitignore.
  На другом компьютере: npm install воссоздаст её из package.json.

package-lock.json
  Точные версии ВСЕХ зависимостей (включая зависимости зависимостей).
  ВСЕГДА коммитить в Git.
  Гарантирует: все разработчики и прод-сервер используют
  одинаковые версии пакетов.
  npm install с lock-файлом = воспроизводимая сборка.
```

```bash
# .gitignore
node_modules/
.env
dist/
```

---

## 6. HTTP: как браузер разговаривает с сервером

### Полный путь запроса

```
1. Пользователь открывает anime-catalog.dev
2. Браузер → DNS-сервер: «какой IP у anime-catalog.dev?»
3. DNS отвечает: «104.21.64.0»
4. Браузер → Сервер 104.21.64.0:
   GET / HTTP/1.1
   Host: anime-catalog.dev
   Accept: text/html
5. Сервер отвечает:
   HTTP/1.1 200 OK
   Content-Type: text/html
   <html>...</html>
6. Браузер рендерит страницу
7. Встречает <link rel="stylesheet" href="/css/main.css">
8. Повторяет шаги 4–5 для CSS, JS, картинок...
```

### Анатомия HTTP-запроса

```
POST /api/franchises HTTP/1.1
Host: anime-catalog.dev
Content-Type: application/json
Authorization: Bearer eyJhbGc...
Accept: application/json

{"title": "Vinland Saga", "rating": 8.8}

──────────────────────────────────────────────────────
Строка запроса:  МЕТОД  /путь  ВЕРСИЯ-HTTP
Заголовки:       Ключ: Значение
Пустая строка:   разделяет заголовки и тело
Тело:            данные (для POST/PUT/PATCH)
```

### HTTP-методы — семантика важна

```
GET     /api/franchises        — получить список
GET     /api/franchises/aot    — получить одну запись
POST    /api/franchises        — создать новую
PUT     /api/franchises/aot    — заменить целиком
PATCH   /api/franchises/aot    — обновить частично
DELETE  /api/franchises/aot    — удалить

Идемпотентность — одинаковый результат при повторных вызовах:
  GET, PUT, DELETE, HEAD — идемпотентны
  POST — НЕ идемпотентен (каждый вызов создаёт новую запись)
  PATCH — формально не идемпотентен
```

### HTTP-статус-коды

```
2xx — Успех
  200 OK              — стандартный успех
  201 Created         — ресурс создан (ответ на POST)
  204 No Content      — успех, тела нет (ответ на DELETE)

3xx — Перенаправление
  301 Moved Permanently  — постоянный редирект (меняет закладки)
  302 Found              — временный редирект
  304 Not Modified       — используй кеш браузера

4xx — Ошибка клиента (запрос неверный)
  400 Bad Request        — неверный формат запроса
  401 Unauthorized       — нужна аутентификация
  403 Forbidden          — аутентифицирован, но нет прав
  404 Not Found          — ресурс не найден
  409 Conflict           — конфликт (дубликат)
  422 Unprocessable      — данные не прошли валидацию
  429 Too Many Requests  — превышен rate limit

5xx — Ошибка сервера
  500 Internal Server Error — что-то упало на сервере
  502 Bad Gateway           — проблема между серверами
  503 Service Unavailable   — сервер перегружен/на тех. работах
```

### Заголовки — метаданные запроса/ответа

```
Запрос → Сервер:
  Content-Type: application/json    — тип тела запроса
  Authorization: Bearer <token>     — токен аутентификации
  Accept: application/json          — какой формат ответа хочу
  Cookie: session=abc123            — куки
  Origin: https://mysite.com        — откуда запрос (CORS)

Сервер → Ответ:
  Content-Type: application/json    — тип тела ответа
  Set-Cookie: session=abc; HttpOnly — установить куки
  Access-Control-Allow-Origin: *    — разрешить CORS
  Cache-Control: max-age=3600       — кешировать 1 час
  X-RateLimit-Remaining: 42         — кастомный заголовок
```

---

## 7. Что такое API и REST

### API — интерфейс для программ

**API (Application Programming Interface)** — набор правил, по которым одна программа общается с другой. Это «договор»: «если пришлёшь такой запрос — получишь такой ответ».

```
Аналогия с рестораном:
  Меню = API (список доступных операций)
  Официант = HTTP (протокол передачи)
  Заказ = запрос (request)
  Блюдо = ответ (response)
  Кухня = сервер (скрыта от гостя)
```

### REST — архитектурный стиль API

**REST (Representational State Transfer)** — не протокол, не стандарт, а набор принципов проектирования API:

```
1. Клиент-сервер        — клиент и сервер независимы
2. Stateless            — каждый запрос содержит всё необходимое,
                          сервер не хранит состояние сессии между запросами
3. Кешируемость         — ответы можно кешировать
4. Единый интерфейс     — URL = ресурс, метод = действие
5. Слоистость           — клиент не знает напрямую ли он говорит с сервером
```

### CRUD → HTTP методы → URL-паттерн

```
Операция    HTTP-метод  URL                         Что делает
──────────  ──────────  ──────────────────────────  ────────────────
Create      POST        /api/franchises             Создать франшизу
Read all    GET         /api/franchises             Список всех
Read one    GET         /api/franchises/aot         Одна по id
Update all  PUT         /api/franchises/aot         Заменить целиком
Update part PATCH       /api/franchises/aot         Обновить поля
Delete      DELETE      /api/franchises/aot         Удалить
```

### REST vs GraphQL

```
REST                              GraphQL
──────────────────────────        ──────────────────────────────────
Много эндпоинтов                  Один эндпоинт /graphql
Сервер решает что вернуть         Клиент точно описывает что хочет
Overfetch (лишние поля)           Только нужные поля
Underfetch (нужно N запросов)     Всё в одном запросе
Просто, понятно, везде            Мощнее, но сложнее

Когда REST:  публичный API, простые операции CRUD, небольшая команда
Когда GraphQL: сложные связанные данные, мобильное приложение
```

---

## 8. Express.js: минимальный сервер

### Создать первый сервер

```bash
mkdir anime-catalog-backend
cd anime-catalog-backend
npm init -y
npm install express cors dotenv
npm install --save-dev nodemon
```

```json
// package.json — добавить:
{
  "type": "module",
  "scripts": {
    "dev": "nodemon src/server.js",
    "start": "node src/server.js"
  }
}
```

```js
// src/server.js
import express from 'express';
import cors from 'cors';

const app = express();
const PORT = process.env.PORT || 3000;

// Middleware
app.use(cors());                    // разрешить запросы с фронтенда
app.use(express.json());            // парсить JSON в теле запроса

// Роуты
app.get('/', (req, res) => {
  res.json({ message: 'Anime Catalog API', version: '1.0' });
});

// Запустить
app.listen(PORT, () => {
  console.log(`Сервер запущен: http://localhost:${PORT}`);
});
```

```bash
npm run dev
# nodemon watching...
# Сервер запущен: http://localhost:3000
```

### Роутинг — карта URL-адресов

```js
// src/routes/franchises.js
import { Router } from 'express';
import { readFile, writeFile } from 'fs/promises';

const router = Router();
const DB_PATH = './data/franchises.json';

// Вспомогательная функция чтения «БД»
async function readDB() {
  const raw = await readFile(DB_PATH, 'utf-8');
  return JSON.parse(raw);
}

// GET /api/franchises — список с фильтрацией
router.get('/', async (req, res) => {
  try {
    const franchises = await readDB();

    // Фильтрация по query-параметрам: /api/franchises?status=ongoing&genre=action
    let result = franchises;

    if (req.query.status && req.query.status !== 'all') {
      result = result.filter(f => f.status === req.query.status);
    }

    if (req.query.genre) {
      result = result.filter(f => f.genres.includes(req.query.genre));
    }

    if (req.query.minRating) {
      result = result.filter(f => f.rating >= parseFloat(req.query.minRating));
    }

    res.json(result);
  } catch (error) {
    res.status(500).json({ error: 'Не удалось загрузить список' });
  }
});

// GET /api/franchises/:id — одна запись
router.get('/:id', async (req, res) => {
  try {
    const franchises = await readDB();
    const franchise = franchises.find(f => f.id === req.params.id);

    if (!franchise) {
      return res.status(404).json({ error: `Франшиза '${req.params.id}' не найдена` });
    }

    res.json(franchise);
  } catch (error) {
    res.status(500).json({ error: 'Ошибка сервера' });
  }
});

// POST /api/franchises — создать (только для admin)
router.post('/', async (req, res) => {
  try {
    const { title, original_title, studio, year_start, genres, rating } = req.body;

    // Валидация
    if (!title) {
      return res.status(422).json({ error: 'Поле title обязательно' });
    }

    const franchises = await readDB();

    const newFranchise = {
      id: title.toLowerCase().replace(/\s+/g, '-'),
      title,
      original_title: original_title || title,
      studio: studio || '',
      year_start: year_start || new Date().getFullYear(),
      year_end: null,
      status: 'ongoing',
      genres: genres || [],
      rating: rating || 0,
      cover: '/assets/img/covers/placeholder.jpg',
      description: '',
      seasons_count: 0,
      episodes_total: 0,
    };

    franchises.push(newFranchise);
    await writeFile(DB_PATH, JSON.stringify(franchises, null, 2));

    res.status(201).json(newFranchise);
  } catch (error) {
    res.status(500).json({ error: 'Не удалось создать запись' });
  }
});

// PATCH /api/franchises/:id — обновить частично
router.patch('/:id', async (req, res) => {
  try {
    const franchises = await readDB();
    const idx = franchises.findIndex(f => f.id === req.params.id);

    if (idx === -1) {
      return res.status(404).json({ error: 'Не найдено' });
    }

    franchises[idx] = { ...franchises[idx], ...req.body };
    await writeFile(DB_PATH, JSON.stringify(franchises, null, 2));

    res.json(franchises[idx]);
  } catch (error) {
    res.status(500).json({ error: 'Ошибка обновления' });
  }
});

// DELETE /api/franchises/:id
router.delete('/:id', async (req, res) => {
  try {
    const franchises = await readDB();
    const filtered = franchises.filter(f => f.id !== req.params.id);

    if (filtered.length === franchises.length) {
      return res.status(404).json({ error: 'Не найдено' });
    }

    await writeFile(DB_PATH, JSON.stringify(filtered, null, 2));
    res.status(204).send();
  } catch (error) {
    res.status(500).json({ error: 'Ошибка удаления' });
  }
});

export default router;
```

```js
// src/server.js — подключить роутер
import franchisesRouter from './routes/franchises.js';
import musicRouter from './routes/music.js';

app.use('/api/franchises', franchisesRouter);
app.use('/api/music', musicRouter);
```

---

## 9. Middleware: конвейер обработки запросов

### Что такое middleware

Middleware — функция, которая стоит между входящим запросом и финальным обработчиком. Каждая вызывает `next()` чтобы передать управление следующей.

```
Запрос → [cors] → [json парсер] → [логгер] → [auth] → [роут] → Ответ

app.use((req, res, next) => {
  // Сделать что-то с req/res
  next();  // передать дальше
  // Если не вызвать next() — запрос зависнет
});
```

### Примеры middleware

```js
// 1. Логирование всех запросов
app.use((req, res, next) => {
  const start = Date.now();
  res.on('finish', () => {
    console.log(`${req.method} ${req.url} ${res.statusCode} ${Date.now()-start}ms`);
  });
  next();
});

// 2. Проверка API-ключа
function requireApiKey(req, res, next) {
  const key = req.headers['x-api-key'];
  if (key !== process.env.API_KEY) {
    return res.status(401).json({ error: 'Неверный API-ключ' });
  }
  next();
}
app.use('/api/admin', requireApiKey);

// 3. Обработчик ошибок — 4 параметра!
app.use((err, req, res, next) => {
  console.error(err.stack);
  res.status(err.status || 500).json({
    error: err.message || 'Внутренняя ошибка сервера',
  });
});
```

### Популярные middleware-пакеты

```js
import cors from 'cors';        // Cross-Origin Resource Sharing
import helmet from 'helmet';    // Заголовки безопасности
import morgan from 'morgan';    // HTTP-логирование
import rateLimit from 'express-rate-limit';  // Защита от DDoS

app.use(cors({ origin: process.env.FRONTEND_URL }));
app.use(helmet());
app.use(morgan('dev'));
app.use(rateLimit({ windowMs: 15 * 60 * 1000, max: 100 }));
```

---

## 10. Базы данных: от JSON до PostgreSQL

### Эволюция хранилища в нашем проекте

```
Часть 1–3: JSON-файлы
  + Просто, не нужен сервер БД
  - Нет поиска, нет транзакций
  - Одновременная запись ломает файл
  - Не масштабируется

  ↓ переходим к части 4

Часть 4: SQLite
  + Один файл, SQL, транзакции, индексы
  + Не нужен отдельный сервер
  + Идеален для небольших приложений
  - Не поддерживает одновременную запись от нескольких серверов

  ↓ когда вырастем

Продакшн: PostgreSQL / MySQL
  + Полноценная СУБД
  + Одновременные соединения
  + Репликация, бэкапы
  - Нужен отдельный сервер
```

### Установка и подключение SQLite

```bash
npm install better-sqlite3
```

```js
// src/db/database.js
import Database from 'better-sqlite3';
import { join } from 'path';

const DB_PATH = join(process.cwd(), 'data', 'catalog.db');

// Создать/открыть БД
const db = new Database(DB_PATH);

// Включить WAL-режим (быстрее для чтения)
db.pragma('journal_mode = WAL');

// Создать таблицы
db.exec(`
  CREATE TABLE IF NOT EXISTS franchises (
    id           TEXT PRIMARY KEY,
    title        TEXT NOT NULL,
    original_title TEXT,
    studio       TEXT,
    year_start   INTEGER,
    year_end     INTEGER,
    status       TEXT DEFAULT 'ongoing',
    rating       REAL DEFAULT 0,
    cover        TEXT,
    description  TEXT,
    seasons_count INTEGER DEFAULT 0,
    episodes_total INTEGER DEFAULT 0,
    created_at   TEXT DEFAULT (datetime('now'))
  );

  CREATE TABLE IF NOT EXISTS genres (
    id           INTEGER PRIMARY KEY AUTOINCREMENT,
    franchise_id TEXT REFERENCES franchises(id) ON DELETE CASCADE,
    genre        TEXT NOT NULL
  );

  CREATE TABLE IF NOT EXISTS music (
    id           TEXT PRIMARY KEY,
    franchise_id TEXT REFERENCES franchises(id),
    type         TEXT NOT NULL,
    number       INTEGER DEFAULT 1,
    title        TEXT NOT NULL,
    artist       TEXT,
    season       INTEGER,
    year         INTEGER,
    youtube_id   TEXT,
    description  TEXT
  );

  CREATE TABLE IF NOT EXISTS users (
    id           INTEGER PRIMARY KEY AUTOINCREMENT,
    username     TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    role         TEXT DEFAULT 'user',
    created_at   TEXT DEFAULT (datetime('now'))
  );

  CREATE INDEX IF NOT EXISTS idx_franchises_status ON franchises(status);
  CREATE INDEX IF NOT EXISTS idx_franchises_rating ON franchises(rating);
  CREATE INDEX IF NOT EXISTS idx_music_franchise ON music(franchise_id);
`);

export default db;
```

### Миграция JSON → SQLite

```js
// scripts/migrate.js — запустить один раз: node scripts/migrate.js
import { readFile } from 'fs/promises';
import db from '../src/db/database.js';

async function migrate() {
  console.log('Начало миграции...');

  const franchisesRaw = await readFile('./data/franchises.json', 'utf-8');
  const franchises = JSON.parse(franchisesRaw);

  const musicRaw = await readFile('./data/music.json', 'utf-8');
  const music = JSON.parse(musicRaw);

  // Подготовленные запросы (быстрее и защита от SQL-инъекций)
  const insertFranchise = db.prepare(`
    INSERT OR REPLACE INTO franchises
      (id, title, original_title, studio, year_start, year_end,
       status, rating, cover, description, seasons_count, episodes_total)
    VALUES
      (@id, @title, @original_title, @studio, @year_start, @year_end,
       @status, @rating, @cover, @description, @seasons_count, @episodes_total)
  `);

  const insertGenre = db.prepare(`
    INSERT OR IGNORE INTO genres (franchise_id, genre) VALUES (@franchise_id, @genre)
  `);

  const insertMusic = db.prepare(`
    INSERT OR REPLACE INTO music
      (id, franchise_id, type, number, title, artist, season, year, youtube_id, description)
    VALUES
      (@id, @franchise_id, @type, @number, @title, @artist, @season, @year, @youtube_id, @description)
  `);

  // Транзакция — всё или ничего
  const migrateAll = db.transaction(() => {
    for (const f of franchises) {
      insertFranchise.run(f);
      for (const genre of f.genres) {
        insertGenre.run({ franchise_id: f.id, genre });
      }
    }
    for (const track of music) {
      insertMusic.run(track);
    }
  });

  migrateAll();
  console.log(`Мигрировано: ${franchises.length} франшиз, ${music.length} треков`);
}

migrate().catch(console.error);
```

---

## 11. SQL: основы языка запросов

SQL — язык для работы с реляционными базами данных. Нужно знать базовый уровень.

### SELECT — читать данные

```sql
-- Все франшизы
SELECT * FROM franchises;

-- Конкретные поля
SELECT id, title, rating FROM franchises;

-- С фильтром
SELECT * FROM franchises WHERE status = 'ongoing';
SELECT * FROM franchises WHERE rating >= 9.0;
SELECT * FROM franchises WHERE year_start BETWEEN 2019 AND 2023;

-- Сортировка
SELECT * FROM franchises ORDER BY rating DESC;
SELECT * FROM franchises ORDER BY title ASC;

-- Ограничение и смещение (пагинация)
SELECT * FROM franchises LIMIT 10 OFFSET 0;   -- страница 1
SELECT * FROM franchises LIMIT 10 OFFSET 10;  -- страница 2

-- Подсчёт
SELECT COUNT(*) FROM franchises;
SELECT COUNT(*) FROM franchises WHERE status = 'ongoing';

-- Агрегация
SELECT AVG(rating) as avg_rating FROM franchises;
SELECT MAX(rating), MIN(rating) FROM franchises;
SELECT status, COUNT(*) as count FROM franchises GROUP BY status;
```

### JOIN — соединить таблицы

```sql
-- Все франшизы с их жанрами
SELECT f.title, g.genre
FROM franchises f
JOIN genres g ON f.id = g.franchise_id
ORDER BY f.title;

-- Франшизы в жанре 'action'
SELECT f.*
FROM franchises f
JOIN genres g ON f.id = g.franchise_id
WHERE g.genre = 'action';

-- Франшиза со всеми треками
SELECT f.title, m.type, m.title as track_title, m.artist
FROM franchises f
LEFT JOIN music m ON f.id = m.franchise_id
WHERE f.id = 'aot';
-- LEFT JOIN включает франшизы БЕЗ треков тоже
```

### INSERT, UPDATE, DELETE

```sql
-- Вставить
INSERT INTO franchises (id, title, status, rating)
VALUES ('new-anime', 'Название', 'ongoing', 8.5);

-- Обновить
UPDATE franchises
SET rating = 9.2, status = 'completed'
WHERE id = 'aot';

-- Удалить
DELETE FROM franchises WHERE id = 'old-anime';
```

### Подготовленные запросы в Node

```js
// Никогда не делайте интерполяцию строк в SQL — это SQL-инъекция!
// ❌ ОПАСНО:
const id = req.params.id;
const result = db.prepare(`SELECT * FROM franchises WHERE id = '${id}'`).get();
// Если id = "' OR '1'='1" — выдаст ВСЮ базу

// ✅ Безопасно — параметры:
const getFranchise = db.prepare('SELECT * FROM franchises WHERE id = ?');
const result = getFranchise.get(req.params.id);

// С именованными параметрами (better-sqlite3):
const getFiltered = db.prepare(`
  SELECT * FROM franchises
  WHERE status = @status AND rating >= @minRating
`);
const result = getFiltered.all({ status: 'ongoing', minRating: 8.0 });
```

---

## 12. Аутентификация и авторизация

### Разница

```
Аутентификация (Authentication, AuthN):
  «Кто вы?» — проверка личности
  Логин + пароль, токен, биометрия

Авторизация (Authorization, AuthZ):
  «Что вам можно?» — проверка прав
  Роли (admin/user), разрешения
```

### Хранение паролей — только хеш

```js
import bcryptjs from 'bcryptjs';

// При регистрации — хешировать пароль
const password = 'mySecretPassword123';
const saltRounds = 12;  // чем больше — тем медленнее, но безопаснее
const hash = await bcryptjs.hash(password, saltRounds);
// '$2a$12$N9qo8uLOickgx2ZMRZoMyeIjZAgcfl7p92ldGxad68LJZdL17lhWy'

// Сохранить ТОЛЬКО hash в БД, никогда не сам пароль

// При входе — сравнить
const isMatch = await bcryptjs.compare(inputPassword, storedHash);
// true или false
```

### JWT — токен вместо сессии

**JWT (JSON Web Token)** — компактный самодостаточный токен с данными.

```
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9    ← header (base64)
.eyJpZCI6MSwidXNlcm5hbWUiOiJhZG1pbiJ9   ← payload (base64)
.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQ ← signature
```

```js
// src/auth/jwt.js
import jwt from 'jsonwebtoken';

const SECRET = process.env.JWT_SECRET;  // из .env

// Создать токен
export function signToken(payload) {
  return jwt.sign(payload, SECRET, { expiresIn: '7d' });
}

// Проверить токен
export function verifyToken(token) {
  return jwt.verify(token, SECRET);  // бросает ошибку если невалиден
}
```

```js
// src/routes/auth.js
import { Router } from 'express';
import bcryptjs from 'bcryptjs';
import db from '../db/database.js';
import { signToken } from '../auth/jwt.js';

const router = Router();

// POST /api/auth/login
router.post('/login', async (req, res) => {
  const { username, password } = req.body;

  if (!username || !password) {
    return res.status(400).json({ error: 'Заполните все поля' });
  }

  const user = db.prepare('SELECT * FROM users WHERE username = ?').get(username);
  if (!user) {
    return res.status(401).json({ error: 'Неверные данные' });
  }

  const isMatch = await bcryptjs.compare(password, user.password_hash);
  if (!isMatch) {
    return res.status(401).json({ error: 'Неверные данные' });
  }

  const token = signToken({ id: user.id, username: user.username, role: user.role });
  res.json({ token, user: { id: user.id, username: user.username, role: user.role } });
});

export default router;
```

```js
// src/middleware/auth.js — middleware для защищённых роутов
import { verifyToken } from '../auth/jwt.js';

export function requireAuth(req, res, next) {
  const header = req.headers.authorization;
  if (!header?.startsWith('Bearer ')) {
    return res.status(401).json({ error: 'Токен не предоставлен' });
  }

  try {
    const token = header.slice(7);
    req.user = verifyToken(token);
    next();
  } catch {
    res.status(401).json({ error: 'Токен недействителен' });
  }
}

export function requireAdmin(req, res, next) {
  requireAuth(req, res, () => {
    if (req.user.role !== 'admin') {
      return res.status(403).json({ error: 'Нет прав' });
    }
    next();
  });
}
```

```js
// Применить к роутам
app.use('/api/admin', requireAdmin);
app.post('/api/franchises', requireAdmin, createFranchise);
```

### Сессии vs JWT

```
┌─────────────────────────────┬────────────────────────────────┐
│         Сессии              │         JWT                    │
├─────────────────────────────┼────────────────────────────────┤
│ Данные на сервере в БД/RAM  │ Данные в самом токене          │
│ Сервер помнит сессию        │ Сервер stateless               │
│ Легко инвалидировать        │ Нельзя отозвать до истечения   │
│ Нужна shared БД при         │ Работает на нескольких         │
│ нескольких серверах         │ серверах без shared состояния  │
│                             │                                │
│ Когда брать:                │ Когда брать:                   │
│ Монолит, критична           │ Микросервисы, мобильные        │
│ немедленная инвалидация     │ приложения, stateless API      │
└─────────────────────────────┴────────────────────────────────┘
```

---

## 13. Переменные окружения и конфигурация

### Зачем нужны переменные окружения

Секреты (пароли, ключи API, строки подключения к БД) нельзя хранить в коде — они попадут в Git и станут публичными. Вместо этого используют переменные окружения.

```
.env — локально              Сервер (Railway, Render):
──────────────────────        ─────────────────────────────
PORT=3000                     Устанавливаются в панели управления
JWT_SECRET=supersecret123     через UI или CLI, не в файлах
DB_PATH=./data/catalog.db
FRONTEND_URL=http://localhost:5173
NODE_ENV=development
```

```bash
# .gitignore — ОБЯЗАТЕЛЬНО добавить
.env
.env.local
.env.production
```

```bash
# .env.example — КОММИТИТЬ (без реальных значений)
PORT=3000
JWT_SECRET=your-secret-key-here
DB_PATH=./data/catalog.db
FRONTEND_URL=http://localhost:5173
NODE_ENV=development
```

```js
// src/config.js
import 'dotenv/config';  // загружает .env в process.env

export const config = {
  port: parseInt(process.env.PORT) || 3000,
  jwtSecret: process.env.JWT_SECRET,
  dbPath: process.env.DB_PATH || './data/catalog.db',
  frontendUrl: process.env.FRONTEND_URL || 'http://localhost:5173',
  isDev: process.env.NODE_ENV !== 'production',
};

// Проверить что критичные переменные заданы
if (!config.jwtSecret) {
  console.error('ОШИБКА: JWT_SECRET не задан в .env');
  process.exit(1);
}
```

---

## 14. CORS и безопасность API

### Что такое CORS и почему он мешает

CORS (Cross-Origin Resource Sharing) — механизм безопасности браузера. Он запрещает JavaScript делать запросы к другому домену/порту без явного разрешения сервера.

```
Фронтенд:  http://localhost:5173
Бэкенд:    http://localhost:3000

Браузер:  «Подождите! 5173 ≠ 3000 — это разные origin!
           Нельзя без разрешения сервера!»
Ошибка:   Access to fetch at 'http://localhost:3000/api/...'
          from origin 'http://localhost:5173' has been blocked by CORS policy
```

**Заметьте:** CORS ограничивает только браузер. `curl`, Postman, Node.js — ходят куда хотят.

### Настройка CORS в Express

```js
import cors from 'cors';

// В разработке — разрешить всё (только для dev!)
app.use(cors());

// В продакшне — только ваш фронтенд
app.use(cors({
  origin: process.env.FRONTEND_URL,           // https://anime-catalog.netlify.app
  methods: ['GET', 'POST', 'PATCH', 'DELETE'],
  allowedHeaders: ['Content-Type', 'Authorization'],
  credentials: true,                           // разрешить куки
}));
```

### Базовые правила безопасности API

```js
// 1. Никогда не доверяй данным от клиента
app.post('/api/franchises', (req, res) => {
  // ❌ Не делайте так — кто угодно может прислать role: 'admin'
  const user = { ...req.body };

  // ✅ Берите только нужные поля явно
  const { title, rating, genres } = req.body;
});

// 2. Rate limiting — защита от перебора паролей и DDoS
import rateLimit from 'express-rate-limit';

const authLimiter = rateLimit({
  windowMs: 15 * 60 * 1000,  // 15 минут
  max: 10,                    // не более 10 попыток
  message: { error: 'Слишком много попыток, подождите' },
});
app.use('/api/auth', authLimiter);

// 3. helmet — заголовки безопасности
import helmet from 'helmet';
app.use(helmet());
// Добавляет: X-Content-Type-Options, X-Frame-Options,
// Content-Security-Policy и другие защитные заголовки

// 4. Валидация входных данных
function validateFranchise(data) {
  const errors = [];
  if (!data.title?.trim()) errors.push('title обязателен');
  if (data.rating && (data.rating < 0 || data.rating > 10))
    errors.push('rating должен быть от 0 до 10');
  return errors;
}
```

---

## 15. Сборка: структура fullstack-проекта

### Финальная структура проекта

```
anime-catalog/
├── frontend/                 ← Vite-проект (части 1-3)
│   ├── index.html
│   ├── css/
│   ├── js/
│   ├── data/                 ← JSON остаётся для статики
│   ├── assets/
│   ├── package.json
│   └── vite.config.js
│
├── backend/                  ← Node.js + Express (часть 4)
│   ├── src/
│   │   ├── server.js
│   │   ├── config.js
│   │   ├── db/
│   │   │   └── database.js
│   │   ├── routes/
│   │   │   ├── franchises.js
│   │   │   ├── music.js
│   │   │   └── auth.js
│   │   └── middleware/
│   │       └── auth.js
│   ├── data/
│   │   └── catalog.db
│   ├── scripts/
│   │   └── migrate.js
│   ├── .env                  ← не в Git!
│   ├── .env.example          ← в Git
│   └── package.json
│
└── README.md
```

### Vite для фронтенда

```bash
cd frontend
npm create vite@latest . -- --template vanilla
npm install
```

```js
// frontend/vite.config.js
import { defineConfig } from 'vite';

export default defineConfig({
  // В разработке — проксировать API-запросы на бэкенд
  server: {
    proxy: {
      '/api': {
        target: 'http://localhost:3000',
        changeOrigin: true,
      },
    },
  },
});
```

Это позволяет на фронтенде писать просто `fetch('/api/franchises')` — Vite сам прокинет на `http://localhost:3000/api/franchises`. Не нужно менять URL при деплое.

### Обновить фронтенд для работы с реальным API

```js
// frontend/js/data.js
// Часть 3: читали из /data/franchises.json
// Часть 4: читаем из /api/franchises

export async function loadFranchises(filters = {}) {
  const params = new URLSearchParams();
  if (filters.status && filters.status !== 'all') params.set('status', filters.status);
  if (filters.genre) params.set('genre', filters.genre);
  if (filters.minRating) params.set('minRating', filters.minRating);

  const url = `/api/franchises${params.size ? '?' + params : ''}`;
  const response = await fetch(url);

  if (!response.ok) throw new Error(`API error: ${response.status}`);
  return response.json();
}

// Аутентифицированный запрос
export async function apiFetch(url, options = {}) {
  const token = localStorage.getItem('admin_token');

  const res = await fetch(url, {
    ...options,
    headers: {
      'Content-Type': 'application/json',
      ...(token ? { Authorization: `Bearer ${token}` } : {}),
      ...options.headers,
    },
  });

  if (res.status === 401) {
    localStorage.removeItem('admin_token');
    window.location.href = '/pages/admin/login.html';
    return;
  }

  if (!res.ok) {
    const err = await res.json().catch(() => ({ error: res.statusText }));
    throw new Error(err.error || 'Ошибка запроса');
  }

  return res.status === 204 ? null : res.json();
}
```

---

## 16. Деплой

### Что такое деплой

**Деплой (deploy)** — процесс публикации приложения так, чтобы оно стало доступно через интернет.

```
Ваш компьютер                   Интернет
────────────────                 ──────────────────────
localhost:5173   ──────────►    https://anime-catalog.netlify.app
localhost:3000   ──────────►    https://anime-api.railway.app
```

### Фронтенд → Netlify

Netlify бесплатно хостит статичные сайты и SPA.

```bash
# 1. Сбилдить фронтенд
cd frontend
npm run build
# Создаётся папка dist/ с готовыми файлами

# 2. Способ 1: через CLI
npm install -g netlify-cli
netlify login
netlify deploy --prod --dir dist

# Способ 2: через GitHub
# → Зайти на netlify.com
# → New site from Git → выбрать репозиторий
# → Build command: npm run build
# → Publish directory: dist
# → Deploy site
```

```toml
# frontend/netlify.toml — конфиг для SPA (все пути → index.html)
[[redirects]]
  from = "/*"
  to = "/index.html"
  status = 200
```

**Переменные окружения на Netlify:**  
Site settings → Environment variables → Add variable:
```
VITE_API_URL = https://anime-api.railway.app
```

```js
// В коде — читать через import.meta.env (Vite)
const API_URL = import.meta.env.VITE_API_URL || 'http://localhost:3000';
```

### Бэкенд → Railway

Railway — PaaS-платформа для деплоя серверных приложений. Бесплатный тир с ограничениями.

```bash
# 1. Установить CLI
npm install -g @railway/cli

# 2. Войти
railway login

# 3. Создать проект из папки backend/
cd backend
railway init
railway up

# Или через GitHub:
# → railway.app → New Project → Deploy from GitHub repo
# → Выбрать репо → Configure → Root Directory: backend
```

```json
// backend/package.json — Railway смотрит на start
{
  "scripts": {
    "start": "node src/server.js",
    "dev": "nodemon src/server.js"
  }
}
```

**Переменные окружения на Railway:**  
Project → Variables → Add Variable:
```
JWT_SECRET = производственный_секрет_64_символа
FRONTEND_URL = https://anime-catalog.netlify.app
NODE_ENV = production
PORT = (Railway задаст автоматически)
```

### CI/CD — автоматический деплой

При подключении GitHub оба сервиса автоматически деплоят при пуше в `main`:

```
git push origin main
    ↓
GitHub webhook уведомляет Netlify/Railway
    ↓
Сервис клонирует репо, запускает build/install
    ↓
Публикует новую версию (~1-3 минуты)
```

---

## 17. DevTools: Network в глубину

### Что смотреть при отладке API

```
Вкладки запроса:
──────────────────────────────────────────────────────
Headers   — заголовки запроса и ответа
Preview   — отформатированный ответ (JSON, HTML)
Response  — сырой текст ответа
Timing    — сколько времени заняли разные фазы
Initiator — откуда в коде инициирован запрос
```

### Чек-лист при ошибке API

```
1. Открыть Network, найти упавший запрос (красный)
2. Status — что именно?
   401 → нет/неверный токен → проверить Authorization заголовок
   403 → нет прав → проверить роль пользователя
   404 → неверный URL → проверить путь
   422 → валидация → посмотреть тело запроса и ответа
   500 → ошибка сервера → смотреть логи сервера
3. Request Headers → есть ли Content-Type: application/json?
4. Request Payload → правильные ли данные отправляются?
5. Response → что сервер написал в error?
```

### CORS-ошибка в Network

```
Ошибка: Access to fetch... has been blocked by CORS policy

Что смотреть:
• Вкладка Headers → есть ли Access-Control-Allow-Origin в ответе?
• Если нет → CORS не настроен на сервере
• Если есть неверное значение → неверный origin в cors()

Частая ловушка: http://localhost:3000 ≠ http://localhost:3000/
(слэш в конце — другой origin для некоторых конфигов)
```

---

## 18. Что fullstack-джуну делать не нужно

```
ЧТО ДЕЛАЕТ ДЖУН:                   ЧТО ДЕЛАЕТ КТО-ТО ДРУГОЙ:
──────────────────────────          ─────────────────────────────────
REST API для фронтенда         ←→   Архитектура микросервисов (senior)
SQLite / простой PostgreSQL    ←→   DBA: оптимизация, репликация, шардинг
JWT аутентификация             ←→   OAuth провайдер (Google, GitHub) — библ.
Деплой на Netlify/Railway      ←→   Kubernetes, CI/CD пайплайны (DevOps)
Базовая безопасность           ←→   Пентест, security audit (специалист)
Vite конфиг                    ←→   Webpack custom config (слишком сложно)

ЧТО НЕ НУЖНО ДЕЛАТЬ ДЖУНУ:
─────────────────────────────────────────────────────────────────
× Писать свои криптографические функции — используй bcrypt, crypto
× Хранить пароли, токены в открытом виде
× Делать eval() / exec() с пользовательскими данными
× Строить SQL через конкатенацию строк — prepared statements
× Коммитить .env в Git
× Игнорировать ошибки в промисах без catch
× Писать огромные роут-функции — выносить в сервисы
× Деплоить с NODE_ENV=development на прод
```

---

## 19. Проект: полная сборка аниме-каталога

### Финальный `src/server.js`

```js
import express from 'express';
import cors from 'cors';
import helmet from 'helmet';
import { config } from './config.js';

// Роуты
import franchisesRouter from './routes/franchises.js';
import musicRouter from './routes/music.js';
import authRouter from './routes/auth.js';
import adminRouter from './routes/admin.js';

// Middleware
import { requireAdmin } from './middleware/auth.js';

const app = express();

// ─── Безопасность ──────────────────────────────────────────────
app.use(helmet());
app.use(cors({
  origin: config.frontendUrl,
  credentials: true,
}));

// ─── Парсинг запросов ──────────────────────────────────────────
app.use(express.json({ limit: '10mb' }));

// ─── Логирование ──────────────────────────────────────────────
app.use((req, res, next) => {
  const start = Date.now();
  res.on('finish', () => {
    const ms = Date.now() - start;
    console.log(`${req.method} ${req.url} ${res.statusCode} ${ms}ms`);
  });
  next();
});

// ─── Публичные роуты ───────────────────────────────────────────
app.use('/api/franchises', franchisesRouter);
app.use('/api/music', musicRouter);
app.use('/api/auth', authRouter);

// ─── Защищённые роуты (только admin) ──────────────────────────
app.use('/api/admin', requireAdmin, adminRouter);

// ─── Health check ──────────────────────────────────────────────
app.get('/health', (req, res) => {
  res.json({ status: 'ok', timestamp: new Date().toISOString() });
});

// ─── 404 для неизвестных маршрутов ────────────────────────────
app.use((req, res) => {
  res.status(404).json({ error: `Маршрут ${req.method} ${req.url} не найден` });
});

// ─── Глобальный обработчик ошибок ─────────────────────────────
app.use((err, req, res, next) => {
  console.error(err);
  res.status(err.status || 500).json({
    error: config.isDev ? err.message : 'Внутренняя ошибка сервера',
  });
});

// ─── Запуск ────────────────────────────────────────────────────
app.listen(config.port, () => {
  console.log(`
  ┌──────────────────────────────────┐
  │  Anime Catalog API               │
  │  http://localhost:${config.port}            │
  │  Среда: ${config.isDev ? 'development' : 'production  '}         │
  └──────────────────────────────────┘
  `);
});
```

### Admin-панель на фронтенде

```js
// frontend/js/admin.js
import { apiFetch } from './data.js';

async function initAdmin() {
  const token = localStorage.getItem('admin_token');
  if (!token) {
    window.location.href = '/pages/admin/login.html';
    return;
  }

  await loadDashboard();
  initAddForm();
}

async function loadDashboard() {
  try {
    const [franchises, music] = await Promise.all([
      apiFetch('/api/franchises'),
      apiFetch('/api/music'),
    ]);

    document.getElementById('stat-franchises').textContent = franchises.length;
    document.getElementById('stat-tracks').textContent = music.length;
    document.getElementById('stat-ongoing').textContent =
      franchises.filter(f => f.status === 'ongoing').length;

    renderRecentTable(franchises.slice(-5).reverse());
  } catch (err) {
    console.error('Dashboard error:', err);
  }
}

function initAddForm() {
  const form = document.querySelector('[aria-label="Форма добавления франшизы"]');
  if (!form) return;

  form.addEventListener('submit', async e => {
    e.preventDefault();
    const data = Object.fromEntries(new FormData(form));

    try {
      await apiFetch('/api/franchises', {
        method: 'POST',
        body: JSON.stringify({
          ...data,
          rating: parseFloat(data.rating),
          year_start: parseInt(data.year_start),
          genres: data.genres?.split(',').map(g => g.trim()) || [],
        }),
      });
      form.reset();
      await loadDashboard();
      showNotification('Франшиза добавлена!', 'success');
    } catch (err) {
      showNotification(err.message, 'error');
    }
  });
}

function showNotification(msg, type) {
  const el = document.querySelector('[aria-live="polite"]');
  if (el) {
    el.textContent = msg;
    el.className = `notification notification--${type}`;
    setTimeout(() => { el.textContent = ''; }, 3000);
  }
}

// Логин
document.querySelector('#login-form')?.addEventListener('submit', async e => {
  e.preventDefault();
  const { username, password } = Object.fromEntries(new FormData(e.target));

  try {
    const { token } = await apiFetch('/api/auth/login', {
      method: 'POST',
      body: JSON.stringify({ username, password }),
    });
    localStorage.setItem('admin_token', token);
    window.location.href = '/pages/admin/';
  } catch (err) {
    document.getElementById('login-error').textContent = err.message;
  }
});

initAdmin();
```

---

## Итого: что умеете после всех четырёх частей

```
HTML:
  ✓ Критический путь рендеринга
  ✓ Семантическая разметка
  ✓ Доступность и SEO
  ✓ Формы, медиа, таблицы

CSS:
  ✓ Каскад, специфичность, box model
  ✓ Flexbox и Grid
  ✓ Переменные, адаптивность
  ✓ Анимации на transform/opacity

JavaScript:
  ✓ Типы, замыкания, this
  ✓ Асинхронность и Event Loop
  ✓ DOM, события, делегирование
  ✓ Модули, реактивность, шаблонизация

Fullstack:
  ✓ HTTP: запросы, ответы, статус-коды
  ✓ REST API: проектирование и реализация
  ✓ Node.js и Express
  ✓ SQLite: создать схему, запросы, JOIN
  ✓ Аутентификация через JWT
  ✓ CORS, базовая безопасность
  ✓ Деплой: Netlify + Railway
  ✓ Переменные окружения

Что дальше:
  → Фреймворки: React или Vue
  → TypeScript
  → PostgreSQL + ORM (Prisma, Drizzle)
  → Тестирование (Vitest, Playwright)
  → Docker и контейнеризация
```

---

> **Вопросы для самопроверки**: [questions/04-fullstack-questions.md](./questions/04-fullstack-questions.md) — 90 вопросов от HTTP до деплоя  
> **Все части курса**: [01-html-guide.md](./01-html-guide.md) · [02-css-guide.md](./02-css-guide.md) · [03-js-guide.md](./03-js-guide.md) · **04-fullstack-guide.md**
