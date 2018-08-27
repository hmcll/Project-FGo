#include "DataProcessor.h"
#include "RandomGenerator.h"
#include "GameInstance.h"
#include "Servent.h"
#include "EnemyMob.h"

const std::array<int, 5> DataProcessor::StarDistributionModifierTemplate = {50,20,20,0,0};

void DataProcessor::DistributeStar(std::array<int, 5>& CriticalStarDistribution)
{
	std::array<float, 5>CriticalStarAbsorbRate;
	CriticalStarDistribution.fill(0);
	auto StarDistributionModifier = StarDistributionModifierTemplate;
	RandomGenerator::RandomShuffle(StarDistributionModifier);
	auto Instance = GameInstance::Get();

	std::vector<std::shared_ptr<Servent>>ServentStack;
	for (auto i : Instance->OnFieldMyServentCollection)if (i)ServentStack.push_back(i);
	
	for (int i = 0; i < 5; i++) {
		CriticalStarAbsorbRate[i] = ServentStack[Instance->CardStack[i] / 5]->GetProperty(ImplementedPropertiesAndBuffName::StarAbsorption) + StarDistributionModifier[i];
	}
	RandomGenerator::StarDistributor(Instance->Star, CriticalStarAbsorbRate, CriticalStarDistribution);
	Instance->Star = 0;
}

float DataProcessor::HitNP_Calculation(const Card & i, const float & CardNPMod, const std::shared_ptr<EnemyMob> Enemy, const bool isArtFirst)
{
	return i._servent->GetProperty(ImplementedPropertiesAndBuffName::NPChargeAtk)* //N / A *
		(CardNPMod * (std::min(5.f, 1.f + i._servent->GetProperty(CardColorName[int(i._color)] + "Buff")) - Enemy->GetProperty(CardColorName[int(i._color)] + "Resistance")) + isArtFirst)* // (カード補正 * (min(5, 1 + -カードバフ) - カード耐性)+ 1stボーナス[1])
		std::min(5.f, 1 + i._servent->GetProperty(ImplementedPropertiesAndBuffName::NPBuff))* //* min(5, 1 + -NPバフ)
		Enemy->GetProperty(ImplementedPropertiesAndBuffName::DTDR)* //DTDR
		(i._isCritical + 1)// * クリティカル補正[2], 2)
		;
}

float DataProcessor::HitStar_Calculation(const Card & i, const float & CardStarMod, const std::shared_ptr<EnemyMob> Enemy, const bool isQuickFirst)
{
	return i._servent->GetProperty(ImplementedPropertiesAndBuffName::StarGeneration) + //SR +
		(CardStarMod* (std::min(5.f, 1.f + i._servent->GetProperty(CardColorName[int(i._color)] + "Buff")) - Enemy->GetProperty(CardColorName[int(i._color)] + "Resistance")) + isQuickFirst * 0.2f) + //(カード補正 * (min(5, 1 + -カードバフ) - カード耐性) + 1stボーナス[0.2])+
		Enemy->GetProperty(ImplementedPropertiesAndBuffName::DSR) + //DSR+
		std::min(5.f, i._servent->GetProperty(ImplementedPropertiesAndBuffName::StarGenerationMod)) -//min(5, +-Starバフ)-
		std::min(5.f, Enemy->GetProperty(ImplementedPropertiesAndBuffName::StarGenerationResistance)) +// min(5, +-敵Starバフ) +
		i._isCritical * 0.2f// クリティカル補正[0.2]
		;
}

void DataProcessor::Total_Calculation(const std::vector<int>& DamageStack, const int & pointer, Card & i, const std::shared_ptr<EnemyMob> Enemy, const CardColor & FirstCardColor)
{
	auto Instance = GameInstance::Get();
	const float HitNP = HitNP_Calculation(i, CardNPMod[int(i._color)][pointer], Enemy, FirstCardColor == CardColor::Art);
	const float HitStarRate = HitStar_Calculation(i, CardStarMod[int(i._color)][pointer], Enemy, FirstCardColor == CardColor::Quick);

	float CurrentHP = i._servent->GetProperty(ImplementedPropertiesAndBuffName::Health);
	for (int HitDamage : DamageStack) {
		CurrentHP -= HitDamage;
		if (CurrentHP < 0) {
			i._servent->ChargeNP(HitNP*1.5f);
			Instance->Star += RandomGenerator::StarGenerator(HitStarRate*1.5f + 0.3f);
		}
		else {
			i._servent->ChargeNP(HitNP);
			Instance->Star += RandomGenerator::StarGenerator(HitStarRate*1.5f);
		}
	}
	i._servent->OnAttack(Enemy);
}

void DataProcessor::Card_Prepare(std::shared_ptr<Unit> Enemy, Card & i, float & CardMod, float & TotalSpecificDamage, float & TotalSpecificDefence)
{
	CardMod = i._color == CardColor::Buster ? i._servent->GetProperty(ImplementedPropertiesAndBuffName::BusterPerformanceMod) : i._color == CardColor::Art ? i._servent->GetProperty(ImplementedPropertiesAndBuffName::ArtPerformanceMod) : i._color == CardColor::Quick ? i._servent->GetProperty(ImplementedPropertiesAndBuffName::QuickPerformanceMod) : 0;
	for (unsigned TraitIterator = 0; TraitIterator < i._servent->Traits.size(); TraitIterator++) {
		if (i._servent->Traits[TraitIterator]) {
			TotalSpecificDefence += Enemy->GetProperty(SpecificDefenceUpTraitName[TraitIterator]);
		}
	}
	for (unsigned TraitIterator = 0; TraitIterator < i._servent->Traits.size(); TraitIterator++) {
		if (Enemy->Traits[TraitIterator]) {
			TotalSpecificDamage += i._servent->GetProperty(SpecificDamageUpTraitName[TraitIterator]);
		}
	}
}
