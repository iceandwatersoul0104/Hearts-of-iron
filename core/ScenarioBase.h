#pragma once
#include <QMap>
#include <QString>
#include "StoryNode.h"
#include "GameState.h"

// ============================================================
//  ScenarioBase — 场景抽象基类
//  每个具体场景继承此类并在 initialize() 中填充节点
// ============================================================
class ScenarioBase {
public:
    virtual ~ScenarioBase() = default;

    // 初始化所有节点（子类实现）
    virtual void initialize() = 0;

    // 获取场景 ID
    virtual ScenarioId scenarioId() const = 0;

    // 根据职业返回起始节点 ID
    virtual QString startNodeId(PlayerClass cls) const = 0;

    // 查找节点
    const StoryNode* getNode(const QString &id) const;
    bool hasNode(const QString &id) const;

    // 失败节点 ID（子类重写）
    virtual QString defeatNodeId() const { return QString(); }

protected:
    // 子类用此方法注册节点
    void addNode(const StoryNode &node);

    QMap<QString, StoryNode> m_nodes;
};
