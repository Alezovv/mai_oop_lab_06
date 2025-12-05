#include "npc.h"

NPC::NPC(std::string type, std::string name, double x, double y)
    : type_(std::move(type)), name_(std::move(name)), x_(x), y_(y), alive_(true) {}

std::string NPC::getType() const { return type_; }
std::string NPC::getName() const { return name_; }
double NPC::getX() const { return x_; }
double NPC::getY() const { return y_; }
bool NPC::isAlive() const { return alive_; }

void NPC::kill()
{
    alive_ = false;
}

std::ostream &operator<<(std::ostream &os, const NPC &npc)
{
    os << npc.type_ << " " << npc.name_ << " " << npc.x_ << " " << npc.y_;
    return os;
}