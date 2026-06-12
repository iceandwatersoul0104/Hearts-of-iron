#pragma once
#include "../core/ScenarioBase.h"

class BritainScenario : public ScenarioBase {
public:
    void       initialize()              override;
    ScenarioId scenarioId()        const override { return ScenarioId::Britain; }
    QString    startNodeId(PlayerClass)  const override;
    QString    defeatNodeId()      const override { return QStringLiteral("br_final_crash"); }
};
