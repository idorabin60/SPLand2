#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation
{
public:
    Simulation(const string &configFilePath); // done
    void start();
    void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
    void addAction(BaseAction *action);
    bool addSettlement(Settlement *settlement);
    bool addFacility(FacilityType facility);
    bool isSettlementExists(const string &settlementName);
    Settlement &getSettlement(const string &settlementName);
    Plan &getPlan(const int planID);
    void step();
    void close();
    void open();
    void parseConfigFile(const std::string &configFilePath);
    void printInitialState() const;
    ~Simulation();
    // ido functions:
    SelectionPolicy *createSelectionPolicy(const std::string &policyType);
    void actionHandler(const std::string &action);
    //Dafna function: 
    void printLog() const;
     void backup();
     void restore();

    // ///rule of 5
    // //Constructor,Destructor - alreadey did
    // // Copy constructor
    Simulation(const Simulation &other);
    // // Copy assignment operator
    Simulation &operator=(const Simulation &other);
    // // Move constructor
    Simulation(Simulation &&other) noexcept;
    // // Move assignment operator
    Simulation &operator=(Simulation &&other) noexcept;

private:
    bool isRunning;
    int planCounter; // For assigning unique plan IDs
    vector<BaseAction *> actionsLog;
    vector<Plan> plans;
    vector<Settlement *> settlements;
    vector<FacilityType> facilitiesOptions;
    //dafna add
     Simulation *backupSim;

     /// i need to fix thr backupsim at initail not to nullput instand the config filer

    
    // Helper methods for parsing configuration
    void parseConfig(const std::string &configFilePath);
    void handleSettlementCommand(const std::vector<std::string> &arguments);
    void handleFacilityCommand(const std::vector<std::string> &arguments);
    void handlePlanCommand(const std::vector<std::string> &arguments);
};