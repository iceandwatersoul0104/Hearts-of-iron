# 钢铁意志：第三帝国的黄昏

> *战死沙场者唯有无尽长眠，惟有死者方能见证战争之终结。*  
> —— 一部关于战争残酷与无义的文字悲剧

---

## 目录

- [项目概述](#项目概述)
- [技术栈](#技术栈)
- [项目结构](#项目结构)
- [构建与运行](#构建与运行)
- [核心架构](#核心架构)
  - [游戏引擎 (GameEngine)](#游戏引擎-gameengine)
  - [玩家系统 (Player)](#玩家系统-player)
  - [场景系统 (Scenario)](#场景系统-scenario)
  - [故事节点与选择 (StoryNode & Choice)](#故事节点与选择-storynode--choice)
  - [存档管理 (SaveManager)](#存档管理-savemanager)
  - [音乐系统 (MusicPlayer)](#音乐系统-musicplayer)
- [游戏系统](#游戏系统)
  - [骰子判定系统](#骰子判定系统)
  - [叙事标志系统](#叙事标志系统)
  - [职业系统](#职业系统)
  - [打字机效果](#打字机效果)
  - [自动存档](#自动存档)
- [UI 界面](#ui-界面)
  - [主菜单 (MenuWidget)](#主菜单-menuwidget)
  - [角色创建 (CharacterCreateWidget)](#角色创建-charactercreatewidget)
  - [游戏主界面 (GameWidget)](#游戏主界面-gamewidget)
  - [存档对话框 (SaveLoadDialog)](#存档对话框-saveloaddialog)
  - [主窗口 (MainWindow)](#主窗口-mainwindow)
- [场景详情](#场景详情)
  - [黄色方案 (FallGelb)](#黄色方案-fallgelb)
  - [不列颠空战 (Britain)](#不列颠空战-britain)
  - [群狼海战 (WolfPack)](#群狼海战-wolfpack)
  - [斯大林格勒战役 (Stalingrad)](#斯大林格勒战役-stalingrad)
  - [柏林战役 (Berlin)](#柏林战役-berlin)
- [美术风格](#美术风格)
- [音乐系统](#音乐系统说明)
- [数据流与生命周期](#数据流与生命周期)
- [已知局限与未来改进](#已知局限与未来改进)
- [致谢](#致谢)

---

## 项目概述

**钢铁意志：第三帝国的黄昏** 是一款以**第二次世界大战欧洲战场**为背景的交互式文字冒险游戏。玩家将从 1940 年德国发动黄色方案开始，依次经历不列颠空战、大西洋海战、斯大林格勒战役，最终在 1945 年柏林战役中迎来帝国覆灭的终章。

不同于传统战争游戏的正面叙事，本作聚焦于**战争的残酷与荒诞**——玩家将站在德军士兵的视角，亲历战友的死亡、平民的苦难、道德困境的抉择，在不断的消耗中见证第三帝国从巅峰走向崩溃。

### 核心特色

- **六种职业路线**：步兵、坦克兵、战斗机飞行员、轰炸机飞行员、潜艇驾驶员、战列舰水兵，不同职业对应不同的初次战役与判定加成
- **五个历史战役**：基于真实历史事件改编，每个战役包含 10-27 个分支叙事节点
- **骰子战斗系统**：基于 D100 的随机判定，职业加成提升成功率
- **叙事标志系统**：玩家的道德选择会留下永久标志，影响后续剧情
- **分支多结局**：每个战役拥有胜利 / 失败多种结局，总计 15+ 种结局路径
- **打字机文本渲染**：逐字打印的故事叙述，增强氛围沉浸感
- **存档系统**：1 个自动存档位 + 3 个手动存档位，JSON 格式持久化

---

## 技术栈

| 层级 | 技术 |
|------|------|
| 语言 | C++17 |
| GUI 框架 | Qt 6 (Core / Widgets / Multimedia) |
| 构建系统 | CMake 3.16+ |
| 音频 | QMediaPlayer + QAudioOutput |
| 数据序列化 | QJsonDocument / QJsonObject |
| 样式 | Qt Style Sheets (QSS) |

---

## 项目结构

```
Hearts of iron/
├── CMakeLists.txt              # CMake 构建配置
├── main.cpp                    # 应用入口
├── core/                       # 核心逻辑层
│   ├── GameState.h             # 全局枚举与辅助函数
│   ├── StoryNode.h             # 故事节点与选项数据结构
│   ├── Player.h / .cpp         # 玩家数据模型与序列化
│   ├── GameEngine.h / .cpp     # 核心游戏逻辑引擎
│   ├── ScenarioBase.h / .cpp   # 场景抽象基类
│   ├── SaveManager.h / .cpp    # 存档管理（JSON 持久化）
│   └── MusicPlayer.h / .cpp    # 背景音乐管理
├── scenarios/                  # 五大战役场景
│   ├── FallGelbScenario.h/.cpp     # 黄色方案 (1940)
│   ├── BritainScenario.h/.cpp      # 不列颠空战 (1940)
│   ├── WolfPackScenario.h/.cpp     # 群狼海战 (1941-1943)
│   ├── StalingradScenario.h/.cpp   # 斯大林格勒 (1942-1943)
│   └── BerlinScenario.h/.cpp       # 柏林战役 (1945)
├── ui/                         # 用户界面层
│   ├── MainWindow.h / .cpp         # 主窗口与路由控制
│   ├── MenuWidget.h / .cpp         # 主菜单界面
│   ├── CharacterCreateWidget.h/.cpp # 角色创建界面
│   ├── GameWidget.h / .cpp         # 游戏主界面（含打字机效果）
│   └── SaveLoadDialog.h / .cpp     # 存档/读档对话框
└── resources/
    ├── resources.qrc            # Qt 资源文件索引
    ├── style.qss                # 全局 QSS 样式表
    └── music/                   # 背景音乐文件（需自行准备）
        ├── main_theme.mp3
        ├── fallgelb.mp3 / fallgelb_village.mp3 / fallgelb_battle.mp3 / fallgelb_triumph.mp3
        ├── britain.mp3 / britain_air.mp3 / britain_night.mp3 / britain_triumph.mp3
        ├── wolfpack.mp3 / wolfpack_deep.mp3 / wolfpack_triumph.mp3
        ├── stalingrad.mp3 / stalingrad_winter.mp3 / stalingrad_end.mp3
        ├── berlin.mp3 / berlin_elegy.mp3 / berlin_end.mp3
        └── defeat_theme.mp3
```

---

## 构建与运行

### 前置依赖

- **Qt 6** (Core + Widgets + Multimedia 模块)
- **CMake 3.16+**
- **支持 C++17 的编译器** (MSVC 2019+, GCC 9+, Clang 10+)

### 构建步骤

```bash
# 1. 进入项目根目录
cd "Hearts of iron"

# 2. 创建构建目录
mkdir build && cd build

# 3. CMake 配置
cmake ..

# 4. 编译
cmake --build . --config Release

# 5. 运行
./钢铁意志_第三帝国.exe
```

### 准备音乐文件

在 `resources/music/` 下放入对应的 `.mp3` 文件（参见上方目录结构中的文件名列表）。如果音乐文件不存在，程序会静默跳过，不影响核心游戏逻辑运行。

---

## 核心架构

项目采用 **MVC 分层架构**：

```
UI 层 (ui/)  ←→  核心引擎 (core/GameEngine)  ←→  数据层 (core/Player, ScenarioBase)
                                        ↕
                                  音乐 (core/MusicPlayer)  +  存档 (core/SaveManager)
```

### 游戏引擎 (GameEngine)

**文件：** `core/GameEngine.h`, `core/GameEngine.cpp`

是整个游戏的中央调度器，继承自 `QObject`，通过 Qt 信号槽机制与 UI 层通信。

#### 核心职责

| 方法 | 说明 |
|------|------|
| `newGame(name, class)` | 创建新游戏，初始化玩家数据，解锁对应职业的起始战役 |
| `loadGame(player)` | 从存档恢复游戏，导航到保存时的节点 |
| `startScenario(id)` | 启动指定战役场景，重置 HP/士气，跳转到职业对应的起始节点 |
| `makeChoice(index)` | 处理玩家选择：检验条件 → 骰子判定 → 应用后果 → 导航跳转 |
| `rollDice(bonusClasses)` | D100 骰子系统，职业匹配时 +20 加成，范围钳制在 [1, 120] |

#### 信号（Signal）

| 信号 | 触发时机 | 接收方 |
|------|---------|--------|
| `nodeChanged(node)` | 故事节点切换 | MainWindow → GameWidget 刷新界面 |
| `statsChanged(hp, morale)` | HP 或士气值变化 | MainWindow → GameWidget 更新进度条 |
| `combatResult(success, hpDelta, moraleDelta)` | 战斗判定结束 | MainWindow → 弹出结果弹窗 |
| `scenarioVictory(id)` | 节点标记 `isVictory = true` | MainWindow → 解锁下一战役、弹窗提示 |
| `scenarioDefeat(id)` | 节点标记 `isDefeat = true` | MainWindow → 切换失败音乐 |
| `flagSet(flag)` | 叙事标志被设置 | 预留，用于 UI 侧特殊反馈 |

#### 选择处理流程

```
玩家点击选项按钮
    ↓
GameWidget 发射 choiceMade(index)
    ↓
MainWindow::onChoiceMade(index)
    ↓ (index == -2 表示进入下一战役，否则普通选择)
GameEngine::makeChoice(index)
    ↓
1. 获取当前节点（空则返回）
2. Narrative 节点 → 直接跳转到 nextNodeId
3. Choice 节点 → 越界检查 → 职业限制检查 → 标志条件检查
4. 战斗选项 → rollDice 判定 → 成功 / 失败分流
5. 非战斗选项 → 直接应用后果 → 导航跳转
6. HP ≤ 0 → 触发失败结局跳转
```

---

### 玩家系统 (Player)

**文件：** `core/Player.h`, `core/Player.cpp`

#### 数据结构

```cpp
class Player {
    QString     name;               // 玩家姓名
    PlayerClass playerClass;        // 职业
    int         hp      = 100;      // 当前生命值 [0, 100]
    int         maxHp   = 100;      // 最大生命值
    int         morale  = 100;      // 当前士气值 [0, 100]
    int         maxMorale = 100;    // 最大士气值
    QSet<QString> flags;           // 叙事标志集合
    ScenarioId  currentScenario;    // 当前所在场景
    QString     currentNodeId;      // 当前节点 ID
    QSet<int>   unlockedScenarios;  // 已解锁场景 ID 集合
};
```

#### 核心规则

- **死亡判定：** `hp <= 0 || morale <= 0` 即判定阵亡
- **HP/士气钳制：** 使用 `std::clamp` 保证值永远在 `[0, max]` 范围内
- **`resetStats()`：** 新战役开始时重置 HP/士气至满值，清空标志和当前节点

#### JSON 序列化

- `toJson()` — 导出完整的玩家状态为 JSON，包含标志数组和已解锁场景数组
- `fromJson(obj)` — 从 JSON 恢复玩家状态，带默认值容错（空字段默认 100）

---

### 场景系统 (Scenario)

**文件：** `core/ScenarioBase.h`, `core/ScenarioBase.cpp`, `scenarios/*.h`, `scenarios/*.cpp`

#### 基类接口

```cpp
class ScenarioBase {
    virtual void initialize() = 0;                              // 初始化所有节点
    virtual ScenarioId scenarioId() const = 0;                  // 返回场景枚举
    virtual QString startNodeId(PlayerClass cls) const = 0;    // 职业起点节点
    virtual QString defeatNodeId() const;                       // 失败节点 ID（可覆盖）
    const StoryNode* getNode(const QString &id) const;          // 按 ID 查找节点
};
```

#### 注册与生命周期

所有五个场景在 `GameEngine` 构造函数中通过 `registerScenarios()` 统一创建并初始化：

```cpp
addScenario(std::make_unique<FallGelbScenario>());
addScenario(std::make_unique<BritainScenario>());
// ...
```

每个场景构造函数调用 `initialize()`，通过多次调用 `addNode(storyNode)` 构建内部 `QMap<QString, StoryNode>` 的节点图。

#### 存档不持久化场景数据

场景数据（节点文本、选项逻辑）是硬编码的常量图，不随存档序列化。只有玩家状态被保存。

---

### 故事节点与选择 (StoryNode & Choice)

**文件：** `core/StoryNode.h`

#### Choice 数据结构

```cpp
struct Choice {
    QString text;                          // 按钮显示文字
    QString nextNodeId;                    // 默认跳转目标
    int hpDelta = 0;                       // HP 变化量
    int moraleDelta = 0;                   // 士气变化量
    QSet<QString> requiredFlags;          // 前置标志（未拥有则不可见）
    QSet<QString> grantedFlags;           // 选择后设置的标志
    bool classRestricted = false;          // 是否职业限制
    QList<PlayerClass> allowedClasses;    // 允许的职业列表
    bool isCombat = false;                // 是否为战斗选项
    int combatThreshold = 50;              // 骰子判定阈值
    QList<PlayerClass> bonusClasses;      // 享受骰子加成的职业
    QString successNodeId;                // 战斗成功跳转
    QString failureNodeId;                // 战斗失败跳转
    int failHpDelta = -20;                // 失败 HP 损失
    int failMoraleDelta = -15;            // 失败士气损失
};
```

#### StoryNode 节点类型

| NodeType | 说明 | 界面行为 |
|----------|------|---------|
| `Narrative` | 纯叙事节点 | 显示"【继续】"按钮，点击跳转到 `nextNodeId` |
| `Choice` | 选择分支节点 | 生成对应选项按钮，包含战斗 / 非战斗选项 |
| `Ending` | 终止节点 | 显示"进入下一战役"或"返回主页"按钮 |

#### 职业专属文本

`StoryNode::classText` 是一个 `QMap<PlayerClass, QString>`，用于覆盖不同职业在同一节点的显示文本。`textFor(cls)` 方法优先返回职业专属文本，否则返回通用文本。

---

### 存档管理 (SaveManager)

**文件：** `core/SaveManager.h`, `core/SaveManager.cpp`

#### 存档槽位设计

| 槽位 | 用途 | 可手动覆盖 |
|------|------|-----------|
| Slot 0 | 自动存档 | ❌ 不可（UI 灰化禁用） |
| Slot 1-3 | 手动存档 | ✅ 可 |

#### 存储路径

```
%APPDATA%/HeartsOfIronGame/saves/save_0.json  (自动存档)
%APPDATA%/HeartsOfIronGame/saves/save_1.json  (手动存档)
...
```

#### 存档数据结构

```json
{
    "slot": 1,
    "autoSave": false,
    "timestamp": "1940-05-10 03:00:00",
    "player": {
        "name": "汉斯 · 缪勒",
        "playerClass": 0,
        "hp": 85,
        "maxHp": 100,
        "morale": 90,
        "maxMorale": 100,
        "currentScenario": 0,
        "currentNodeId": "fg_village_enter",
        "flags": ["spared_village"],
        "unlockedScenarios": [0, 1]
    }
}
```

---

### 音乐系统 (MusicPlayer)

**文件：** `core/MusicPlayer.h`, `core/MusicPlayer.cpp`

#### 设计要点

- 使用 `QMediaPlayer` + `QAudioOutput` 实现音频播放
- 通过 **键-路径映射**（`QMap<QString, QString>`）管理音轨——场景节点通过 `musicKey` 声明需要播放的音乐
- **循环播放：** 默认开启，检测 `EndOfMedia` 状态后重置位置并重播
- **静音支持：** `setMuted(true)` 将音量设为零，但不改变存储的音量值
- **播放去重：** 如果请求的 key 等于当前 key 且正在播放，则跳过
- **文件容错：** 如果音乐文件不存在（`QFileInfo::exists` 返回 false），静默跳过不崩溃

#### 已注册的音轨

| 键 (key) | 对应文件 | 使用场景 |
|-----------|---------|---------|
| `main_theme` | main_theme.mp3 | 主菜单 |
| `fallgelb` | fallgelb.mp3 | 黄色方案 — 行军、命令 |
| `fallgelb_village` | fallgelb_village.mp3 | 黄色方案 — 比利时村庄 |
| `fallgelb_battle` | fallgelb_battle.mp3 | 黄色方案 — 战斗 |
| `fallgelb_triumph` | fallgelb_triumph.mp3 | 黄色方案 — 胜利 |
| `britain` | britain.mp3 | 不列颠空战 — 简报、基地 |
| `britain_air` | britain_air.mp3 | 不列颠空战 — 空中 |
| `britain_night` | britain_night.mp3 | 不列颠空战 — 夜间轰炸 |
| `britain_triumph` | britain_triumph.mp3 | 不列颠空战 — 胜利 |
| `wolfpack` | wolfpack.mp3 | 群狼海战 — 海面、基地 |
| `wolfpack_deep` | wolfpack_deep.mp3 | 群狼海战 — 深潜、战斗 |
| `wolfpack_triumph` | wolfpack_triumph.mp3 | 群狼海战 — 胜利 |
| `stalingrad` | stalingrad.mp3 | 斯大林格勒 — 城区战斗 |
| `stalingrad_winter` | stalingrad_winter.mp3 | 斯大林格勒 — 冬季 |
| `stalingrad_end` | stalingrad_end.mp3 | 斯大林格勒 — 结局 |
| `berlin` | berlin.mp3 | 柏林战役 — 战斗 |
| `berlin_elegy` | berlin_elegy.mp3 | 柏林战役 — 终局挽歌 |
| `berlin_end` | berlin_end.mp3 | 柏林战役 — 结局 |
| `defeat_theme` | defeat_theme.mp3 | 所有战役失败 |

---

## 游戏系统

### 骰子判定系统

所有战斗选项通过 **D100 骰子系统** 进行随机判定：

```cpp
int rollDice(const QList<PlayerClass> &bonusClasses) const {
    int roll = QRandomGenerator::global()->bounded(1, 101); // [1, 100]
    if (bonusClasses.contains(m_player.playerClass))
        roll += 20;                       // 职业匹配 → +20 加成
    return qBound(1, roll, 120);          // 钳制范围 [1, 120]
}
```

判定逻辑：
```
最终投掷值 = 基础 1-100 随机数 + (职业匹配 ? 20 : 0)
结果：最终值 ≥ 选项的 combatThreshold → 成功，否则失败
```

| 阈值 | 无加成成功率 | 有加成成功率 | 典型场景 |
|------|------------|------------|---------|
| 40 | 61% | 81% | 谨慎策略（迂回、等待） |
| 50 | 51% | 71% | 标准战斗 |
| 55 | 46% | 66% | 高强度战斗（强攻） |
| 60 | 41% | 61% | 危险行动（正面突击） |
| 65 | 36% | 56% | 极限行动（RAF 反击、坦克突围） |
| 70 | 31% | 51% | 绝望行动（强攻伏尔加河岸） |
| 90 | 11% | 31% | 最终死战（斯大林格勒最后抵抗） |

### 叙事标志系统

玩家的道德抉择会留下永久的**叙事标志**（`QSet<QString> flags`），影响后续选项的可见性：

| 标志 | 来源场景 | 触发条件 | 效果 |
|------|---------|---------|------|
| `spared_village` | 黄色方案 | 不骚扰比利时村民 | 解锁后续人道相关选项 |
| `waited_for_civilians` | 黄色方案 | 等待难民自行疏散 | 影响后续叙事文本 |
| `reflected` | 黄色方案 | 独自反思胜利 | 改变结局叙事 |
| `refused_bomb` | 不列颠空战 | 拒绝轰炸非军事目标 | 影响军官会谈文本 |
| `questioned_london` | 不列颠空战 | 质疑伦敦轰炸令 | 影响道德叙事走向 |
| `saved_survivors` | 群狼海战 | 违反命令救助商船幸存者 | 改变道德叙事 |
| `shared_ration` | 斯大林格勒 | 分享最后口粮 | 改变冬季叙事氛围 |
| `called_artillery` | 斯大林格勒 | 呼叫炮兵覆盖工厂 | 反映道德代价 |
| `wrote_truth` | 群狼海战 | 如实写信描述战争 | 改变结局叙事 |
| `saved_boy` | 柏林战役 | 劝孩子回家 | 改变国会大厦叙事 |

### 职业系统

#### 六大职业

| 职业 | 初战场景 | 可用场景 | 判定加成的典型场景 |
|------|---------|---------|------------------|
| **步兵** | 黄色方案 | FallGelb, Stalingrad, Berlin | 地面强攻、匍匐推进、逐楼清扫 |
| **坦克兵** | 黄色方案 | FallGelb, Stalingrad, Berlin | 坦克迂回、装甲强攻 |
| **战斗机飞行员** | 不列颠空战 | Britain, Berlin (防空塔) | 空战狗斗、编队作战 |
| **轰炸机飞行员** | 不列颠空战 | Britain, Berlin (防空塔) | 燃油管理、空战判定 |
| **潜艇驾驶员** | 群狼海战 | WolfPack, Berlin (河岸防御) | 鱼雷攻击、深潜规避 |
| **战列舰水兵** | 群狼海战 | WolfPack, Berlin (河岸防御) | 海上战斗、落水生存 |

#### 职业限制

某些选项通过 `classRestricted = true` 和 `allowedClasses` 限制仅特定职业可选。不满足限制的选项会直接在 UI 中隐藏不生成按钮。

---

### 打字机效果

**实现文件：** `ui/GameWidget.cpp`

- 使用 `QTimer` 每 20ms 打印一个字符
- 打字期间**禁用选项按钮和存档/读档按钮**，防止玩家提前操作
- 支持**点击跳过**：安装事件过滤器监听 `QTextBrowser` 和其 `viewport` 的 `MouseButtonPress` 事件
- 文字全部打印完毕后调用 `displayFullText()`，启用所有按钮

### 自动存档

每次 `onNodeChanged` 触发时自动写入 Slot 0（自动存档位），确保玩家进度不丢失。

---

## UI 界面

### 主菜单 (MenuWidget)

**文件：** `ui/MenuWidget.h`, `ui/MenuWidget.cpp`

- 大标题 "钢铁意志：第三帝国的黄昏"
- 反战副标题 "—— 一部关于战争残酷与无义的文字悲剧"
- 底部标语 "战死沙场者唯有无尽长眠，惟有死者方能见证战争之终结。"
- 三个按钮：开始新的战役 / 继续旧的战役 / 退出战役
- 通过 Qt 信号 `newGameClicked` / `loadGameClicked` / `exitGameClicked` 与 MainWindow 通信

### 角色创建 (CharacterCreateWidget)

**文件：** `ui/CharacterCreateWidget.h`, `ui/CharacterCreateWidget.cpp`

- **姓名输入：** QLineEdit，最大 15 字符，默认名 "汉斯 · 缪勒"
- **职业选择：** 2×3 网格卡片布局，6 个半透明 QPushButton 覆盖层触发选择
- **右侧预览面板：**
  - 初始生命/士气值显示
  - 初战方向提示
  - 详细的兵种特征描述文案
- **选择高亮：** 通过动态属性 `selected="true"` + QSS 伪类实现视觉反馈
- **表单验证：** 姓名为空时弹出警告，阻止开始游戏

### 游戏主界面 (GameWidget)

**文件：** `ui/GameWidget.h`, `ui/GameWidget.cpp`

#### 布局结构

```
┌──────────────────────────────────────────────────┐
│  顶部状态栏 (gameHeader)                         │
│  ┌──────────┬──────────────────┬────────────────┐│
│  │ 军官姓名  │  生命: ████░░    │  黄色方案      ││
│  │ 兵种      │  士气: ██████   │  阿登森林      ││
│  └──────────┴──────────────────┴────────────────┘│
│                                                  │
│  ┌──────────────────────────────────────────────┐│
│  │                                              ││
│  │         中央文本叙事区 (QTextBrowser)          ││
│  │         打字机逐字打印 + 滚动                  ││
│  │                                              ││
│  └──────────────────────────────────────────────┘│
│                                                  │
│  ┌───────────────────────────┬──────────────────┐│
│  │  选项按钮区               │  保存战役         ││
│  │  [选项1: 强攻]           │  载入战役         ││
│  │  [选项2: 迂回]           │  撤回后方         ││
│  └───────────────────────────┴──────────────────┘│
└──────────────────────────────────────────────────┘
```

#### 按钮样式区分

- **普通选项：** `choiceOptionBtn` — 暗色边框，左侧文字对齐
- **战斗选项：** `combatOptionBtn` — 红色调边框，红色文字，加粗
- **控制按钮：** `gameCtrlBtn` — 小字体，右侧栏排列

#### 选项过滤逻辑

在 `showStoryNode()` 中，根据当前玩家职业动态过滤选项：
- 职业受限且玩家不匹配 → 跳过不生成按钮
- `choiceIndex` 属性与实际生成顺序对应，而非节点原始索引

### 存档对话框 (SaveLoadDialog)

**文件：** `ui/SaveLoadDialog.h`, `ui/SaveLoadDialog.cpp`

- **双模式：** `savingMode = true` 为保存模式，`false` 为读取模式
- **4 个槽位卡片：** Slot 0 自动存档 + Slot 1-3 手动存档
- **视觉状态：**
  - 空槽位 — 灰色文字 "战区通讯中断"
  - 有数据 — 显示玩家姓名、兵种、战役名、时间戳
  - 选中 — 红色边框高亮
- **防呆设计：**
  - 保存模式：自动存档位灰色禁用
  - 读取模式：空槽位灰色禁用
  - 删除按钮：仅非自动存档且有数据的槽位可用
- **检查操作数据：** 通过 `m_player` 指针（保存模式）写入当前玩家状态；读取模式通过 `gameLoaded` 信号回传

### 主窗口 (MainWindow)

**文件：** `ui/MainWindow.h`, `ui/MainWindow.cpp`

#### 核心职责

作为所有子部件的容器和信号路由中心：

```
MainWindow
  ├── QStackedWidget (页面容器)
  │   ├── [0] MenuWidget           — 主菜单
  │   ├── [1] CharacterCreateWidget — 角色创建
  │   └── [2] GameWidget           — 游戏主界面
  ├── GameEngine   — 游戏逻辑引擎
  ├── SaveManager  — 存档管理器
  └── MusicPlayer  — 音乐播放器
```

#### 信号路由表

| 发送方 | 信号 | MainWindow 处理 | 转发至 |
|--------|------|----------------|--------|
| MenuWidget | `newGameClicked` | `showCharacterCreate()` | CharacterCreateWidget |
| MenuWidget | `loadGameClicked` | `openLoadDialog()` | SaveLoadDialog |
| CharacterCreateWidget | `startGame` | `onStartGame()` → `engine->newGame()` + `startScenario()` | GameWidget |
| GameWidget | `choiceMade(index)` | `onChoiceMade()` | GameEngine |
| GameWidget | `saveClicked` | `openSaveDialog()` | SaveLoadDialog |
| GameWidget | `loadClicked` | `openLoadDialog()` | SaveLoadDialog |
| GameWidget | `exitClicked` | `showMainMenu()` | MenuWidget + 音乐切换 |
| GameEngine | `nodeChanged` | `onNodeChanged()` → 刷新界面 + 自动存档 | GameWidget |
| GameEngine | `statsChanged` | `onStatsChanged()` | GameWidget |
| GameEngine | `combatResult` | `onCombatResult()` → QMessageBox | 无 |
| GameEngine | `scenarioVictory` | `onScenarioVictory()` → 提示 + 存档 | 无 |
| GameEngine | `scenarioDefeat` | `onScenarioDefeat()` → 音乐切换 | MusicPlayer |

#### 战役间跳转逻辑

当玩家在非柏林战役中获胜时：
1. `Ending` 节点显示"【进入下一战役】"按钮
2. 点击触发 `choiceMade(-2)`
3. `MainWindow::onChoiceMade` 检测到 `-2`，计算 `currentScenario + 1` 作为下一个场景
4. 调用 `engine->startScenario(nextScen)` 进入下一战役
5. 柏林战役（最终章）的结束按钮直接返回主菜单

---

## 场景详情

### 黄色方案 (FallGelb)

**历史背景：** 1940 年 5-6 月，德国通过阿登森林突破法国防线，六周内迫使法国投降。

| 项目 | 详情 |
|------|------|
| **职业** | 步兵、坦克兵 |
| **节点数** | 28 个节点 |
| **通关路径** | 阿登行军 → 比利时村庄 → 马斯河渡口 → 法国色当 → 难民公路 → 敦刻尔克停止令 → 法国陷落 |
| **关键道德选择** | ① 村庄是否骚扰平民 ② 是否驱散法国难民 ③ 是否质疑敦刻尔克停止令 ④ 巴黎胜利后庆祝或独处反思 |
| **胜利结局** | 1 个（黄色方案完成 — 六周征服法国） |
| **失败结局** | 1 个（阵亡于法国农村） |
| **节点数** | 28 |

### 不列颠空战 (Britain)

**历史背景：** 1940 年 7-10 月，德国空军试图夺取英吉利海峡制空权，为入侵英国做准备但最终失败。

| 项目 | 详情 |
|------|------|
| **职业** | 战斗机飞行员、轰炸机飞行员 |
| **节点数** | 27 个节点 |
| **分支起点** | 战斗机/轰炸机各有独立的简报节点 |
| **关键道德选择** | ① 是否在不确定目标时依然投弹轰炸 ② 是否质疑轰炸伦敦平民的命令 ③ 燃油危机中的策略权衡 ④ RAF 大反攻中战斗或撤退 |
| **特殊节点** | 被击落 → 英国战俘营 → 换俘归队 |
| **胜利结局** | 2 个（生存 / 生存与反思） |
| **失败结局** | 1 个（坠机阵亡） |
| **节点数** | 27 |

### 群狼海战 (WolfPack)

**历史背景：** 1941-1943 年，大西洋潜艇战的高峰与衰退——德国 U 艇部队从"快乐时代"走向 75% 阵亡率的惨败。

| 项目 | 详情 |
|------|------|
| **职业** | 潜艇驾驶员、战列舰水兵 |
| **分支起点** | 潜艇简报（U-96）或战列舰简报（俾斯麦号） |
| **关键道德选择** | ① 是否救助商船幸存者（违反邓尼茨命令） ② 深水炸弹攻击下的生存策略 ③ 最终反思时诚实还是沉默 |
| **俾斯麦号线** | 击沉胡德号 → 被围猎 → 沉没/获救 |
| **胜利结局** | 2 个（诚实面对战争 vs 沉默） |
| **失败结局** | 1 个（葬身大西洋） |

### 斯大林格勒战役 (Stalingrad)

**历史背景：** 1942-1943 年，人类史上最惨烈的城市巷战，第六集团军被苏军包围合围并最终投降。

| 项目 | 详情 |
|------|------|
| **职业** | 步兵、坦克兵 |
| **节点数** | 27 个节点 |
| **关键选择** | ① 废墟巷战中的正面突击 vs 下水道迂回 ② 拖拉机厂攻防：强攻/炮火覆盖/迂回绕过 ③ 冬季饥饿：搜寻食物 vs 分享口粮 ④ 被包围后：突围 vs 等待解救 ⑤ 最后抉择：投降 vs 死战到底 |
| **胜利结局** | 2 个（放下武器投降 / 打完最后一颗子弹后被俘） |
| **失败结局** | 1 个（阵亡于废墟） |

### 柏林战役 (Berlin)

**历史背景：** 1945 年 4 月，苏联红军发起对柏林的最后攻势，德国无条件投降前夜的末日图景。

| 项目 | 详情 |
|------|------|
| **职业** | 全部六种职业可参与 |
| **分支起点** | 职业分配：步兵/坦克 → 奥德河防线、飞行兵 → 防空塔、海军 → 施普雷河防线 |
| **关键选择** | ① 不同职业的前线战斗 ② 遇到少年兵——劝其回家 / 命令坚守 / 视而不见 ③ 元首自杀后的最终抉择：向东投降 / 向西突围 / 隐蔽等待 |
| **胜利结局** | 3 个（向东投降、向西突围、隐蔽等待 → 全部存活） |
| **失败结局** | 1 个（突围失败阵亡） |
| **最终行为** | 柏林结局不显示"下一战役"按钮，而是"返回主页 — 铭记历史" |

---

## 美术风格

### 主题：军工战损风（Industrial War-Worn）

**文件：** `resources/style.qss`

全局采用**暗黑色系 + 暗红强调色**，模拟二战时期军工设施、战损金属的视觉质感：

- **背景色：** `#121212`（近乎纯黑，模拟暗夜或地下掩体）
- **前景文字：** `#e2e2e2`（灰白，模拟褪色军服）
- **强调色：** `#aa3333` / `#ff3333`（暗红，模拟血、铁锈、战火）
- **信息色：** `#ff9900`（橙色，用于关键信息标注）
- **HP 进度条：** `#992222`（血红）
- **士气进度条：** `#226699`（冷蓝，暗示心理状态）
- **战斗选项按钮：** 红色调背景 + 红色文字，视觉区分风险行动

### 设计原则

- **无圆角：** 所有 `border-radius: 0px`，体现军事工业的冷硬直线
- **暗色层次：** 通过 `#121212` → `#181818` → `#1c1c1c` → `#242424` 四层灰色建立纵深
- **等宽/工业字体：** font-family 末尾回退到 `monospace`，营造电报/打字机感
- **仅需 QSS：** 无外部图片依赖，纯文本 + 样式表实现所有视觉效果

---

## 数据流与生命周期

### 完整游戏流程

```
启动应用
    │
    ├─ main.cpp 创建 QApplication，加载 style.qss
    │
    ├─ MainWindow 构造
    │   ├─ 创建 GameEngine、SaveManager、MusicPlayer
    │   ├─ 构建 UI 页面栈（Menu → Create → Game）
    │   ├─ 连接所有信号槽
    │   ├─ 注册音乐轨道
    │   └─ 播放主菜单主题曲
    │
    ▼ 【主菜单】
    │
    ├─ "开始新的战役" → 角色创建页面
    │   ├─ 选择姓名 + 职业
    │   └─ 开始游戏 → GameEngine::newGame + startScenario
    │       ├─ 显示首个故事节点
    │       ├─ 打字机逐字播放文本
    │       └─ 等待玩家选择
    │
    ├─ "继续旧的战役" → 存档对话框（读取模式）
    │   ├─ 选择有数据的槽位
    │   └─ 加载 → GameEngine::loadGame → 导航到保存时的节点
    │
    ▼ 【游戏循环】
    │
    玩家点击选项
    │   ├─ 普通选项 → applyChoice → navigateTo → 下一个节点
    │   └─ 战斗选项 → rollDice 判定
    │       ├─ 成功 → applyChoice → navigateTo(成功节点)
    │       └─ 失败 → HP/士气下降 → dead? → 失败节点 : 失败跳转节点
    │
    每次节点切换：
    ├─ 刷新 UI（文本、选项、HP/士气条）
    ├─ 切换音乐（如果节点指定了 musicKey）
    └─ 自动存档 → SaveManager::autoSave
    │
    ▼ 【结束状态】
    │
    ├─ 胜利（非柏林） → "进入下一战役" → 下一个场景
    ├─ 胜利（柏林）   → "返回主页" → 主菜单
    └─ 失败           → "返回主页" → 主菜单
```

### 内存管理

- 所有 Qt 对象使用**父子所有权树**——`MainWindow` 是根，析构时自动释放所有子对象
- 场景对象使用 `std::unique_ptr<ScenarioBase>` 管理，所有权归 `GameEngine::m_scenarios`
- `SaveLoadDialog` 在栈上创建，`exec()` 模态阻塞，离开作用域自动析构

---

## 已知局限与未来改进

### 当前局限

1. **无国际化（i18n）：** 所有文本硬编码为中文，没有英文或其他语言版本
2. **音乐文件需手动准备：** 由于版权原因，`resources/music/` 目录下的 .mp3 文件未随源码分发
3. **窗口固定大小：** 最小 850×650，默认 960×700，未做响应式缩放
4. **UI 透明按钮覆盖层：** 角色创建和存档对话框中使用硬编码尺寸的透明 QPushButton 做卡片点击，窗口缩放时可能偏移
5. **无成就/统计系统：** 玩家的道德选择虽然留下标志，但没有统一的后续影响或成就解锁
6. **战斗选项的标志检查缺少 UI 反馈：** 如果玩家不满足前置标志或职业限制，选项会静默隐藏，不告知为什么不可选

### 可能的改进方向

- 添加英文翻译支持（使用 Qt Linguist 的 `.ts` 文件）
- 为音乐文件提供 `placeholder` 或静默模式
- 实现成就面板与进度统计
- 添加跳过已读文本的快速模式
- 允许自定义窗口缩放与全屏模式
- 扩展场景分支深度，让叙事标志更显著地影响后续剧情

---

## 致谢

本作受以下作品启发：
- 战史著作《第三帝国的兴亡》《斯大林格勒》《大西洋战役》
- 德剧《我们的父辈》(Unsere Mütter, unsere Väter)
- 电影《从海底出击》(Das Boot)、《帝国的毁灭》(Der Untergang)

---

*愿世界永无战争。*
