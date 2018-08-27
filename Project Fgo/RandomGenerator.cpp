#include "RandomGenerator.h"
#include "Servent.h"
#include <algorithm>
#include <memory>
std::shared_ptr<RandomGenerator> RandomGenerator::Instance;

std::uniform_real_distribution<float> RandomGenerator::RandomModifierDistribution(0.9f, 1.1f);
std::uniform_real_distribution<float> RandomGenerator::DebuffDistribution(0.f, 1.f);
std::uniform_int_distribution<int> RandomGenerator::StarGeneratorDistribution(0, 9);
std::uniform_int_distribution<int> RandomGenerator::CriticalDistribution(0, 99);

void RandomGenerator::Initalize()
{
	Instance = std::make_shared<RandomGenerator>();
	Instance->RandomEngine.seed(std::random_device()());
}

void RandomGenerator::Reset()
{
	if (Instance)
		Instance.reset();
	Initalize();
}

bool RandomGenerator::CriticalDistributor(const int & CriticalRate)
{
	if (!Instance)
		Initalize();
	return CriticalDistribution(Instance->RandomEngine) < CriticalRate;
}

int RandomGenerator::StarGenerator(const float & StarRate)
{
	if (!Instance)
		Initalize();
	return int(StarRate) + ((StarRate - int(StarRate)) * 10 > StarGeneratorDistribution(Instance->RandomEngine));
}

void RandomGenerator::StarDistributor(const int & Star, const std::array<float, 5>& CriticalStarAbsorbRate, std::array<int, 5>& CriticalStarDistribution)
{
	if (!Instance)
		Initalize();
	int star = std::min(Star, 50);
	std::vector<int*> Distribution;
	Distribution.push_back(&CriticalStarDistribution[0]);
	Distribution.push_back(&CriticalStarDistribution[1]);
	Distribution.push_back(&CriticalStarDistribution[2]);
	Distribution.push_back(&CriticalStarDistribution[3]);
	Distribution.push_back(&CriticalStarDistribution[4]);

	std::vector<float> AbsorbRate;
	AbsorbRate.push_back(CriticalStarAbsorbRate[0]);
	AbsorbRate.push_back(CriticalStarAbsorbRate[1]);
	AbsorbRate.push_back(CriticalStarAbsorbRate[2]);
	AbsorbRate.push_back(CriticalStarAbsorbRate[3]);
	AbsorbRate.push_back(CriticalStarAbsorbRate[4]);

	std::uniform_int_distribution<int> StarDistributer = std::uniform_int_distribution<int>(0, int(AbsorbRate[0] + AbsorbRate[1] + AbsorbRate[2] + AbsorbRate[3] + AbsorbRate[4]) - 1);
	for (int i = 0; i < star; i++) {
		int Place = StarDistributer(Instance->RandomEngine);
		for (int j = 0; j < Distribution.size(); j++) {
			if (Place -= AbsorbRate[j] < 0) {
				*Distribution[j] ++;
				if (*Distribution[j] == 10) {
					Distribution.erase(Distribution._Make_iterator_offset(j));
					AbsorbRate.erase(AbsorbRate._Make_iterator_offset(j));
					int RandomMax = -1;
					for (int k : AbsorbRate)RandomMax += k;
					StarDistributer = std::uniform_int_distribution<int>(0, RandomMax);
				}
				break;
			}
		}
	}
}

float RandomGenerator::RandomModifier()
{
	if (!Instance)
		Initalize();
	return RandomModifierDistribution(Instance->RandomEngine);
}

int RandomGenerator::RandomSelect(std::vector<std::shared_ptr<Servent>> ArrayIn)
{
	if (!Instance)
		Initalize();
	std::vector<int> Weights;
	int TotalWeight = 0;
	for (auto i : ArrayIn) {
		Weights.push_back(i->GetProperty(ImplementedPropertiesAndBuffName::TargetConcentrationMod));
		TotalWeight += i->GetProperty(ImplementedPropertiesAndBuffName::TargetConcentrationMod);
	}
	if (TotalWeight == 0) {
		std::uniform_int_distribution<int>Selector(0,ArrayIn.size() - 1);
		return Selector(Instance->RandomEngine);
	}
	else {
		std::uniform_int_distribution<int>Selector(0, TotalWeight - 1);
		int Selected = Selector(Instance->RandomEngine);
		for (int j = 0; j < Weights.size(); j++) {
			if (Weights[j] == 0) {
				continue;
			}
			if ((Selected -= Weights[j]) <= 0) {
				return j;
			}
		}
	}

	return ArrayIn.size() - 1;
}

float RandomGenerator::DebuffDistributor()
{
	if (!Instance)
		Initalize();
	return DebuffDistribution(Instance->RandomEngine);
}
