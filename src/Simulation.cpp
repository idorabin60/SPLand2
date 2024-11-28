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
    std::cout << "Sim is running!" << std::endl;

    SimulateStep user_step = SimulateStep(1);
    user_step.act(*this);
    std::cout << user_step.toString() << std::endl;

    std::string action; // Declare the variable here
    std::cout << "Enter action please: ";
    std::cin >> action;

    while (action != "close")
    {
        actionHandler(action);
        std::cout << "Enter action please: ";
        std::cin >> action;
    }

    std::cout << "Finished" << std::endl;
}

void Simulation::step()
{
    for (Plan element : plans)
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
        std::cout << "Call add settlement operation" << std::endl;
    }
    else if (words[0] == "restore")
    {
        std::cout << "Call restore operation" << std::endl;
    }
    else if (words[0] == "facility")
    {
        std::cout << "Call add facility operation" << std::endl;
    }
    else if (words[0] == "plan")
    {
        std::cout << "Call add plan operation" << std::endl;
    }
    else if (words[0] == "backup")
    {
        std::cout << "Call backup operation" << std::endl;
    }
    else if (words[0] == "log")
    {
        std::cout << "Call log operation" << std::endl;
    }
    else
    {
        std::cout << "Unknown action: " << words[0] << std::endl;
    }
}
