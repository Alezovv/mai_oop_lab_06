#pragma once

#include <memory>
#include <vector>
#include <string>

#include "npc.h"
#include "observer.h"

class Dungeon
{
public:
    bool IsNameUnique(const std::string& name) const;
    void AddNPC(std::unique_ptr<NPC> npc);
    void AddObserver(std::shared_ptr<IObserver> observer);

    void SaveToFile(const std::string &filename) const;
    void LoadFromFile(const std::string &filename);
    void Print() const;

    void Battle(double range);

private:
    std::vector<std::unique_ptr<NPC>> npcs_;
    std::vector<std::shared_ptr<IObserver>> observers_;
};