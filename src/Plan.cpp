#include "Plan.h"
#include "Settlement.h"
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
Plan::Plan(const int planId,
           const Settlement &settlement,
           SelectionPolicy *selectionPolicy,
           const std::vector<FacilityType> &facilityOptions,
           int life_quality_score,
           int economy_score,
           int environment_score,
           std::vector<Facility *> facilities,
           std::vector<Facility *> underConstruction)
    : plan_id(planId),
      settlement(settlement),
      selectionPolicy(selectionPolicy),
      status(PlanStatus::AVALIABLE),
      facilityOptions(facilityOptions),
      life_quality_score(life_quality_score),
      economy_score(economy_score),
      environment_score(environment_score),
      facilities(std::move(facilities)),              // Move the ownership
      underConstruction(std::move(underConstruction)) // Move the ownership
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

const std::vector<Facility *> &Plan::getUnderConstruction() const
{
    return underConstruction;
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
    delete selectionPolicy; // Free the old memory
    selectionPolicy = newSelectionPolicy;
}
void Plan::step()
{
    if (status == PlanStatus::BUSY)
    {
        // If the status of the plan is busy, iterate through all facilities and step them
        for (int i = 0; i < underConstruction.size(); ++i)
        {
            if (underConstruction[i]->step() == FacilityStatus::OPERATIONAL)
            {
                facilities.push_back(underConstruction[i]); // Move the facility to operational list
                                                            // update the scores
                life_quality_score += underConstruction[i]->getLifeQualityScore();
                economy_score += underConstruction[i]->getEconomyScore();
                environment_score += underConstruction[i]->getEnvironmentScore();
                underConstruction.erase(underConstruction.begin() + i); // Remove the facility from underConstruction
                --i;                                                    // Decrement to avoid skipping the next element after erase
            }
        }
    }
    else
    { // The status is available
        int facility_capacity = static_cast<int>(settlement.getType()) + 1 - underConstruction.size();
        for (int i = 0; i < facility_capacity; i++)
        {
            Facility *new_facility = new Facility(
                selectionPolicy->selectFacility(facilityOptions),
                settlement.getName());
            underConstruction.push_back(new_facility);
        }
    }

    // Update plan status
    if ((int)underConstruction.size() != static_cast<int>(settlement.getType()) + 1)
    {
        status = PlanStatus::AVALIABLE;
    }
    else
    {
        status = PlanStatus::BUSY;
    }
}

// Convert Plan object to a string representation
const std::string Plan::toString() const
{
    std::ostringstream oss;
    oss << "PlanID: " << plan_id << "\n";
    oss << "SettlementName" << settlement.getName() << "\n";
    oss << "SelectionPolicy" << selectionPolicy->toString() << "\n";
    oss << "PlanStatus: " << (status == PlanStatus::AVALIABLE ? "Available" : "Busy") << "\n";
    oss << "Life Quality Score: " << life_quality_score << "\n";
    oss << "Economy Score: " << economy_score << "\n";
    oss << "Environment Score: " << environment_score << "\n";
    oss << "Facilities: " << facilities.size() << " completed\n";
    oss << "Under Construction: " << underConstruction.size() << " facilities:\n";
    for (Facility *fac : underConstruction)
    {
        oss << fac->getName() << "\n";
    }
    return oss.str();
}
Plan::Plan(const Plan &other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy ? other.selectionPolicy->clone() : nullptr), // Deep copy selectionPolicy
      status(other.status),
      facilityOptions(other.facilityOptions),
      facilities(),
      underConstruction(),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score)
{
    // Deep copy the facilities vector
    for (Facility *facility : other.facilities)
    {
        facilities.push_back(facility->clone());
    }

    // Deep copy the underConstruction vector
    for (Facility *facility : other.underConstruction)
    {
        underConstruction.push_back(facility->clone());
    }
}

Plan::Plan(Plan &&other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy),
      facilities(std::move(other.facilities)),
      underConstruction(std::move(other.underConstruction)),
      facilityOptions(other.facilityOptions),
      status(other.status),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score)
{
    other.selectionPolicy = nullptr;
    other.facilities.clear();
    other.underConstruction.clear();
}

Plan::~Plan()
{
    delete selectionPolicy;

    for (Facility *facility : facilities)
    {
        delete facility;
    }
    facilities.clear(); // Clear the vector to avoid dangling pointers

    for (Facility *facilityUnderConstructions : underConstruction)
    {
        delete facilityUnderConstructions;
    }
    underConstruction.clear(); // Clear the vector to avoid dangling pointers
}

const std::string &Plan::getSettlement() const
{
    if (&settlement == nullptr)
    {
        throw std::runtime_error("Invalid Settlement reference in Plan.");
    }
    return settlement.getName();
}

const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}

int Plan::getPlanId() const
{
    return plan_id;
}
SelectionPolicy *Plan::getSelectionPolicy() const
{
    return selectionPolicy;
}