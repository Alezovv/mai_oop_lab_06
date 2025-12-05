#include "dungeon.h"
#include "fight_visitor.h"
#include "npc_factory.h"
#include <algorithm>
#include <fstream>
#include <iostream>

void Dungeon::AddNPC(std::unique_ptr<NPC> npc)
{
    npcs_.push_back(std::move(npc));
}

void Dungeon::AddObserver(std::shared_ptr<IObserver> observer)
{
    observers_.push_back(observer);
}

void Dungeon::SaveToFile(const std::string &filename) const
{
    std::ofstream out(filename);
    for (const auto &npc : npcs_)
    {
        out << *npc << "\n";
    }
}

void Dungeon::LoadFromFile(const std::string &filename)
{
    std::ifstream in(filename);
    if (!in.is_open())
        throw std::runtime_error("File not found");

    npcs_.clear();
    std::string type, name;
    double x, y;
    while (in >> type >> name >> x >> y)
    {
        npcs_.push_back(NPCFactory::CreateNPC(type, name, x, y));
    }
}

void Dungeon::Print() const
{
    std::cout << "Dungeon Contents" << std::endl;
    for (const auto &npc : npcs_)
    {
        std::cout << npc->getType() << " " << npc->getName()
                  << " (" << npc->getX() << ", " << npc->getY() << ")" << std::endl;
    }
}

void Dungeon::Battle(double range)
{
    std::cout << "Battle Started (Range: " << range << ")" << std::endl;

    FightVisitor visitor(npcs_, range, observers_);

    for (auto &npc : npcs_)
    {
        if (npc->isAlive())
        {
            npc->Accept(visitor);
        }
    }

    auto newEnd = std::remove_if(npcs_.begin(), npcs_.end(),
                                 [](const std::unique_ptr<NPC> &n)
                                 { return !n->isAlive(); });
    npcs_.erase(newEnd, npcs_.end());

    std::cout << "Battle Ended" << std::endl;
}