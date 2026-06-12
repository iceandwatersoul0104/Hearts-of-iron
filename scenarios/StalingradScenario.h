#pragma once
#include "../core/ScenarioBase.h"

class StalingradScenario : public ScenarioBase {
public:
    void       initialize()              override;
    ScenarioId scenarioId()        const override { return ScenarioId::Stalingrad; }
    QString    startNodeId(PlayerClass)  const override;
    QString    defeatNodeId()      const override { return QStringLiteral("st_defeat"); }
};
