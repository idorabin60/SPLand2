#include "Action.h"
#include "Simulation.h"
#include <iostream>
#include <sstream>
using namespace std;

// Constructor and generic methods
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::ERROR) {}
void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    this->errorMsg = std::move(errorMsg);
    status = ActionStatus::ERROR;
    std::cout << "Error: " << this->errorMsg << std::endl;
}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
}

//--------------------------//////

// SimulateStep Implementation
SimulateStep::SimulateStep(const int numOfSteps) : BaseAction(), numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation)
{
    for (int i = 1; i <= numOfSteps; i++)
    {
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const
{
    if (getStatus() == ActionStatus::ERROR)
        return ("Action: Step " + to_string(numOfSteps) + " ERROR");
    else
        return ("Action: Step " + to_string(numOfSteps) + " COMPLETED");
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

//--------------------------//////
// PrintPlanStatus Implementation
PrintPlanStatus::PrintPlanStatus(int planId)
    : BaseAction(), planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation)
{
    Plan this_plan = simulation.getPlan(planId);
    std::cout << this_plan.toString() << std::endl;
    ;
    complete();
}
PrintPlanStatus *PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this);
}
const string PrintPlanStatus::toString() const
{
    if (getStatus() == ActionStatus::ERROR)
        return ("Action: PrintPlanStatus of Plan" + to_string(planId) + " ERROR");
    else
        return ("Action: PrintPlanStatus of Plan" + to_string(planId) + +" COMPLETED");
}

//--------------------------//////
// AddPlan Implementation
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
    : BaseAction(), settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation)
{
    Settlement &settlement_to_addPlan = simulation.getSettlement(settlementName);
    SelectionPolicy *wanted_policy = nullptr;
    if (selectionPolicy == "bal")
        wanted_policy = new BalancedSelection(0, 0, 0);
    else if (selectionPolicy == "eco")
        wanted_policy = new EconomySelection();
    else if (selectionPolicy == "sus")
        wanted_policy = new SustainabilitySelection();
    else
        wanted_policy = new NaiveSelection();

    simulation.addPlan(settlement_to_addPlan, wanted_policy);
    complete();
}

const string AddPlan::toString() const
{

    if (getStatus() == ActionStatus::ERROR)
        return ("Action: AddPlan ERROR");
    else
        return ("Action: AddPlan COMPLETED");
}

AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this);
}

//--------------------------//////
// AddSettlement Implementation

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType)
    : BaseAction(), settlementName(settlementName), settlementType(settlementType)
{
}

void AddSettlement::act(Simulation &simulation)
{
    if (!simulation.isSettlementExists(settlementName))
    {
        Settlement *new_settlent = new Settlement(settlementName, settlementType);
        if (simulation.addSettlement(new_settlent))
            complete();
        else
            error("Settlement already exsite");
    }
    else
    {
        error("Settlement already exsite");
    }
}
AddSettlement *AddSettlement::clone() const
{
    return new AddSettlement(*this);
}
const string AddSettlement::toString() const
{
    if (getStatus() == ActionStatus::ERROR)
        return ("Action: AddSettlement ERROR!");
    else
        return ("Action: AddSettlement COMPLETED!");
}

//--------------------------//////
// AddFacility Implementation
AddFacility::AddFacility(const string &facilityName,
                         const FacilityCategory facilityCategory,
                         const int price,
                         const int lifeQualityScore,
                         const int economyScore,
                         const int environmentScore)
    : BaseAction(), // Calling the BaseAction constructor
      facilityName(facilityName),
      facilityCategory(facilityCategory),
      price(price),
      lifeQualityScore(lifeQualityScore),
      economyScore(economyScore),
      environmentScore(environmentScore)
{
}

void AddFacility::act(Simulation &simulation)
{
    FacilityType new_facility = FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if (simulation.addFacility(new_facility))
        complete();
    else
        std::cout << "Error!!" << std::endl;
}
AddFacility *AddFacility::clone() const
{
    return new AddFacility(*this);
}
const string AddFacility::toString() const
{
    if (getStatus() == ActionStatus::ERROR)
        return ("Action: AddFacility ERROR!");
    else
        return ("Action: AddFacility COMPLETED!");
}

//--------------------------//////
// ChangePlanPolicy Implementation

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy)
    : BaseAction(), planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation)
{
    Plan &to_change = simulation.getPlan(planId);
    SelectionPolicy *wanted_policy = nullptr;
    if (newPolicy == "bal")
        wanted_policy = new BalancedSelection(to_change.getlifeQualityScore(), to_change.getEconomyScore(), to_change.getEnvironmentScore());
    else if (newPolicy == "eco")
        wanted_policy = new EconomySelection();
    else if (newPolicy == "sus")
        wanted_policy = new SustainabilitySelection();
    else
        wanted_policy = new NaiveSelection();
    to_change.setSelectionPolicy(wanted_policy);
}
ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}
const string ChangePlanPolicy::toString() const
{
    if (getStatus() == ActionStatus::ERROR)
        return ("Action: ChangePlanPolicy ERROR!");
    else
        return ("Action: ChangePlanPolicy COMPLETED!");
}

//--------------------------//////
// PrintActionsLog Implementation

PrintActionsLog::PrintActionsLog() : BaseAction() {}

void PrintActionsLog::act(Simulation &simulation)
{
    simulation.printLog();
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}
const string PrintActionsLog::toString() const
{
    if (getStatus() == ActionStatus::ERROR)
        return ("Action: PrintActionsLog ERROR!");
    else
        return ("Action: PrintActionsLog COMPLETED!");
}

// Default Constructor
BackupSimulation::BackupSimulation() {}

// Action method
void BackupSimulation::act(Simulation &simulation)
{
    simulation.backup();
    complete(); // Mark the action as complete
}

// Clone method
BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation(*this); // Create a copy of the object
}

// Convert to string method
const string BackupSimulation::toString() const
{
    if (getStatus() == ActionStatus::ERROR)
        return ("Action: BackupSimulation ERROR!");
    else
        return ("Action: BackupSimulation COMPLETED!");
}

// Default Constructor
RestoreSimulation::RestoreSimulation() {}

// Action method
void RestoreSimulation::act(Simulation &simulation)
{
    simulation.restore();
    complete();
}

// Clone method
RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this); // Create a copy of the object
}

// Convert to string method
const string RestoreSimulation::toString() const
{
    if (getStatus() == ActionStatus::ERROR)
        return ("Action: RestoreSimulation ERROR!");
    else
        return ("Action: RestoreSimulation COMPLETED!");
}