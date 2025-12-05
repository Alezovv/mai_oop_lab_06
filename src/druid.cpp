#include "druid.h"
#include "fight_visitor.h"

void Druid::Accept(FightVisitor &visitor)
{
    visitor.Visit(*this);
}