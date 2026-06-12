#pragma once
#include "../core/ScenarioBase.h"

class FallGelbScenario : public ScenarioBase {
public:
    void       initialize()              override;
    ScenarioId scenarioId()        const override { return ScenarioId::FallGelb; }
    QString    startNodeId(PlayerClass)  const override;
    QString    defeatNodeId()      const override { return QStringLiteral("fg_defeat"); }
};
