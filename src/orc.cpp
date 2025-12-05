#include "orc.h"
#include "fight_visitor.h"

void Orc::Accept(FightVisitor &visitor)
{
    visitor.Visit(*this);
}