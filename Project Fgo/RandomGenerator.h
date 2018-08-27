#pragma once
#include <random>
#include <array>

class Servent;
class RandomGenerator {
private:
	static void Initalize();
	static std::shared_ptr<RandomGenerator> Instance;
	static void Reset();
public:
	template<typename T>
	static void RandomShuffle(T& in) {
		if (!Instance)
			Initalize();
		std::shuffle(in.begin(), in.end(), Instance->RandomEngine);
	}
	static bool CriticalDistributor(const int & CriticalRate);

	static int StarGenerator(const float& StarRate);
	static void StarDistributor(const int& Star, const std::array<float, 5>& CriticalStarAbsorbRate, std::array<int, 5>&CriticalStarDistribution);
	static float RandomModifier();
	static int RandomSelect(std::vector<std::shared_ptr<Servent>> ArrayIn);
	static float DebuffDistributor();
private:
	
	static std::uniform_real_distribution<float> RandomModifierDistribution;
	static std::uniform_real_distribution<float> DebuffDistribution;
	static std::uniform_int_distribution<int> StarGeneratorDistribution;
	static std::uniform_int_distribution<int> CriticalDistribution;
	std::default_random_engine RandomEngine;
};