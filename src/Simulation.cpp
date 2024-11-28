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
    std::string action = "";

    std::cout << "Simulation is running!" << std::endl;

    while (true)
    {
        std::cout << "Type an action (or 'close' to stop): ";
        std::getline(std::cin, action); // Use getline to capture the entire input line

        if (action == "close")
        {
            std::cout << "Simulation finished." << std::endl;
            break; // Exit the loop if user types "close"
        }

        actionHandler(action); // Handle the full line of input
    }

    isRunning = false; // Mark simulation as stopped
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
        return new BalancedSelection(0, 0, 0); // Example default scores
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
            return *set;
        }
    }
    throw std::runtime_error("Settlement not found: " + settlementName);
}
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    Settlement s = settlement;
    Plan *new_plan = new Plan(planCounter, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back((*new_plan));
    planCounter++;
}
////
bool Simulation::addSettlement(Settlement *settlement)
{
    // asumme that that settlement dosent exsit
    settlements.push_back(settlement);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName)
{
    for (Settlement *set : settlements)
    {
        if (set->getName() == settlementName)
        {
            return true;
        }
    }
    return false;
}
bool Simulation::addFacility(FacilityType facility)
{
    for (FacilityType fac : facilitiesOptions)
    {
        if (fac.getName() == facility.getName())
        {
            return false;
        }
    }
    facilitiesOptions.push_back(facility);
    return true;
}

// parsing string method:
std::vector<std::string> parseToWords(const std::string &input)
{
    std::vector<std::string> words;
    std::istringstream stream(input);
    std::string word;

    // Extract each word and add to the vector
    while (stream >> word)
    {
        words.push_back(word);
    }

    return words;
}
// Create an action handler
void Simulation::actionHandler(const std::string &action)
{
    std::vector<std::string> words = parseToWords(action);
    if (words[0] == "settlement")
    {
        if (words[2] == "0")
        {
            AddSettlement settlemntToBeAdded = AddSettlement(words[1], SettlementType::VILLAGE);
            settlemntToBeAdded.act(*this);
        }
        else if (words[2] == "1")
        {
            AddSettlement settlemntToBeAdded = AddSettlement(words[1], SettlementType::CITY);
            settlemntToBeAdded.act(*this);
        }
        else if (words[2] == "2")
        {
            AddSettlement settlemntToBeAdded = AddSettlement(words[1], SettlementType::METROPOLIS);
            settlemntToBeAdded.act(*this);
        }
    }
    else if (words[0] == "restore")
    {
        std::cout << "Call restore operation" << std::endl;
    }
    else if (words[0] == "facility")
    {
        if (words[2] == "0")
        {
            AddFacility faccilityToBeAdded = AddFacility(words[1], FacilityCategory::LIFE_QUALITY, std::stoi(words[3]), std::stoi(words[4]), std::stoi(words[5]), std::stoi(words[6]));
            faccilityToBeAdded.act(*this);
        }
        if (words[2] == "1")
        {
            AddFacility faccilityToBeAdded = AddFacility(words[1], FacilityCategory::ECONOMY, std::stoi(words[3]), std::stoi(words[4]), std::stoi(words[5]), std::stoi(words[6]));
            faccilityToBeAdded.act(*this);
        }
        if (words[2] == "3")
        {
            AddFacility faccilityToBeAdded = AddFacility(words[1], FacilityCategory::ENVIRONMENT, std::stoi(words[3]), std::stoi(words[4]), std::stoi(words[5]), std::stoi(words[6]));
            faccilityToBeAdded.act(*this);
        }
    }
    else if (words[0] == "plan")
    {
        if (isSettlementExists(words[1]))
        {
            AddPlan planToBeAdded(words[1], words[2]);
            planToBeAdded.act(*this);
        }
        else
        {
            std::cout << "ahi ze ihsov lo kaiam ma ata dba" << std::endl;
        }
    }

    else if (words[0] == "backup")
    {
        std::cout << "Call backup operation" << std::endl;
    }
    else if (words[0] == "log")
    {
        std::cout << "Call log operation" << std::endl;
    }
    else if (words[0] == "planStatus")
    {
        PrintPlanStatus planStatusToBeAdded = PrintPlanStatus(std::stoi(words[1]));
        planStatusToBeAdded.act(*this);
    }
    else
    {
        std::cout << "Unknown action: " << words[0] << std::endl;
    }
}