#pragma once
#include <array>
#include <memory>
#include"HelperClasses.h"

class Unit;
class EnemyMob;

class DataProcessor {
	DataProcessor() {};
public:
	static void DistributeStar(std::array<int, 5>& CriticalStarDistribution);

	static float HitNP_Calculation(const Card& i, const float & cardNPMod,const std::shared_ptr<EnemyMob> Enemy, const bool isArtFirst);

	static float HitStar_Calculation(const Card& i, const float & cardStarMod, const std::shared_ptr<EnemyMob> Enemy, const bool isQuickFirst);

	static void Total_Calculation(const std::vector<int>& DamageStack, const int& pointer, Card& i, const std::shared_ptr<EnemyMob>Enemy, const CardColor& FirstCardColor);

	static void Card_Prepare(std::shared_ptr<Unit> Enemy, Card& i, float& CardMod, float& TotalSpecificDamage, float& TotalSpecificDefence);
	
private:
	const static std::array<int, 5> StarDistributionModifierTemplate;
};