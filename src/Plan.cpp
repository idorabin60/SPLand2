#include "Plan.h"
#include <iostream>
#include <sstream>

Plan::Plan(const int planId,
           const Settlement &settlement,
           SelectionPolicy *selectionPolicy,
           const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(settlement),
      selectionPolicy(selectionPolicy),
      status(PlanStatus::AVALIABLE),
      facilityOptions(facilityOptions), // Initialize reference to facilityOptions
      facilities(),                     // Explicitly initialize as empty (optional, default behavior)
      underConstruction(),              // Explicitly initialize as empty (optional, default behavior)
      life_quality_score(0),
      economy_score(0),
      environment_score(0)
{
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}
const int Plan::getEnvironmentScore() const
{
    return environment_score;
}
const int Plan::getEconomyScore() const
{
    return economy_score;
}
// Get all completed facillities
const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}
void Plan::addFacility(Facility *facility)
{
    if (facility->getStatus() == FacilityStatus::OPERATIONAL)
    {
        facilities.push_back(facility);
    }
    else
    {
        underConstruction.push_back(facility);
    }
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
void Plan::setSelectionPolicy(SelectionPolicy *newSelectionPolicy)
{
    selectionPolicy = newSelectionPolicy;
}

// Convert Plan object to a string representation
const std::string Plan::toString() const
{
    std::ostringstream oss;
    oss << "Plan ID: " << plan_id << "\n";
    oss << "Status: " << (status == PlanStatus::AVALIABLE ? "Available" : "Busy") << "\n";
    oss << "Life Quality Score: " << life_quality_score << "\n";
    oss << "Economy Score: " << economy_score << "\n";
    oss << "Environment Score: " << environment_score << "\n";
    oss << "Facilities: " << facilities.size() << " completed\n";
    oss << "Under Construction: " << underConstruction.size() << " facilities\n";
    return oss.str();
}
