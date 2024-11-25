#include "Plan.h"
#include <iostream>
#include <sstream>

// Constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(settlement),
      selectionPolicy(selectionPolicy),
      status(PlanStatus::AVALIABLE),
      facilityOptions(facilityOptions),
      life_quality_score(0),
      economy_score(0),
      environment_score(0) {}
const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}
const int Plan::getEnvironmentScore() const
{
    return environment_score;
}
const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}
// Get all facilities
const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}
void Plan::addFacility(Facility *facility)
{
    // What should happen when we add facility?
}
void Plan::printStatus()
{
    switch (status)
    {
    case PlanStatus::AVALIABLE:
        std::cout << "Available";
        break;
    case PlanStatus::BUSY:
        std::cout << "Busy";
        break;
    default:
        std::cout << "Unknown";
        break;
    }
}
