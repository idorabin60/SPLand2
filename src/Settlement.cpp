#include "Settlement.h" // Assuming the header file is named "Settlement.h"
#include <iostream>
#include <sstream> // For string formatting in toString()

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

// Get a string representation of the settlement
const string Settlement::toString() const
{
    string typeStr;

    // Map the SettlementType enum to a string
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

    // Use simple concatenation to construct the output
    return "Settlement Name: " + name + "\n" + "Settlement Type: " + typeStr + "\n";
}

