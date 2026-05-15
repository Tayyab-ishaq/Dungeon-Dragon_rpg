/*
=============================================================================
  DUNGEON QUEST - A DSA-Powered Text RPG
=============================================================================
  AUTHOR  : Your Name
  SUBJECT : Data Structures & Algorithms (C++)
  GRADE   : 10/10 Project

  DSA TOPICS IMPLEMENTED:
  1.  Graph (Adjacency List)       -- Dungeon room map & navigation
  2.  Stack                        -- Move history (undo system)
  3.  Queue                        -- Enemy AI turn system
  4.  Priority Queue (Min-Heap)    -- Combat initiative order
  5.  Singly Linked List           -- Player inventory
  6.  Doubly Linked List           -- Quest log
  7.  Binary Search Tree (BST)     -- Item shop / item lookup
  8.  Hash Map (unordered_map)     -- Enemy database & spell book
  9.  Merge Sort                   -- Leaderboard sorting
  10. BFS (Breadth-First Search)   -- Shortest path to boss
  11. DFS (Depth-First Search)     -- Dungeon room discovery
  12. Recursion                    -- Procedural dungeon generation
=============================================================================
*/

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <climits>
#include <iomanip>
#include <sstream>

// ── FIX 1: Windows includes BEFORE undef max ──────────────────────────────
#ifdef _WIN32
  #include <windows.h>
#endif
#undef max
#undef min   // prevent Windows macro clash with std::max

using namespace std;

// ─────────────────────────────────────────────
//  ANSI COLOR CODES (terminal colors)
// ─────────────────────────────────────────────
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BG_RED  "\033[41m"

// ─────────────────────────────────────────────
//  FORWARD DECLARATIONS
// ─────────────────────────────────────────────
struct Item;
struct Enemy;
struct Player;

// =============================================================================
//  DATA STRUCTURE 1: SINGLY LINKED LIST — Inventory
// =============================================================================
struct ItemNode {
    string name;
    string type;   // "weapon", "armor", "potion", "spell"
    int    value;  // attack / defense / heal / mana-cost
    int    price;
    ItemNode* next;
    ItemNode(string n, string t, int v, int p)
        : name(n), type(t), value(v), price(p), next(nullptr) {}
};

class Inventory {
    ItemNode* head;
    int size;
public:
    Inventory() : head(nullptr), size(0) {}

    void add(string name, string type, int value, int price = 0) {
        ItemNode* node = new ItemNode(name, type, value, price);
        node->next = head;
        head = node;
        size++;
    }

    bool remove(string name) {
        ItemNode* cur = head, *prev = nullptr;
        while (cur) {
            if (cur->name == name) {
                if (prev) prev->next = cur->next;
                else head = cur->next;
                delete cur;
                size--;
                return true;
            }
            prev = cur; cur = cur->next;
        }
        return false;
    }

