#include "fight_visitor.h"
#include "utils.h"
#include "druid.h"
#include "orc.h"
#include "slaver.h"

FightVisitor::FightVisitor(std::vector<std::unique_ptr<NPC>> &npcs, double range, const std::vector<std::shared_ptr<IObserver>> &observers)
    : npcs_(npcs), range_(range), observers_(observers) {}

void FightVisitor::Notify(const NPC &attacker, const NPC &defender)
{
    for (const auto &obs : observers_)
    {
        obs->onKill(attacker.getName(), defender.getName());
    }
}

void FightVisitor::Visit(Druid &attacker)
{
}

void FightVisitor::Visit(Orc &attacker)
{
    for (auto &defender : npcs_)
    {
        if (defender.get() == &attacker || !defender->isAlive())
            continue;

        if (Distance(attacker.getX(), attacker.getY(), defender->getX(), defender->getY()) <= range_)
        {
            defender->kill();
            Notify(attacker, *defender);
        }
    }
}

void FightVisitor::Visit(Slaver &attacker)
{
    for (auto &defender : npcs_)
    {
        if (defender.get() == &attacker || !defender->isAlive())
            continue;

        if (dynamic_cast<Druid *>(defender.get()))
        {
            if (Distance(attacker.getX(), attacker.getY(), defender->getX(), defender->getY()) <= range_)
            {
                defender->kill();
                Notify(attacker, *defender);
            }
        }
    }
}