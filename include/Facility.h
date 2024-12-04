#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

enum class FacilityStatus
{
    UNDER_CONSTRUCTIONS,
    OPERATIONAL,
};

enum class FacilityCategory
{
    LIFE_QUALITY,
    ECONOMY,
    ENVIRONMENT,
};

class FacilityType
{
public:
    FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score);
    const string &getName() const;
    virtual ~FacilityType() = default; // Virtual destructor

    int getCost() const;
    int getLifeQualityScore() const;
    int getEnvironmentScore() const;
    int getEconomyScore() const;
    FacilityCategory getCategory() const;
    virtual FacilityType *clone() const;

protected:
    const string name;
    const FacilityCategory category;
    const int price;
    const int lifeQuality_score;
    const int economy_score;
    const int environment_score;
};

class Facility : public FacilityType
{

public:
    Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score);
    Facility(const FacilityType &type, const string &settlementName);
    const string &getSettlementName() const;
    virtual ~Facility() override = default; // Virtual destructor

    const int getTimeLeft() const;
    FacilityStatus step();
    void setStatus(FacilityStatus status);
    const FacilityStatus &getStatus() const;
    const string toString() const;
    Facility *clone() const override;
    static std::string facilityCategoryToString(FacilityCategory category); // Make it static

private:
    const string settlementName;
    FacilityStatus status;
    int timeLeft;
};