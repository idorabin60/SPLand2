#include "Settlement.h" 
#include <iostream>
#include <sstream> 

using namespace std;

// Constructor
Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

// Get the name of the settlement
const string &Settlement::getName() const
{
    return name;
}

// Get the type of the settlement
SettlementType Settlement::getType() const
{
    return type;
}

const string Settlement::toString() const
{
    string typeStr;
    if (type == SettlementType::VILLAGE)
    {
        typeStr = "Village";
    }
    else if (type == SettlementType::CITY)
    {
        typeStr = "City";
    }
    else if (type == SettlementType::METROPOLIS)
    {
        typeStr = "Metropolis";
    }
    return "Settlement Name: " + name + "\n" + "Settlement Type: " + typeStr + "\n";
}
