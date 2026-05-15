# 🐉 Dungeon Quest — A DSA-Powered Text RPG

> A terminal-based dungeon crawler built entirely around **12 core Data Structures & Algorithms** concepts, written in modern C++.

---

## 📖 About

**Dungeon Quest** is a text-based RPG where every game mechanic is powered by a real DSA concept — not just used as decoration, but as the actual logic behind navigation, combat, inventory, shops, and progression. Fight monsters, buy gear, explore rooms, and slay the Dragon Boss — all while seeing DSA work in action.

This project was built as a **Data Structures & Algorithms course project** and demonstrates practical, non-trivial use of 12 different concepts in a single cohesive application.

---

## ✨ Features

- ⚔️ **Turn-based combat** with initiative order, spells, potions, and flee mechanics
- 🗺️ **Dungeon exploration** across 9 connected rooms with a real graph structure
- 🛒 **Item shop** with BST-powered sorted search and lookup
- 🎒 **Inventory system** using a singly linked list
- 📜 **Quest log** with a doubly linked list (navigate forward & back)
- 🔁 **Undo last move** using a stack
- 🧠 **BFS shortest path** to the boss from any room
- 🔍 **DFS room discovery** order shown at game start
- 🏆 **Leaderboard** sorted with merge sort
- 🎲 **Procedural room descriptions** via recursion
- 🧟 **Enemy & spell databases** powered by hash maps
- 🌈 Full **ANSI terminal color** support (with Windows compatibility)

---

## 🧩 DSA Concepts Implemented

| # | Concept | Where It's Used |
|---|---------|----------------|
| 1 | **Graph (Adjacency List)** | Dungeon room map & connections |
| 2 | **Stack** | Move history — undo last room |
| 3 | **Queue** | Enemy/player turn order in combat |
| 4 | **Priority Queue (Min-Heap)** | Combat initiative by speed stat |
| 5 | **Singly Linked List** | Player inventory (add/remove/find) |
| 6 | **Doubly Linked List** | Quest log (forward & backward traversal) |
| 7 | **Binary Search Tree (BST)** | Item shop — sorted insert & search |
| 8 | **Hash Map (`unordered_map`)** | Enemy database & spell book |
| 9 | **Merge Sort** | Leaderboard ranking by score |
| 10 | **BFS (Breadth-First Search)** | Shortest path from current room to boss |
| 11 | **DFS (Depth-First Search)** | Full dungeon discovery order |
| 12 | **Recursion** | Procedural room ambience generator |

---

## 🗺️ Dungeon Layout

```
[0] Entrance Hall
 ├── [1] Goblin Warren ── [3] Skeleton Crypt ── [6] Mage Tower ──┐
 └── [2] Merchant Alcove                                          ├── [8] Dragon's Lair (BOSS)
          ├── [4] Treasury ─────────────────────────────[6]──────┘
          └── [5] Orc Barracks ── [7] Cursed Library ───────────┘
```

9 rooms, 10 edges, all navigable via the graph's adjacency list.

---

## 🚀 Getting Started

### Prerequisites

- A C++17 compatible compiler (`g++`, `clang++`, or MSVC)
- A terminal with ANSI color support
  - **Windows**: Windows Terminal or VS Code terminal (auto-enabled in code)
  - **Linux/macOS**: Any standard terminal

### Compile & Run

**Linux / macOS**
```bash
g++ -std=c++17 -o dungeon_quest main.cpp
./dungeon_quest
```

**Windows (MinGW)**
```bash
g++ -std=c++17 -o dungeon_quest.exe main.cpp
dungeon_quest.exe
```

**Windows (MSVC)**
```bash
cl /EHsc /std:c++17 main.cpp /Fe:dungeon_quest.exe
dungeon_quest.exe
```

---

## 🎮 How to Play

1. Enter your hero's name to begin
2. The game shows the **BFS shortest path** to the Dragon's Lair and **DFS discovery order**
3. Navigate between rooms using the action menu
4. When you enter a new room with an enemy, **combat starts automatically**
5. In combat, you can **Attack**, **Cast a Spell**, **Use a Potion**, or **Flee**
6. Visit the **Merchant Alcove** to buy weapons, armor, and potions from the shop
7. Collect treasure, gain XP, level up, and reach **Room 8** to fight the Dragon Boss

### In-Game Actions

| Key | Action |
|-----|--------|
| `1` | Move to an adjacent room |
| `2` | View the dungeon map |
| `3` | View player stats |
| `4` | View inventory |
| `5` | View quest log |
| `6` | Undo last move (Stack) |
| `7` | BFS — shortest path to boss |
| `8` | View DSA info panel |
| `9` | View leaderboard |
| `0` | Quit |

---

## ⚔️ Combat Guide

- **Attack** — physical damage based on your ATK vs enemy DEF
- **Cast Spell** — choose from Fireball, Ice Shard, Thunder Bolt, or Heal (costs mana)
- **Use Potion** — consumes a Health Potion or Mega Potion from inventory
- **Flee** — 50% chance to escape combat

Initiative order (who goes first each round) is determined by a **priority queue** using randomized speed values.

---

## 🛒 Shop Items

| Item | Type | Value | Price |
|------|------|-------|-------|
| Iron Sword | Weapon | +15 ATK | 30 gold |
| Staff of Fire | Weapon | +25 ATK | 50 gold |
| Iron Armor | Armor | +10 DEF | 25 gold |
| Dragon Scale | Armor | +20 DEF | 60 gold |
| Health Potion | Potion | +50 HP | 20 gold |
| Mega Potion | Potion | +100 HP | 40 gold |

Items are stored in a **BST sorted by name** for O(log n) lookup.

---

## 📁 Project Structure

```
dungeon-quest/
│
├── main.cpp          # Complete source — all DSA + game logic
└── README.md         # This file
```

Single-file project by design, so every DSA implementation is easy to find and review.

---

## 🏆 Scoring & Leaderboard

Your final score is calculated as:

```
Score = (Level × 100) + Gold + XP + (500 if Dragon slain)
```

Pre-seeded with 4 fictional heroes. Your entry is inserted and the list is sorted with **merge sort** before display.

---

## 🛠️ Technical Notes

- All dynamic data structures (Inventory, QuestLog, BST, Graph) use **manual memory management** with `new`/`delete` and proper destructors — no smart pointers
- The Windows `max`/`min` macro conflict with `std::max`/`std::min` is resolved with `#undef`
- ANSI color output on Windows is enabled at runtime via `SetConsoleMode`
- `cin.ignore()` is used after every `cin >>` to prevent input stream corruption on `getline` calls

---

## 📚 License

This project is open source under the [MIT License](LICENSE).

---

## 👤 Author

Built as a DSA course project. Feel free to fork, extend, or use as a reference for your own DSA projects.

> *"The dungeon is a data structure. You are the algorithm."*
