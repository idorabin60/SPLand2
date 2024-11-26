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
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation) {
    // Implement the act function as required
}

const string SimulateStep::toString() const {
    // Return a string representation of the SimulateStep
    return "SimulateStep: " + to_string(numOfSteps) + " steps.";
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}
