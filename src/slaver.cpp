#include "slaver.h"
#include "fight_visitor.h"

void Slaver::Accept(FightVisitor &visitor)
{
    visitor.Visit(*this);
}