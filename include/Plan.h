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
    Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
    Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions, int life_quality_score, int economy_score, int environment_score, vector<Facility *> facilities, vector<Facility *> underConstruction);

    const int getlifeQualityScore() const;
    const int getEconomyScore() const;
    const int getEnvironmentScore() const;
    void setSelectionPolicy(SelectionPolicy *selectionPolicy);
    void step();
    void printStatus();

    const vector<Facility *> &getFacilities() const; // Corrected
    const std::vector<Facility *> &getUnderConstruction() const;

    void addFacility(Facility *facility);
    const string toString() const;

    Plan(const Plan &other);
    ~Plan();
    Plan(Plan &&other);
    Plan &operator=(const Plan &other) = delete;
    Plan &operator=(Plan &&other) = delete;

    const string &getSettlement() const;
    int getPlanId() const;
    SelectionPolicy *getSelectionPolicy() const;

private:
    int plan_id;
    const Settlement &settlement;
    SelectionPolicy *selectionPolicy;
    PlanStatus status;
    vector<Facility *> facilities;
    vector<Facility *> underConstruction;
    const vector<FacilityType> &facilityOptions;
    int life_quality_score, economy_score, environment_score;
};