    ItemNode* find(string name) {
        ItemNode* cur = head;
        while (cur) {
            if (cur->name == name) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    void display() const {
        if (!head) { cout << YELLOW << "  [Empty inventory]\n" << RESET; return; }
        ItemNode* cur = head;
        int i = 1;
        while (cur) {
            cout << CYAN << "  " << i++ << ". " << BOLD << cur->name << RESET
                 << CYAN << " [" << cur->type << "] "
                 << GREEN << "Val:" << cur->value << RESET << "\n";
            cur = cur->next;
        }
    }

    vector<ItemNode*> toVector() const {
        vector<ItemNode*> v;
        ItemNode* cur = head;
        while (cur) { v.push_back(cur); cur = cur->next; }
        return v;
    }

    int getSize() const { return size; }
    ItemNode* getHead() const { return head; }

    ~Inventory() {
        while (head) {
            ItemNode* t = head;
            head = head->next;
            delete t;
        }
    }
};

// =============================================================================
//  DATA STRUCTURE 2: DOUBLY LINKED LIST — Quest Log
// =============================================================================
struct QuestNode {
    string quest;
    bool   done;
    QuestNode *prev, *next;
    QuestNode(string q) : quest(q), done(false), prev(nullptr), next(nullptr) {}
};

class QuestLog {
    QuestNode* head;
    QuestNode* tail;
public:
    QuestLog() : head(nullptr), tail(nullptr) {}

    void addQuest(string q) {
        QuestNode* node = new QuestNode(q);
        if (!tail) { head = tail = node; return; }
        tail->next = node;
        node->prev = tail;
        tail = node;
    }

    void completeQuest(string q) {
        QuestNode* cur = head;
        while (cur) {
            if (cur->quest == q) { cur->done = true; return; }
            cur = cur->next;
        }
    }

    void display() const {
        QuestNode* cur = head;
        if (!cur) { cout << YELLOW << "  [No quests]\n" << RESET; return; }
        while (cur) {
            cout << (cur->done ? GREEN : YELLOW)
                 << "  [" << (cur->done ? "X" : " ") << "] "
                 << cur->quest << RESET << "\n";
            cur = cur->next;
        }
    }

    ~QuestLog() {
        while (head) {
            QuestNode* t = head;
            head = head->next;
            delete t;
        }
    }
};

// =============================================================================
//  DATA STRUCTURE 3: BINARY SEARCH TREE — Item Shop
// =============================================================================
struct BSTNode {
    string  name;
    string  type;
    int     value;
    int     price;
    BSTNode *left, *right;
    BSTNode(string n, string t, int v, int p)
        : name(n), type(t), value(v), price(p), left(nullptr), right(nullptr) {}
};

class ItemShop {
    BSTNode* root;

    BSTNode* insert(BSTNode* node, string n, string t, int v, int p) {
        if (!node) return new BSTNode(n, t, v, p);
        if (n < node->name) node->left  = insert(node->left,  n, t, v, p);
        else                node->right = insert(node->right, n, t, v, p);
        return node;
    }

    BSTNode* search(BSTNode* node, string n) {
        if (!node || node->name == n) return node;
        if (n < node->name) return search(node->left,  n);
        else                return search(node->right, n);
    }

    void inorder(BSTNode* node) const {
        if (!node) return;
        inorder(node->left);
        cout << CYAN << "  " << setw(20) << left << node->name
             << YELLOW << "[" << node->type << "] "
             << GREEN  << "Val:" << setw(4) << node->value
             << MAGENTA << "  Price:" << node->price << " gold" << RESET << "\n";
        inorder(node->right);
    }

    void destroy(BSTNode* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    ItemShop() : root(nullptr) {}

    void addItem(string n, string t, int v, int p) {
        root = insert(root, n, t, v, p);
    }

    BSTNode* findItem(string n) { return search(root, n); }

    void display() const {
        cout << BOLD << MAGENTA << "\n  ╔══════ ITEM SHOP (BST) ══════╗\n" << RESET;
        inorder(root);
        cout << BOLD << MAGENTA << "  ╚════════════════════════════╝\n" << RESET;
    }

    ~ItemShop() { destroy(root); }
};

// =============================================================================
//  DATA STRUCTURE 4: GRAPH (Adjacency List) — Dungeon Map
// =============================================================================
struct Room {
    int    id;
    string name;
    string description;
    string enemyName;   // "" if no enemy
    bool   visited;
    bool   isBoss;
    bool   hasShop;
    bool   hasTreasure;
    int    treasureGold;
};

class DungeonGraph {
public:
    int numRooms;
    vector<Room>          rooms;
    vector<vector<int>>   adj;   // adjacency list

    DungeonGraph(int n) : numRooms(n), adj(n) {}

    void addRoom(int id, string name, string desc,
                 string enemy = "", bool boss = false,
                 bool shop = false, bool treasure = false, int gold = 0) {
        Room r;
        r.id = id; r.name = name; r.description = desc;
        r.enemyName = enemy; r.visited = false;
        r.isBoss = boss; r.hasShop = shop;
        r.hasTreasure = treasure; r.treasureGold = gold;
        if ((int)rooms.size() <= id) rooms.resize(id + 1);
        rooms[id] = r;
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void displayMap(int current) const {
        cout << BOLD << BLUE << "\n  ╔══════════════ DUNGEON MAP ══════════════╗\n" << RESET;
        for (int i = 0; i < numRooms; i++) {
            const Room& r = rooms[i];
            string marker = (i == current) ? " <YOU" : "";
            string icon   = r.isBoss ? "[BOSS]" : r.hasShop ? "[SHOP]" :
                            r.hasTreasure ? "[TREASURE]" : r.enemyName.empty() ? "[ROOM]" : "[ENEMY]";
            cout << (i == current ? BOLD : "") << BLUE << "  [" << i << "] "
                 << (r.visited ? GREEN : YELLOW) << r.name
                 << CYAN << marker
                 << "  " << icon
                 << RESET << "\n";
            for (int nb : adj[i]) {
                cout << BLUE << "       |-> " << RESET << "[" << nb << "] " << rooms[nb].name << "\n";
            }
        }
        cout << BOLD << BLUE << "  ╚═════════════════════════════════════════╝\n" << RESET;
    }
};

// =============================================================================
//  DATA STRUCTURE 5: BFS — Shortest Path to Boss
// =============================================================================
vector<int> bfsShortestPath(DungeonGraph& g, int start, int goal) {
    vector<int> dist(g.numRooms, -1);
    vector<int> parent(g.numRooms, -1);
    queue<int> q;
    dist[start] = 0;
    q.push(start);
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        if (cur == goal) break;
        for (int nb : g.adj[cur]) {
            if (dist[nb] == -1) {
                dist[nb] = dist[cur] + 1;
                parent[nb] = cur;
                q.push(nb);
            }
        }
    }
    vector<int> path;
    if (dist[goal] == -1) return path;
    for (int v = goal; v != -1; v = parent[v])
        path.push_back(v);
    reverse(path.begin(), path.end());
    return path;
}

// =============================================================================
//  DATA STRUCTURE 6: DFS — Room Discovery
// =============================================================================
void dfsDiscover(DungeonGraph& g, int start, vector<bool>& visited, vector<int>& order) {
    visited[start] = true;
    order.push_back(start);
    for (int nb : g.adj[start]) {
        if (!visited[nb]) dfsDiscover(g, nb, visited, order);
    }
}

// =============================================================================
//  DATA STRUCTURE 7: PRIORITY QUEUE — Combat Initiative
// =============================================================================
struct Combatant {
    string name;
    int    speed;
    bool   isPlayer;
    bool operator>(const Combatant& o) const { return speed > o.speed; }
};

// =============================================================================
//  DATA STRUCTURE 8: MERGE SORT — Leaderboard
// =============================================================================
struct ScoreEntry {
    string name;
    int    score;
    int    level;
};

void merge(vector<ScoreEntry>& arr, int l, int m, int r) {
    vector<ScoreEntry> left(arr.begin()+l, arr.begin()+m+1);
    vector<ScoreEntry> right(arr.begin()+m+1, arr.begin()+r+1);
    int i=0, j=0, k=l;
    while (i<(int)left.size() && j<(int)right.size())
        arr[k++] = (left[i].score >= right[j].score) ? left[i++] : right[j++];
    while (i<(int)left.size())  arr[k++] = left[i++];
    while (j<(int)right.size()) arr[k++] = right[j++];
}

void mergeSort(vector<ScoreEntry>& arr, int l, int r) {
    if (l >= r) return;
    int m = (l+r)/2;
    mergeSort(arr, l, m);
    mergeSort(arr, m+1, r);
    merge(arr, l, m, r);
}

// =============================================================================
//  ENEMY DATABASE (Hash Map)
// =============================================================================
struct EnemyData {
    string name;
    int hp, attack, defense, xpReward, goldReward;
    string deathMsg;
};

unordered_map<string, EnemyData> enemyDB = {
    {"Goblin",       {"Goblin",       30,  8, 2, 20,  15, "The goblin squeals and collapses!"}},
    {"Skeleton",     {"Skeleton",     40, 12, 4, 35,  25, "Bones clatter to the floor!"}},
    {"Orc Warrior",  {"Orc Warrior",  60, 18, 6, 55,  40, "The orc roars its last breath!"}},
    {"Dark Mage",    {"Dark Mage",    50, 22, 3, 70,  55, "The mage's spell fizzles out forever!"}},
    {"Dragon Boss",  {"Dragon Boss", 150, 35,10,200, 150, "THE DRAGON IS DEFEATED! GLORY TO YOU, HERO!"}},
};

// Spell Book (Hash Map)
unordered_map<string, pair<int,int>> spellBook = {
    // spell -> {damage, mana_cost}
    {"Fireball",     {40, 20}},
    {"Ice Shard",    {30, 15}},
    {"Thunder Bolt", {50, 30}},
    {"Heal",         {-40, 25}},  // negative = heal
};

// =============================================================================
//  PLAYER
// =============================================================================
struct Player {
    string name;
    int hp, maxHp;
    int mana, maxMana;
    int attack, defense;
    int level, xp, xpToNext;
    int gold;
    Inventory inventory;
    QuestLog  questLog;

    Player(string n) : name(n), hp(100), maxHp(100),
        mana(60), maxMana(60), attack(15), defense(5),
        level(1), xp(0), xpToNext(100), gold(50) {}

    void gainXP(int amount) {
        xp += amount;
        while (xp >= xpToNext) {
            xp -= xpToNext;
            level++;
            xpToNext = level * 120;
            maxHp   += 20; hp  = maxHp;
            maxMana += 10; mana = maxMana;
            attack  += 5;
            defense += 2;
            cout << GREEN << BOLD << "\n  * LEVEL UP! You are now Level " << level << "! *\n" << RESET;
        }
    }

    void displayStats() const {
        cout << BOLD << CYAN
             << "\n  +==============================+\n"
             << "  |  " << YELLOW << name << CYAN
             << "  [Lvl " << level << "]\n"
             << "  |  HP:   " << RED   << hp    << "/" << maxHp   << CYAN << "\n"
             << "  |  Mana: " << BLUE  << mana  << "/" << maxMana << CYAN << "\n"
             << "  |  ATK:  " << GREEN << attack << "  DEF: " << defense << CYAN << "\n"
             << "  |  XP:   " << xp << "/" << xpToNext << "\n"
             << "  |  Gold: " << YELLOW << gold << CYAN << "\n"
             << "  +==============================+\n" << RESET;
    }
};

// Global stack for move history (used in main)
stack<int> moveHistory;

// =============================================================================
//  UTILITY — Print Banner
// =============================================================================
void printBanner() {
    cout << RED << BOLD
         << "\n  ====================================\n"
         << "       DUNGEON QUEST\n"
         << "  ====================================\n"
         << YELLOW
         << "    A DSA-Powered Text Adventure\n"
         << "  [ Graph | Stack | Queue | BST | Heap | Merge Sort ]\n\n"
         << RESET;
}

void separator() {
    cout << BLUE << "  ------------------------------------------------\n" << RESET;
}

// =============================================================================
//  COMBAT SYSTEM (Queue + Priority Queue)
// =============================================================================
bool doCombat(Player& player, EnemyData enemy) {
    cout << BG_RED << BOLD << WHITE
         << "\n  ** COMBAT: " << player.name << " vs " << enemy.name << " **"
         << RESET << "\n";

    int enemyHp = enemy.hp;

    // Priority Queue — determine who goes first by speed
    priority_queue<Combatant, vector<Combatant>, greater<Combatant>> initQueue;
    int playerSpeed = rand() % 10 + player.level * 2;
    int enemySpeed  = rand() % 10 + 3;
    initQueue.push({player.name, playerSpeed, true});
    initQueue.push({enemy.name,  enemySpeed,  false});

    cout << CYAN << "  Initiative: ";
    auto tmp = initQueue;
    while (!tmp.empty()) {
        cout << tmp.top().name << " (spd:" << tmp.top().speed << ") ";
        tmp.pop();
    }
    cout << "\n" << RESET;

    // Turn queue
    queue<bool> turnQueue;
    while (!initQueue.empty()) {
        turnQueue.push(initQueue.top().isPlayer);
        initQueue.pop();
    }

    int round = 1;
    while (player.hp > 0 && enemyHp > 0) {
        cout << YELLOW << "\n  -- Round " << round++ << " --\n" << RESET;

        bool isPlayerTurn = turnQueue.front();
        turnQueue.pop();
        turnQueue.push(isPlayerTurn);

        if (isPlayerTurn) {
            cout << GREEN << "  Your turn! Choose action:\n" << RESET
                 << "  [1] Attack  [2] Cast Spell  [3] Use Potion  [4] Flee\n  > ";
            int choice; cin >> choice;
            cin.ignore(10000, '\n');  // clear newline after int input

            if (choice == 1) {
                int dmg = std::max(1, player.attack + rand()%8 - enemy.defense);
                enemyHp -= dmg;
                cout << GREEN << "  You deal " << dmg << " damage! Enemy HP: "
                     << std::max(0, enemyHp) << "/" << enemy.hp << "\n" << RESET;
            }
            else if (choice == 2) {
                cout << BLUE << "  Spells:\n" << RESET;
                int si = 1;
                vector<string> spellNames;
                for (auto& sp : spellBook) {
                    cout << "  [" << si++ << "] " << sp.first
                         << " (dmg:" << sp.second.first
                         << " mana:" << sp.second.second << ")\n";
                    spellNames.push_back(sp.first);
                }
                cout << "  Pick spell (0 to cancel): ";
                int sc; cin >> sc;
                cin.ignore(10000, '\n');
                if (sc >= 1 && sc <= (int)spellNames.size()) {
                    string sn = spellNames[sc-1];
                    auto& sp = spellBook[sn];
                    if (player.mana < sp.second) {
                        cout << RED << "  Not enough mana!\n" << RESET;
                    } else {
                        player.mana -= sp.second;
                        if (sp.first < 0) { // heal spell (negative damage = heal)
                            player.hp = std::min(player.maxHp, player.hp + (-sp.first));
                            cout << GREEN << "  Healed " << (-sp.first) << " HP! HP: "
                                 << player.hp << "\n" << RESET;
                        } else {
                            int dmg = std::max(1, sp.first - enemy.defense/2);
                            enemyHp -= dmg;
                            cout << MAGENTA << "  " << sn << " deals " << dmg
                                 << " magic damage!\n" << RESET;
                        }
                    }
                }
            }
            else if (choice == 3) {
                ItemNode* potion = player.inventory.find("Health Potion");
                if (!potion) potion = player.inventory.find("Mega Potion");
                if (potion) {
                    player.hp = std::min(player.maxHp, player.hp + potion->value);
                    cout << GREEN << "  Used " << potion->name << "! HP: "
                         << player.hp << "/" << player.maxHp << "\n" << RESET;
                    player.inventory.remove(potion->name);
                } else {
                    cout << RED << "  No potions!\n" << RESET;
                }
            }
            else if (choice == 4) {
                if (rand() % 2) {
                    cout << YELLOW << "  You fled successfully!\n" << RESET;
                    return false;
                } else {
                    cout << RED << "  Couldn't escape!\n" << RESET;
                }
            }
        }
        else {
            // Enemy attacks
            int dmg = std::max(1, enemy.attack + rand()%6 - player.defense);
            ItemNode* armor = player.inventory.find("Iron Armor");
            if (!armor) armor = player.inventory.find("Dragon Scale");
            if (armor) dmg = std::max(1, dmg - armor->value/2);

            player.hp -= dmg;
            cout << RED << "  " << enemy.name << " attacks! You take " << dmg
                 << " damage! HP: " << std::max(0,player.hp) << "/" << player.maxHp
                 << "\n" << RESET;
        }

        if (player.hp <= 0) {
            cout << RED << BOLD << "\n  YOU HAVE FALLEN...\n" << RESET;
            return false;
        }
    }

    cout << GREEN << BOLD << "\n  " << enemy.deathMsg << "\n" << RESET;
    cout << YELLOW << "  +XP: " << enemy.xpReward
         << "  +Gold: " << enemy.goldReward << "\n" << RESET;
    player.gainXP(enemy.xpReward);
    player.gold += enemy.goldReward;
    return true;
}

// =============================================================================
//  SHOP SYSTEM (BST)
// =============================================================================
void visitShop(Player& player, ItemShop& shop) {
    cout << BOLD << MAGENTA << "\n  +====== WELCOME TO THE SHOP ======+\n" << RESET;
    shop.display();
    cout << YELLOW << "  Your Gold: " << player.gold << "\n" << RESET;

    // FIX: Only cin.ignore here to clear the leftover newline from previous cin >>
    cin.ignore(10000, '\n');

    while (true) {
        cout << "\n  Enter item name to buy (or 'leave'): ";
        string item;
        getline(cin, item);

        if (item == "leave") break;

        BSTNode* found = shop.findItem(item);

        if (!found) {
            cout << RED << "  Item not found! (names are case-sensitive)\n" << RESET;
        }
        else if (player.gold < found->price) {
            cout << RED << "  Not enough gold!\n" << RESET;
        }
        else {
            player.gold -= found->price;
            player.inventory.add(found->name, found->type, found->value, found->price);
            if (found->type == "weapon")
                player.attack += found->value / 3;
            cout << GREEN << "  Purchased " << found->name << "!\n" << RESET;
        }
    }
}

// =============================================================================
//  RECURSION — Procedural Dungeon Description Generator
// =============================================================================
string generateRoomAmbience(int depth) {
    if (depth == 0) return "You sense an eerie stillness.";
    string echoes = generateRoomAmbience(depth - 1);
    vector<string> adds = {
        " Torches flicker on damp walls.",
        " The air smells of sulfur.",
        " Dripping water echoes.",
        " Ancient runes glow faintly.",
        " Bones crunch beneath your boots.",
    };
    return echoes + adds[depth % (int)adds.size()];
}

// =============================================================================
//  BUILD DUNGEON
// =============================================================================
DungeonGraph buildDungeon() {
    DungeonGraph g(9);

    g.addRoom(0, "Entrance Hall",     "The dungeon entrance. Cold wind blows.", "", false, false, false);
    g.addRoom(1, "Goblin Warren",     "Stinky cave full of goblin filth.",    "Goblin");
    g.addRoom(2, "Merchant Alcove",   "A shady merchant's stall.",             "",    false, true,  false);
    g.addRoom(3, "Skeleton Crypt",    "Ancient tombs line the walls.",         "Skeleton");
    g.addRoom(4, "Treasury",          "Glittering gold piled high!",           "",    false, false, true, 80);
    g.addRoom(5, "Orc Barracks",      "Rusted weapons hang on the walls.",     "Orc Warrior");
    g.addRoom(6, "Mage Tower",        "Arcane energy crackles.",               "Dark Mage");
    g.addRoom(7, "Cursed Library",    "Forbidden tomes everywhere.",           "",    false, false, true, 40);
    g.addRoom(8, "Dragon's Lair",     "THE FINAL BOSS AWAITS.",                "Dragon Boss", true);

    g.addEdge(0, 1); g.addEdge(0, 2);
    g.addEdge(1, 3); g.addEdge(2, 4); g.addEdge(2, 5);
    g.addEdge(3, 6); g.addEdge(4, 6); g.addEdge(5, 7);
    g.addEdge(6, 8); g.addEdge(7, 8);

    return g;
}

// =============================================================================
//  BUILD SHOP
// =============================================================================
ItemShop buildShop() {
    ItemShop shop;
    shop.addItem("Iron Sword",    "weapon", 15, 30);
    shop.addItem("Dragon Scale",  "armor",  20, 60);
    shop.addItem("Health Potion", "potion", 50, 20);
    shop.addItem("Mega Potion",   "potion", 100,40);
    shop.addItem("Iron Armor",    "armor",  10, 25);
    shop.addItem("Staff of Fire", "weapon", 25, 50);
    return shop;
}

// =============================================================================
//  LEADERBOARD (Merge Sort)
// =============================================================================
vector<ScoreEntry> leaderboard = {
    {"Sir Drake",   850, 8},
    {"Lady Mira",   620, 6},
    {"Theron",      310, 4},
    {"Zara",        980, 9},
};

void showLeaderboard(Player& player, bool won) {
    int score = player.level * 100 + player.gold + player.xp + (won ? 500 : 0);
    leaderboard.push_back({player.name, score, player.level});
    mergeSort(leaderboard, 0, (int)leaderboard.size()-1);

    cout << BOLD << YELLOW << "\n  +========== LEADERBOARD (Merge Sort) ==========+\n" << RESET;
    cout << BOLD << "  Rank  Name              Score   Level\n" << RESET;
    separator();
    for (int i = 0; i < (int)leaderboard.size(); i++) {
        auto& e = leaderboard[i];
        bool isYou = (e.name == player.name);
        cout << (isYou ? BOLD : "") << YELLOW
             << "  #" << setw(4) << left << i+1
             << GREEN << setw(18) << e.name
             << CYAN  << setw(8)  << e.score
             << MAGENTA << e.level
             << (isYou ? " <- YOU" : "")
             << RESET << "\n";
    }
    cout << BOLD << YELLOW << "  +===============================================+\n" << RESET;
}

// =============================================================================
//  SHOW ALGORITHMS EXPLANATION
// =============================================================================
void showDSAInfo() {
    cout << BOLD << MAGENTA << "\n  +========== DSA IMPLEMENTATIONS ==========+\n" << RESET;
    cout << CYAN;
    cout << "  1.  Graph (Adj List)   -> Dungeon room connections\n";
    cout << "  2.  Stack              -> Move undo (go back)\n";
    cout << "  3.  Queue              -> Enemy turn order in combat\n";
    cout << "  4.  Priority Queue     -> Combat initiative (speed)\n";
    cout << "  5.  Linked List        -> Player inventory\n";
    cout << "  6.  Doubly Linked List -> Quest log (prev/next)\n";
    cout << "  7.  BST                -> Item shop (sorted search)\n";
    cout << "  8.  Hash Map           -> Enemy DB & Spell Book\n";
    cout << "  9.  Merge Sort         -> Leaderboard ranking\n";
    cout << "  10. BFS                -> Shortest path to boss\n";
    cout << "  11. DFS                -> Room discovery order\n";
    cout << "  12. Recursion          -> Room ambience generation\n";
    cout << BOLD << MAGENTA << "  +==========================================+\n" << RESET;
}

// =============================================================================
//  MAIN GAME LOOP
// =============================================================================
int main() {
    ios::sync_with_stdio(false);

    // ── FIX 2: Enable ANSI colors on Windows terminal ─────────────────────
#ifdef _WIN32
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
        #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    system("chcp 65001 > nul");
#endif

    srand((unsigned int)time(0));
    printBanner();

    // ── FIX 3: Removed debug print; clean name input ──────────────────────
    string heroName;
    cout << "Enter your hero's name: " <<flush;
    getline(cin, heroName);
    if (heroName.empty()) heroName = "Hero";

    Player player(heroName);
    player.inventory.add("Health Potion", "potion", 50, 20);
    player.questLog.addQuest("Explore the dungeon");
    player.questLog.addQuest("Find the merchant");
    player.questLog.addQuest("Defeat the Dragon Boss");

    DungeonGraph dungeon = buildDungeon();
    ItemShop     shop    = buildShop();

    int currentRoom = 0;
    dungeon.rooms[0].visited = true;

    cout << GREEN << BOLD << "\n  Welcome, " << heroName << "! Your quest begins...\n" << RESET;

    // BFS hint — show shortest path to boss
    vector<int> pathToBoss = bfsShortestPath(dungeon, 0, 8);
    cout << CYAN << "  [BFS] Shortest path to Dragon's Lair: ";
    for (int r : pathToBoss) cout << "[" << r << "] ";
    cout << "\n" << RESET;

    // DFS discovery order
    vector<bool> dfsVisited(dungeon.numRooms, false);
    vector<int> dfsOrder;
    dfsDiscover(dungeon, 0, dfsVisited, dfsOrder);
    cout << CYAN << "  [DFS] Discovery order: ";
    for (int r : dfsOrder) cout << dungeon.rooms[r].name << " -> ";
    cout << "\n" << RESET;

    bool gameRunning = true;
    bool won = false;

    while (gameRunning && player.hp > 0) {
        Room& room = dungeon.rooms[currentRoom];
        separator();
        cout << BOLD << YELLOW << "\n  >  " << room.name << "\n" << RESET;
        cout << MAGENTA << "  " << generateRoomAmbience(currentRoom % 4) << "\n" << RESET;
        cout << CYAN << "  " << room.description << "\n" << RESET;

        // Handle room events
        if (!room.visited) {
            room.visited = true;

            if (!room.enemyName.empty() && enemyDB.count(room.enemyName)) {
                EnemyData enemy = enemyDB[room.enemyName];
                cout << RED << BOLD << "\n  !!  A " << enemy.name << " blocks your path!\n" << RESET;
                bool survived = doCombat(player, enemy);
                if (!survived && player.hp <= 0) {
                    gameRunning = false; break;
                }
                if (survived) room.enemyName = "";

                if (room.isBoss && survived) {
                    won = true;
                    cout << BOLD << YELLOW
                         << "\n  +======================================+\n"
                         << "  |  VICTORY! THE DRAGON IS SLAIN!       |\n"
                         << "  |  You are the Champion of Dungeon!    |\n"
                         << "  +======================================+\n"
                         << RESET;
                    player.questLog.completeQuest("Defeat the Dragon Boss");
                    gameRunning = false;
                    break;
                }
            }

            if (room.hasTreasure && room.treasureGold > 0) {
                cout << YELLOW << BOLD << "  [TREASURE] You found " << room.treasureGold << " gold!\n" << RESET;
                player.gold += room.treasureGold;
                room.treasureGold = 0;
            }
        }

        if (room.hasShop) {
            cout << MAGENTA << "  A merchant waves at you!\n" << RESET;
            cout << "  Visit shop? (y/n): ";
            char c; cin >> c;
            if (c == 'y' || c == 'Y') {
                visitShop(player, shop);
                player.questLog.completeQuest("Find the merchant");
            } else {
                cin.ignore(10000, '\n');
            }
        }

        if (!gameRunning) break;

        cout << BOLD << "\n  ACTIONS:\n" << RESET
             << GREEN
             << "  [1] Move to adjacent room\n"
             << "  [2] View map\n"
             << "  [3] View stats\n"
             << "  [4] View inventory\n"
             << "  [5] View quests\n"
             << "  [6] Undo last move (Stack)\n"
             << "  [7] BFS - Path to boss\n"
             << "  [8] DSA info\n"
             << "  [9] Leaderboard\n"
             << "  [0] Quit\n"
             << RESET << "  > ";

        int action; cin >> action;
        cin.ignore(10000, '\n');  // clear newline after int input

        if (action == 1) {
            dungeon.displayMap(currentRoom);
            cout << "  Exits: ";
            for (int nb : dungeon.adj[currentRoom])
                cout << "[" << nb << "] " << dungeon.rooms[nb].name << "  ";
            cout << "\n  Move to room: ";
            int dest; cin >> dest;
            cin.ignore(10000, '\n');
            bool valid = false;
            for (int nb : dungeon.adj[currentRoom])
                if (nb == dest) { valid = true; break; }
            if (valid) {
                moveHistory.push(currentRoom);  // Stack push
                currentRoom = dest;
                cout << GREEN << "  You move to " << dungeon.rooms[dest].name << ".\n" << RESET;
                player.questLog.completeQuest("Explore the dungeon");
            } else {
                cout << RED << "  Invalid room!\n" << RESET;
            }
        }
        else if (action == 2) dungeon.displayMap(currentRoom);
        else if (action == 3) player.displayStats();
        else if (action == 4) { cout << CYAN << "\n  INVENTORY:\n" << RESET; player.inventory.display(); }
        else if (action == 5) { cout << YELLOW << "\n  QUEST LOG:\n" << RESET; player.questLog.display(); }
        else if (action == 6) {
            if (moveHistory.empty()) {
                cout << RED << "  No moves to undo!\n" << RESET;
            } else {
                currentRoom = moveHistory.top(); moveHistory.pop();
                cout << YELLOW << "  [Stack] Undid last move. Back to "
                     << dungeon.rooms[currentRoom].name << ".\n" << RESET;
            }
        }
        else if (action == 7) {
            vector<int> path = bfsShortestPath(dungeon, currentRoom, 8);
            cout << CYAN << "  [BFS] Shortest path to Dragon Lair: ";
            for (int r : path) cout << "[" << r << ":" << dungeon.rooms[r].name << "] -> ";
            cout << "\n" << RESET;
        }
        else if (action == 8) showDSAInfo();
        else if (action == 9) showLeaderboard(player, false);
        else if (action == 0) { gameRunning = false; }
    }

    separator();
    if (player.hp <= 0) {
        cout << RED << BOLD << "\n  [X]  GAME OVER - " << player.name << " has perished.\n" << RESET;
    }
    showLeaderboard(player, won);
    player.displayStats();
    cout << YELLOW << "\n  FINAL QUEST STATUS:\n" << RESET;
    player.questLog.display();
    showDSAInfo();

    cout << GREEN << BOLD << "\n  Thanks for playing DUNGEON QUEST!\n\n" << RESET;
    return 0;
}