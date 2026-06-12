#pragma once
#include "../core/ScenarioBase.h"

class BerlinScenario : public ScenarioBase {
public:
    void       initialize()              override;
    ScenarioId scenarioId()        const override { return ScenarioId::Berlin; }
    QString    startNodeId(PlayerClass)  const override;
    QString    defeatNodeId()      const override { return QStringLiteral("be_captured"); }
};
