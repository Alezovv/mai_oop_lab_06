#pragma once

#include "npc.h"

class Orc : public NPC
{
public:
    Orc(std::string name, double x, double y) : NPC("Orc", std::move(name), x, y) {}
    void Accept(FightVisitor &visitor) override;
};