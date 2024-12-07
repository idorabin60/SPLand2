#include <vector>
#include "Facility.h"
#include "SelectionPolicy.h"
#include <algorithm> // For std::min and std::max
#include <stdexcept>

using std::vector;
// NaiveSelection Implementation
NaiveSelection::NaiveSelection() : lastSelectedIndex(0) {};
const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size(); // so it will return to the start of the vector (we dont want unexpected behavior)
    return facilitiesOptions[lastSelectedIndex];
}
const string NaiveSelection::toString() const
{
    return "naiv";
}

NaiveSelection *NaiveSelection::clone() const
{
    return new NaiveSelection(*this);
}

// BalancedSelection Implementation
BalancedSelection::BalancedSelection(int lifeQualityScore, int economyScore, int environmentScore)
    : LifeQualityScore(lifeQualityScore), EconomyScore(economyScore), EnvironmentScore(environmentScore) {}
const FacilityType &BalancedSelection::selectFacility(const std::vector<FacilityType> &facilitiesOptions)
{
    int minDifference = std::numeric_limits<int>::max(); // Initialize to a very large number
    const FacilityType *bestFacility = nullptr;

    for (const FacilityType &facility : facilitiesOptions)
    {
        // Calculate temporary scores for the current facility
        int tempLifeQualityScore = LifeQualityScore + facility.getLifeQualityScore();
        int tempEconomyScore = EconomyScore + facility.getEconomyScore();
        int tempEnvironmentScore = EnvironmentScore + facility.getEnvironmentScore();

        // Calculate the score difference
        int maxScore = std::max({tempLifeQualityScore, tempEconomyScore, tempEnvironmentScore});
        int minScore = std::min({tempLifeQualityScore, tempEconomyScore, tempEnvironmentScore});
        int diff = maxScore - minScore;

        // Check if this facility is better
        if (diff < minDifference)
        {
            minDifference = diff;
            bestFacility = &facility;
        }
    }

    // Ensure a valid facility was found
    if (bestFacility)
    {
        // Update the fields with the scores of the best facility
        LifeQualityScore += bestFacility->getLifeQualityScore();
        EconomyScore += bestFacility->getEconomyScore();
        EnvironmentScore += bestFacility->getEnvironmentScore();
    }

    return *bestFacility; // Return the best facility
}
const string BalancedSelection::toString() const
{
    return "bal";
}
BalancedSelection *BalancedSelection::clone() const
{
    return new BalancedSelection(*this);
}

// Econemy selection:
EconomySelection::EconomySelection() : lastSelectedIndex(0) {};
const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.empty())
    {
        throw std::runtime_error("No facilities available to select from.");
    }

    for (std::vector<FacilityType>::size_type i = lastSelectedIndex; i < facilitiesOptions.size(); i++)
    {
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ECONOMY)
        {
            lastSelectedIndex = (i + 1) % facilitiesOptions.size(); // Update to the next index
            return facilitiesOptions[i];
        }
    }

    for (int i = 0; i < lastSelectedIndex; i++)
    {
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ECONOMY)
        {
            lastSelectedIndex = (i + 1) % facilitiesOptions.size(); // Update to the next index
            return facilitiesOptions[i];
        }
    }

    throw std::runtime_error("No facility with the correct category found.");
}

EconomySelection *EconomySelection::clone() const
{
    return new EconomySelection(*this);
}
const string EconomySelection::toString() const
{
    return "eco";
}
// Sustainable selection:
SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(0) {};
const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.empty())
    {
        throw std::runtime_error("No facilities available to select from.");
    }

    for (std::vector<FacilityType>::size_type i = lastSelectedIndex; i < facilitiesOptions.size(); i++)
    {
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ENVIRONMENT)
        {
            lastSelectedIndex = (i + 1) % facilitiesOptions.size(); // Update to the next index
            return facilitiesOptions[i];
        }
    }

    for (int i = 0; i < lastSelectedIndex; i++)
    {
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ENVIRONMENT)
        {
            lastSelectedIndex = (i + 1) % facilitiesOptions.size(); // Update to the next index
            return facilitiesOptions[i];
        }
    }

    throw std::runtime_error("No facility with the correct category found.");
}

SustainabilitySelection *SustainabilitySelection::clone() const
{
    return new SustainabilitySelection(*this);
}
const string SustainabilitySelection::toString() const
{
    return "sus";
}
