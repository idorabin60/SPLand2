#include "Action.h"
#include <iostream>
#include <sstream>
using namespace std;

// Constructor and generic methods
BaseAction::BaseAction() : status(ActionStatus::ERROR), errorMsg("") {}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    this->errorMsg = std::move(errorMsg);
    status = ActionStatus::ERROR;
    std::cout << "Error: " << this->errorMsg << std::endl;
}

ActionStatus BaseAction::getStatus() const {
    return status;
}

const string &BaseAction::getErrorMsg() const {
    return errorMsg;
}

//--------------------------//////

// SimulateStep Implementation
SimulateStep::SimulateStep(const int numOfSteps) : BaseAction(), numOfSteps(numOfSteps){}

void SimulateStep::act(Simulation &simulation) {
    for (int i=0; i<numOfSteps;i++){
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const {
    return ("STEP" + to_string(numOfSteps) + to_string(getStatus());
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}

//--------------------------//////
// AddPlan Implementation
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
    : BaseAction(), settlementName(settlementName), selectionPolicy(selectionPolicy) {}

    void AddPlan::act(Simulation &simulation) {
        Settlement settlement_to_addPlan = simulation.getSettlement(settlementName);
         BalancedSelection wanted_policy;
        if (selectionPolicy=="bal")
           wanted_policy = BalancedSelection(0,0,0);
        if(selectionPolicy=="eco")
           wanted_policy = EconomySelection();
        if (selectionPolicy=="sus")
            wanted_policy = SustainabilitySelection();
        else
           wanted_policy = NaiveSelection();
        
        simulation.addPlan(settlement_to_addPlan, wanted_policy);
        complete();
}

const string AddPlan::toString() const {
    stringstream ss;
    ss << "AddPlan - Settlement: " << settlementName 
       << ", Selection Policy: " << selectionPolicy << to_string(getStatus();
    return ss.str();
}

AddPlan* AddPlan::clone() const {
    return new AddPlan(*this);
}

//--------------------------//////
// AddSettlement Implementation

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType)
    : BaseAction(), settlementName(settlementName), settlementType(settlementType) {
        erroeMsg="Settlement already exsite";
    }

void AddSettlement::act(Simulation &simulation) {
    if(simulation.isSettlementExists(settlementName)){
        Settlement new_settlent = Settlement(settlementName, settlementType);
        simulation.addSettlement(new_settlent);
        complete();
    }
    else{
        error();
    }     
}
AddSettlement* AddSettlement::clone() const {
    return new AddSettlement(*this);  
}
const string AddSettlement::toString() const {
    stringstream ss;
    ss << "AddSettlement - Name: " << settlementName 
       << ", Type: " << static_cast<int>(settlementType) << to_string(getStatus();
    return ss.str();
}

//--------------------------//////
// AddFacility Implementation

