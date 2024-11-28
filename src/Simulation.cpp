#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "Simulation.h"
#include "Auxiliary.h"
#include "Settlement.h"
#include "Facility.h"
#include "SelectionPolicy.h"
#include "Action.h"
#include "Plan.h"
#include <sstream>
using namespace std;

// Constructor
Simulation::Simulation(const std::string &configFilePath)
    : isRunning(false), planCounter(0) // Initialize fields
{
    parseConfig(configFilePath);
    printInitialState();
}

// Destructor
Simulation::~Simulation()
{
    for (auto settlement : settlements)
    {
        delete settlement;
    }
}

// Function to parse configuration file
void Simulation::parseConfig(const std::string &configFilePath)
{
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        throw std::runtime_error("Failed to open configuration file.");
    }

    std::string line;
    while (std::getline(configFile, line))
    {
        // Trim leading/trailing whitespaces
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        // Skip empty lines or comments
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        // Parse the line into arguments
        std::vector<std::string> arguments = Auxiliary::parseArguments(line);
        if (arguments.empty())
        {
            continue;
        }

        // Process based on the first argument
        const std::string &command = arguments[0];
        if (command == "settlement")
        {
            handleSettlementCommand(arguments);
        }
        else if (command == "facility")
        {
            handleFacilityCommand(arguments);
        }
        else if (command == "plan")
        {
            handlePlanCommand(arguments);
        }
        else
        {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }
    planCounter = plans.size();
    configFile.close();
}

// Handle settlement command
void Simulation::handleSettlementCommand(const std::vector<std::string> &arguments)
{
    if (arguments.size() == 3)
    {
        try
        {
            SettlementType type = static_cast<SettlementType>(std::stoi(arguments[2]));
            settlements.push_back(new Settlement(arguments[1], type));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error parsing settlement type for " << arguments[1] << ": " << e.what() << std::endl;
        }
    }
    else
    {
        std::cerr << "Invalid number of arguments for settlement command" << std::endl;
    }
}

// Handle facility command
void Simulation::handleFacilityCommand(const std::vector<std::string> &arguments)
{
    if (arguments.size() == 7)
    {
        try
        {
            FacilityType facility(
                arguments[1],
                static_cast<FacilityCategory>(std::stoi(arguments[2])),
                std::stoi(arguments[3]),
                std::stoi(arguments[4]),
                std::stoi(arguments[5]),
                std::stoi(arguments[6]));
            facilitiesOptions.push_back(facility);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error parsing facility for " << arguments[1] << ": " << e.what() << std::endl;
        }
    }
    else
    {
        std::cerr << "Invalid number of arguments for facility command" << std::endl;
    }
}

// Handle plan command
void Simulation::handlePlanCommand(const std::vector<std::string> &arguments)
{
    if (arguments.size() == 3)
    {
        Settlement *foundSettlement = nullptr;

        // Standard for loop to iterate through settlements
        for (size_t i = 0; i < settlements.size(); ++i)
        {
            if (settlements[i]->getName() == arguments[1])
            {
                foundSettlement = settlements[i];
                break; // Exit loop as we found the matching settlement
            }
        }

        if (foundSettlement)
        {
            SelectionPolicy *policy = createSelectionPolicy(arguments[2]);
            plans.emplace_back(planCounter++, *foundSettlement, policy, facilitiesOptions);
        }
        else
        {
            std::cerr << "Settlement not found for plan: " << arguments[1] << std::endl;
        }
    }
    else
    {
        std::cerr << "Invalid number of arguments for plan command" << std::endl;
    }
}

// Start function
void Simulation::start()
{
    isRunning = true;
    std::cout << "Sim is running!";
    // std::string userInput;
    // std::getline(std::cin, userInput);
    // int numOfSteps = userInput[userInput.size() - 1]- '0';
    // std::string str =(userInput.substr(0, userInput.size() - 1));
    // if (str =="step"){
    SimulateStep user_step = SimulateStep(1);
    user_step.act(*this);
    // std::cout << user_step.toString() << std::endl;;
    std::cout << planCounter << std::endl;
    ;
    AddPlan temp = AddPlan("BeitSPL", "eco");
    temp.act(*this);
    std::cout << temp.toString() << std::endl;
    ;
    PrintPlanStatus printTemp = PrintPlanStatus(2);
    printTemp.act(*this);
    std::cout << printTemp.toString() << std::endl;
    ;
}

void Simulation::step()
{
    for (Plan &element : plans)
    {
        element.step();
    }
}

// Stop function
void Simulation::close()
{
    isRunning = false;
}

// Create SelectionPolicy based on input string
SelectionPolicy *Simulation::createSelectionPolicy(const std::string &policyType)
{
    if (policyType == "bal")
    {
        return new BalancedSelection(50, 50, 50); // Example default scores
    }
    else if (policyType == "eco")
    {
        return new EconomySelection();
    }
    else
    {
        return new NaiveSelection();
    }
}

// Function to print the initial state of the Simulation
void Simulation::printInitialState() const
{
    std::cout << "Simulation Initial State:" << std::endl;
    std::cout << "isRunning: " << (isRunning ? "true" : "false") << std::endl;
    std::cout << "planCounter: " << planCounter << std::endl;

    std::cout << "Settlements:" << std::endl;
    if (settlements.empty())
    {
        std::cout << "  None" << std::endl;
    }
    else
    {
        for (const auto &settlement : settlements)
        {
            std::cout << settlement->toString() << std::endl;
        }
    }

    std::cout << "Facilities Options:" << std::endl;
    if (facilitiesOptions.empty())
    {
        std::cout << "  None" << std::endl;
    }
    else
    {
        for (const auto &facility : facilitiesOptions)
        {
            std::cout << "  Name: " << facility.getName()
                      << "  Category: " << Facility::facilityCategoryToString(facility.getCategory()) << "\n"
                      << ", Price: " << facility.getCost()
                      << ", Life Quality Score: " << facility.getLifeQualityScore()
                      << ", Economy Score: " << facility.getEconomyScore()
                      << ", Environment Score: " << facility.getEnvironmentScore() << std::endl;
        }
    }
}
Plan &Simulation::getPlan(const int planID)
{
    // Check if the planID is within valid bounds
    if (planID < 0 || planID >= plans.size())
    {
        std::cout << "Invalid plan ID" << std::endl;
    }
    // Return the plan by reference
    return plans[planID];
}
Settlement &Simulation::getSettlement(const string &settlementName)
{
    for (Settlement *set : settlements)
    {
        if (set->getName() == settlementName)
        {
            std::cout << set->toString();
            return *set;
        }
    }
    throw std::runtime_error("Settlement not found: " + settlementName);
}
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    Settlement s = settlement;
    Plan new_plan = Plan(planCounter, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(std::move(new_plan));
    planCounter++;
}
