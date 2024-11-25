#include "Facility.h"
#include <iostream>
#include <sstream> // For string formatting in toString()

using namespace std;

// Constructor
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score) {}

const string &FacilityType::getName() const
{
    return name;
}
int FacilityType::getCost() const
{
    return price;
}
int FacilityType::getLifeQualityScore() const
{
    return lifeQuality_score;
}
int FacilityType::getEnvironmentScore() const
{
    return environment_score;
}
int FacilityType::getEconomyScore() const
{
    return economy_score;
}
FacilityCategory FacilityType::getCategory() const
{
    return category;
}

// class Facility
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category,
                   const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
      settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price) {}

Facility::Facility(const FacilityType &type, const string &settlementName)
    : FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price) {}

const string &Facility::getSettlementName() const
{
    return settlementName;
}

const int Facility::getTimeLeft() const
{
    return timeLeft;
}

FacilityStatus Facility::step()
{
    if (timeLeft > 0)
    {
        timeLeft--; // Reduce timeLeft
    }
    else
    {
        status = FacilityStatus::OPERATIONAL;
    }
    return status;
}

void Facility::setStatus(FacilityStatus newStatus)
{
    status = newStatus;
}

const FacilityStatus &Facility::getStatus() const
{
    return status;
}
std::string FacilityStatusToString(FacilityStatus status)
{
    switch (status)
    {
    case FacilityStatus::UNDER_CONSTRUCTIONS:
        return "Under Construction";
    case FacilityStatus::OPERATIONAL:
        return "Operational";
    // Add other cases if there are more FacilityStatus values
    default:
        return "Unknown Status";
    }
}

const string Facility::toString() const
{
    std::ostringstream oss;
    oss << "Facility: " << getName()
        << ", Settlement: " << getSettlementName()
        << ", Status: " << FacilityStatusToString(getStatus())
        << ", Time Left: " << getTimeLeft();
    return oss.str();
}
