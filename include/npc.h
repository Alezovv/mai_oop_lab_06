#pragma once

#include <string>
#include <memory>
#include <iostream>

class FightVisitor;

class NPC
{
public:
    NPC(std::string type, std::string name, double x, double y);
    virtual ~NPC() = default;

    std::string getType() const;
    std::string getName() const;
    double getX() const;
    double getY() const;

    bool isAlive() const;
    void kill();

    virtual void Accept(FightVisitor &visitor) = 0;

    // Для сохранения
    friend std::ostream &operator<<(std::ostream &os, const NPC &npc);

protected:
    std::string type_;
    std::string name_;
    double x_;
    double y_;
    bool alive_;
};