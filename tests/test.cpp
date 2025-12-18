#include <gtest/gtest.h>

#include <memory>
#include <vector>
#include <fstream>

#include "npc.h"
#include "druid.h"
#include "orc.h"
#include "slaver.h"
#include "npc_factory.h"
#include "fight_visitor.h"
#include "dungeon.h"
#include "observer.h"

class MockObserver : public IObserver
{
public:
    std::vector<std::pair<std::string, std::string>> killLog;

    void onKill(const std::string &killer, const std::string &victim) override
    {
        killLog.push_back({killer, victim});
    }
};

TEST(NPCFactoryTest, CreatesCorrectTypes)
{
    auto npc1 = NPCFactory::CreateNPC("druid", "Elf", 10, 10);
    EXPECT_EQ(npc1->getType(), "Druid");
    EXPECT_EQ(npc1->getName(), "Elf");

    auto npc2 = NPCFactory::CreateNPC("orc", "Zog", 20, 20);
    EXPECT_EQ(npc2->getType(), "Orc");

    auto npc3 = NPCFactory::CreateNPC("slaver", "Bob", 30, 30);
    EXPECT_EQ(npc3->getType(), "Slaver");
}

TEST(BattleTest, OrcKillsDruidInRange)
{
    std::vector<std::unique_ptr<NPC>> npcs;
    std::vector<std::shared_ptr<IObserver>> observers;

    npcs.push_back(std::make_unique<Orc>("Killer", 0, 0));
    npcs.push_back(std::make_unique<Druid>("Victim", 10, 0));

    FightVisitor visitor(npcs, 20.0, observers);

    npcs[0]->Accept(visitor);

    EXPECT_TRUE(npcs[0]->isAlive());
    EXPECT_FALSE(npcs[1]->isAlive());
}

TEST(BattleTest, OrcMissesDruidOutOfRange)
{
    std::vector<std::unique_ptr<NPC>> npcs;
    std::vector<std::shared_ptr<IObserver>> observers;

    npcs.push_back(std::make_unique<Orc>("Killer", 0, 0));
    npcs.push_back(std::make_unique<Druid>("Lucky", 100, 0));

    FightVisitor visitor(npcs, 50.0, observers);

    npcs[0]->Accept(visitor);

    EXPECT_TRUE(npcs[1]->isAlive());
}

TEST(BattleTest, SlaverKillsDruid)
{
    std::vector<std::unique_ptr<NPC>> npcs;
    std::vector<std::shared_ptr<IObserver>> observers;

    npcs.push_back(std::make_unique<Slaver>("Trader", 0, 0));
    npcs.push_back(std::make_unique<Druid>("Victim", 5, 5));

    FightVisitor visitor(npcs, 10.0, observers);

    npcs[0]->Accept(visitor);

    EXPECT_FALSE(npcs[1]->isAlive());
}

TEST(BattleTest, SlaverDoesNotKillOrc)
{
    std::vector<std::unique_ptr<NPC>> npcs;
    std::vector<std::shared_ptr<IObserver>> observers;

    npcs.push_back(std::make_unique<Slaver>("Trader", 0, 0));
    npcs.push_back(std::make_unique<Orc>("StrongOne", 5, 5));

    FightVisitor visitor(npcs, 10.0, observers);

    npcs[0]->Accept(visitor);

    EXPECT_TRUE(npcs[1]->isAlive());
}

TEST(BattleTest, DruidIsPeaceful)
{
    std::vector<std::unique_ptr<NPC>> npcs;
    std::vector<std::shared_ptr<IObserver>> observers;

    npcs.push_back(std::make_unique<Druid>("Hippie", 0, 0));
    npcs.push_back(std::make_unique<Orc>("Enemy", 5, 5));

    FightVisitor visitor(npcs, 100.0, observers);

    npcs[0]->Accept(visitor);

    EXPECT_TRUE(npcs[1]->isAlive());
}

TEST(IOTest, SaveAndLoadPreservesData)
{
    std::string filename = "test_dungeon.txt";

    {
        Dungeon d1;
        d1.AddNPC(NPCFactory::CreateNPC("Orc", "Original", 100, 200));
        d1.SaveToFile(filename);
    }

    Dungeon d2;
    d2.LoadFromFile(filename);

    d2.AddNPC(NPCFactory::CreateNPC("Druid", "Victim", 101, 200));
    auto mockObs = std::make_shared<MockObserver>();
    d2.AddObserver(mockObs);

    d2.Battle(10.0);

    ASSERT_EQ(mockObs->killLog.size(), 1);
    EXPECT_EQ(mockObs->killLog[0].first, "Original");

    std::remove(filename.c_str());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}