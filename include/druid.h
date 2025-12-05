#pragma once

#include "npc.h"

class Druid : public NPC
{
public:
    Druid(std::string name, double x, double y) : NPC("Druid", std::move(name), x, y) {}
    void Accept(FightVisitor &visitor) override;
};