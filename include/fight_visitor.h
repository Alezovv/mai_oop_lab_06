#pragma once

#include <vector>
#include <memory>

#include "npc.h"
#include "observer.h"

class Druid;
class Orc;
class Slaver;

class FightVisitor
{
public:
    FightVisitor(std::vector<std::unique_ptr<NPC>> &npcs, double range, const std::vector<std::shared_ptr<IObserver>> &observers);

    void Visit(Druid &attacker);
    void Visit(Orc &attacker);
    void Visit(Slaver &attacker);

private:
    std::vector<std::unique_ptr<NPC>> &npcs_;
    double range_;
    const std::vector<std::shared_ptr<IObserver>> &observers_;

    void Notify(const NPC &attacker, const NPC &defender);
};