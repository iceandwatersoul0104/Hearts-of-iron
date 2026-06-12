#include "ScenarioBase.h"

const StoryNode* ScenarioBase::getNode(const QString &id) const {
    auto it = m_nodes.find(id);
    if (it != m_nodes.end())
        return &it.value();
    return nullptr;
}

bool ScenarioBase::hasNode(const QString &id) const {
    return m_nodes.contains(id);
}

void ScenarioBase::addNode(const StoryNode &node) {
    m_nodes[node.id] = node;
}
