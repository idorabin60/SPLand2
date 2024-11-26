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

// Constructor
Simulation::Simulation(const std::string &configFilePath)
    : isRunning(false), planCounter(0) // Initialize fields
{
    // Open the configuration file
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
            // Handle settlement creation
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
        else if (command == "facility")
        {
            // Handle facility option creation
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
        else if (command == "plan")
        {
            // Handle plan creation
            if (arguments.size() == 3)
            {
                auto settlementIt = std::find_if(settlements.begin(), settlements.end(),
                                                 [&arguments](Settlement *settlement)
                                                 { return settlement->getName() == arguments[1]; });

                if (settlementIt != settlements.end())
                {
                    SelectionPolicy *policy = createSelectionPolicy(arguments[2]);
                    plans.emplace_back(planCounter++, **settlementIt, policy, facilitiesOptions);
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
        else
        {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }

    configFile.close();
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

// Start function
void Simulation::start()
{
    isRunning = true;
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
                      << ", Life Quaily Score:: " << facility.getLifeQualityScore()
                      << ", Econemy Score: " << facility.getEconomyScore()
                      << ", Environment Score: " << facility.getEnvironmentScore();
        }
    }
}
