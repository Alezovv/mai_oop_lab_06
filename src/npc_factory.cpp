#include <stdexcept>
#include <algorithm>

#include "npc_factory.h"
#include "druid.h"
#include "orc.h"
#include "slaver.h"

std::string ToLower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

std::unique_ptr<NPC> NPCFactory::CreateNPC(const std::string &type, const std::string &name, double x, double y)
{
    if (x <= 0 || x > 500 || y < 0 || y > 500) {
            throw std::invalid_argument("Invalid NPC coordinates");
    
        }
    std::string t = ToLower(type);
    if (t == "druid")
        return std::make_unique<Druid>(name, x, y);
    if (t == "orc")
        return std::make_unique<Orc>(name, x, y);
    if (t == "slaver")
        return std::make_unique<Slaver>(name, x, y);

    throw std::runtime_error("Unknown NPC type: " + type);
}