#pragma once

#include "npc.h"

class Slaver : public NPC
{
public:
    Slaver(std::string name, double x, double y) : NPC("Slaver", std::move(name), x, y) {}
    void Accept(FightVisitor &visitor) override;
};