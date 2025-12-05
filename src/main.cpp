#include "dungeon.h"
#include "npc_factory.h"
#include "observer.h"
#include <iostream>

int main()
{
    Dungeon dungeon;

    auto consoleObs = std::make_shared<ConsoleObserver>();
    auto fileObs = std::make_shared<FileObserver>();

    dungeon.AddObserver(consoleObs);
    dungeon.AddObserver(fileObs);

    dungeon.AddNPC(NPCFactory::CreateNPC("Druid", "Galdor", 100, 100));
    dungeon.AddNPC(NPCFactory::CreateNPC("Orc", "Bash", 110, 100));
    dungeon.AddNPC(NPCFactory::CreateNPC("Slaver", "Ruk", 300, 300));
    dungeon.AddNPC(NPCFactory::CreateNPC("Druid", "Luthien", 310, 300));

    dungeon.Print();
    dungeon.SaveToFile("dungeon.txt");

    dungeon.Battle(50.0);

    dungeon.Print();

    return 0;
}