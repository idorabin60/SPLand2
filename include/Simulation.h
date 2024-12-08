#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "globals.h"

using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation
{
public:
    Simulation(const string &configFilePath); 
    void start();
    void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
    void addAction(BaseAction *action);
    bool addSettlement(Settlement *settlement);
    bool addFacility(FacilityType facility);
    bool isSettlementExists(const string &settlementName);
    bool isPlanIdExsits(const int planID);
    Settlement &getSettlement(const string &settlementName);
    Plan &getPlan(const int planID);
    void step();
    void close();
    void open();
    void parseConfigFile(const std::string &configFilePath);
    ~Simulation();
    SelectionPolicy *createSelectionPolicy(const std::string &policyType);
    std::vector<std::string> parseToWords(const std::string& input);
    void actionHandler(const std::string &action);
    void printLog() const;
    void backup();
    bool restore();
    //rule of 5
    Simulation(const Simulation &other);
    Simulation &operator=(const Simulation &other);
    Simulation(Simulation &&other) noexcept;
    Simulation &operator=(Simulation &&other) noexcept;

private:
    bool isRunning;
    int planCounter; // For assigning unique plan IDs
    vector<BaseAction *> actionsLog;
    vector<Plan> plans;
    vector<Settlement *> settlements;
    vector<FacilityType> facilitiesOptions;
    void parseConfig(const std::string &configFilePath);
    void handleSettlementCommand(const std::vector<std::string> &arguments);
    void handleFacilityCommand(const std::vector<std::string> &arguments);
    void handlePlanCommand(const std::vector<std::string> &arguments);
};