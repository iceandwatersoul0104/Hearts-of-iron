#pragma once
#include "../core/ScenarioBase.h"

class WolfPackScenario : public ScenarioBase {
public:
    void       initialize()              override;
    ScenarioId scenarioId()        const override { return ScenarioId::WolfPack; }
    QString    startNodeId(PlayerClass)  const override;
    QString    defeatNodeId()      const override { return QStringLiteral("wp_defeat"); }
};
