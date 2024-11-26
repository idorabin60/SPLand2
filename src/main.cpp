#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Auxiliary.h"

// Function to process settlement data
void processSettlement(const std::vector<std::string> &args)
{
    if (args.size() != 3)
    {
        std::cerr << "Invalid settlement line format.\n";
        return;
    }
    std::string settlementName = args[1];
    int settlementType = std::stoi(args[2]);
    std::cout << "Processed settlement: " << settlementName << ", Type: " << settlementType << std::endl;
}

// Function to process facility data
void processFacility(const std::vector<std::string> &args)
{
    if (args.size() != 7)
    {
        std::cerr << "Invalid facility line format.\n";
        return;
    }
    std::string facilityName = args[1];
    int category = std::stoi(args[2]);
    int price = std::stoi(args[3]);
    int lifeqImpact = std::stoi(args[4]);
    int ecoImpact = std::stoi(args[5]);
    int envImpact = std::stoi(args[6]);
    std::cout << "Processed facility: " << facilityName
              << ", Category: " << category
              << ", Price: " << price
              << ", Life Quality Impact: " << lifeqImpact
              << ", Eco Impact: " << ecoImpact
              << ", Env Impact: " << envImpact << std::endl;
}

// Function to process plan data
void processPlan(const std::vector<std::string> &args)
{
    if (args.size() != 3)
    {
        std::cerr << "Invalid plan line format.\n";
        return;
    }
    std::string settlementName = args[1];
    std::string selectionPolicy = args[2];
    std::cout << "Processed plan: Settlement: " << settlementName
              << ", Policy: " << selectionPolicy << std::endl;
}

int main()
{
    std::ifstream configFile("config_file.txt");
    if (!configFile.is_open())
    {
        std::cerr << "Failed to open configuration file.\n";
        return 1;
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
            processSettlement(arguments);
        }
        else if (command == "facility")
        {
            processFacility(arguments);
        }
        else if (command == "plan")
        {
            processPlan(arguments);
        }
        else
        {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }

    configFile.close();
    return 0;
}
