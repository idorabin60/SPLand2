#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus
{
    AVALIABLE,
    BUSY,
};

class Plan
{
public:
    Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions); // done
    const int getlifeQualityScore() const;                                                                                               // done
    const int getEconomyScore() const;                                                                                                   // done
    const int getEnvironmentScore() const;                                                                                               // done
    void setSelectionPolicy(SelectionPolicy *selectionPolicy);
    void step();
    void printStatus();                              // done
    const vector<Facility *> &getFacilities() const; // pending
    void addFacility(Facility *facility);            // pending
    const string toString() const;                   // done

private:
    int plan_id;
    const Settlement &settlement;
    SelectionPolicy *selectionPolicy; // What happens if we change this to a reference?
    PlanStatus status;
    vector<Facility *> facilities;
    vector<Facility *> underConstruction;
    const vector<FacilityType> &facilityOptions;
    int life_quality_score, economy_score, environment_score;
};