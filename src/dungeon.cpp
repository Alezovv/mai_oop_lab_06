#include "dungeon.h"
#include "fight_visitor.h"
#include "npc_factory.h"
#include <algorithm>
#include <fstream>
#include <iostream>

bool Dungeon::IsNameUnique(const std::string& name) const
{
    for (const auto& npc : npcs_)
    {
        if (npc->getName() == name)
            return false;
    }
    return true; 
}

void Dungeon::AddNPC(std::unique_ptr<NPC> npc)
{
    // Проверка уникальности имени
    if (!IsNameUnique(npc->getName()))
    {
        throw std::runtime_error(
            "NPC with name '" + npc->getName() + "' already exists"
        );
    }

    double x = npc->getX();
    double y = npc->getY();
    
    if (x <= 0 || x > 500 || y < 0 || y > 500)
    {
        throw std::runtime_error(
            "NPC coordinates out of bounds: (" + 
            std::to_string(x) + ", " + std::to_string(y) + 
            "). Must be: 0 < x <= 500, 0 <= y <= 500"
        );
    }
    
    npcs_.push_back(std::move(npc));
}

void Dungeon::AddObserver(std::shared_ptr<IObserver> observer)
{
    observers_.push_back(observer);
}

void Dungeon::SaveToFile(const std::string &filename) const // Сохранение и загрузка из файла
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
        if (x <= 0 || x > 500 || y < 0 || y > 500)
        {
            std::cerr << "Warning: NPC " << name 
                      << " has invalid coordinates (" 
                      << x << ", " << y << "). Skipping." << std::endl;
            continue;
        }
        
        npcs_.push_back(NPCFactory::CreateNPC(type, name, x, y));
    }
}

void Dungeon::Print() const // Печать перечня объектов
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

    FightVisitor visitor(npcs_, range, observers_); // Боевой режим с дальностью

    for (auto &npc : npcs_) // NPC сражаются «каждый с каждым»
    {
        if (npc->isAlive())
        {
            npc->Accept(visitor);
        }
    }

    auto newEnd = std::remove_if(npcs_.begin(), npcs_.end(),
                                 [](const std::unique_ptr<NPC> &n)
                                 { return !n->isAlive(); }); // Проигравший NPC удаляется
    npcs_.erase(newEnd, npcs_.end());

    std::cout << "Battle Ended" << std::endl;
}