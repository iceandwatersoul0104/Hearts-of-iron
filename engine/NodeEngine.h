#pragma once
#include <QObject>
#include "DlcTypes.h"
#include "DiceSystem.h"
#include "PlayerSystem.h"

class NodeEngine : public QObject {
    Q_OBJECT
public:
    explicit NodeEngine(DiceSystem *dice, QObject *parent = nullptr);

    // 初始化：载入 DLC
    bool startDlc(const DlcManifest &manifest, const QString &dlcBasePath,
                  PlayerSystem &player);

    // 开始某一章节
    bool startChapter(const QString &chapterId);

    // 当前节点
    const StoryNode *currentNode() const;
    const DlcChapter *currentChapterData() const { return &m_currentChapter; }

    // 玩家选择（choiceIndex: 选项数组索引，-1 = Narrative的继续按钮）
    bool makeChoice(int choiceIndex);

    // 状态查询
    bool isGameOver() const;
    bool isVictory()  const;
    bool isInCombat() const;

signals:
    void nodeChanged(const StoryNode *node);
    void statsChanged(int hp, int morale);
    void chapterVictory(const QString &chapterId);
    void chapterDefeat(const QString &chapterId);
    void combatResult(bool success, int hpChange, int moraleChange);
    void flagSet(const QString &flag);

private:
    void navigateTo(const QString &nodeId);
    void applyChoice(const Choice &choice);
    bool loadChapterInternal(const QString &chapterId);

    DiceSystem *m_dice = nullptr;
    DlcManifest m_manifest;
    QString m_dlcBasePath;
    PlayerSystem *m_player = nullptr;
    DlcChapter m_currentChapter;
    QMap<QString, DlcChapter> m_loadedChapters;
};
