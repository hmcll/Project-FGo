#include "Jack.h"
#include "GameInstance.h"
#include "RandomGenerator.h"
#include "EnemyMob.h"
#include "Skill.h"
#include "IOHolder.h"
#include "DataProcessor.h"
namespace ImplementedServents {

	IMPLEMENTED_SERVENT_IMPLEMENTATION(Jack)


	Jack::Jack(int NPLevel, int _Skill1Level, int _Skill2Level, int _Skill3Level, int Health, int Attack)
	{
		NPLevel--;
		_Skill1Level--, _Skill2Level--, _Skill3Level--;

		NoblePhantasmColor = CardColor::Quick;
		NoblePhantasmName = "Maria the Ripper";
		//Properties
		RegisterProperties(Health, Attack, 2.f, 2.f, 5.f, 4.f, 1.f, 1.f, 3.f, NPLevel, "Jack", Class::Assassin, 0.0107f, 0.04f, 97.f, 0.255f + 0.105f, 0.44f, Attribute::Earth, _Skill1Level, _Skill2Level, _Skill3Level);
		RegisterTraits(Trait::EarthOrSky, Trait::Female, Trait::Humanoid, Trait::Servant, Trait::WeakToEnumaElish, Trait::Chaotic, Trait::Evil);

		//Skill1
		Skill1.CoolDown = Skill1Level == 8 ? 5 : Skill1Level > 4 ? 7 : 6;
		Skill1.ChooseSkillTarget = Skill::NoNeedToChoose;
		Skill1.Name = "MurdereroftheMistyNight";
		//Skill2
		Skill2.CoolDown = Skill2Level == 9 ? 7 : Skill2Level > 4 ? 8 : 9;
		Skill2.ChooseSkillTarget = Skill::Enemy;
		Skill2.Name = "InformationErasure";
		//Skill3
		Skill3.CoolDown = Skill3Level == 9 ? 6 : Skill3Level > 4 ? 7 : 8;
		Skill3.ChooseSkillTarget = Skill::TeamMate;
		Skill3.Name = "Surgery";
	}

	void Jack::CallNoblePhantasm(std::shared_ptr<EnemyMob> To) {
		const std::vector<float> NPHitDamageMul{
			0.1f,
			0.2f,
			0.3f
		};
		const std::array<float, 10> NPDamageMul{
			14.f,
			18.f,
			20.f,
			21.f,
			22.f
		};
		const std::array<float, 10> NPFemaleSpecialAttackMul{
			0.5f,
			0.625f,
			0.75f,
			0.875f,
			1.f
		};

		Card NPCard;
		NPCard._servent = std::static_pointer_cast<Servent>(shared_from_this());
		NPCard._color = NoblePhantasmColor;
		NPCard._isCritical = false;
		NPCard._isNP = true;

		auto PercentFemaleSpecialAttack = std::make_shared<RegularBuff>(SpecificDamageUpTraitName[int(Trait::Female)], NPFemaleSpecialAttackMul[GetProperty(ImplementedPropertiesAndBuffName::NoblePhantasmLevel)],1,-1);
		AddRegularBuff(PercentFemaleSpecialAttack);
		RegisterRoundEndSpecial(PercentFemaleSpecialAttack);

		float cardMod = 0, TotalSpecificDamage = 0, TotalSpecificDefence = 0;
		DataProcessor::Card_Prepare(To, NPCard, cardMod, TotalSpecificDamage, TotalSpecificDefence);

		float TotalDamage = GetProperty(ImplementedPropertiesAndBuffName::Attack)* //ATK *
			NPDamageMul[GetProperty(ImplementedPropertiesAndBuffName::NoblePhantasmLevel)] * //宝具倍率 *
			0.23f* //攻撃補正[0.23] *
			(CardDamageValue[int(CardColor::Quick)][0] * std::min(1.f + cardMod, 5.f))* //(カード補正 * min(5, 1 + -カードバフ)) *
			ClassAtkBonus[int(UnitClass)] * //Class補正 *
			TriangleModifier[int(UnitClass)][int(To->UnitClass)] * //Class相性補正 *
			AttributeModifier[int(UnitAttribute)][int(To->UnitAttribute)] * //Servant相性補正 *
			RandomGenerator::RandomModifier()* //乱数補正[0.9 - 1.1] *
			(1 + GetProperty(ImplementedPropertiesAndBuffName::PercentAttackMod))* //(1 + -攻撃力バフ) * (Ignore Defence)
			(1 + std::min(TotalSpecificDamage, 5.f) + std::min(GetProperty(ImplementedPropertiesAndBuffName::NoblePhantasmDamageMod), 5.f)) * //(1 + min(5, +-特攻威力バフ) + min(5, +-宝具威力バフ)) *
			1.f//宝具特攻
			+ std::min(GetProperty(ImplementedPropertiesAndBuffName::ConstAttackMod), 1000.f)//+ min(1000, +-ダメージバフ) + min(1000, +-敵ダメージバフ)
			;
		float TempDamage = TotalDamage;
		std::vector<int> DamageStack;

		for (float DamageMul : NPHitDamageMul) {
			DamageStack.push_back(DamageMul * TotalDamage);
			TempDamage -= DamageMul * TotalDamage;
		}
		DamageStack.push_back(int(TempDamage));

		DataProcessor::Total_Calculation(DamageStack, 0, NPCard, To, CardColor::EX);

		To->ChangeHealth(-1 * TotalDamage);

		IOHolder::OutLine(NPCard._servent->Name, "对", To->Name, "造成了", TotalDamage, "点伤害");
		system("timeout 3 >nul");
	}

	float Jack::CommonCard_Calculation(HitProperties Hit)
	{
		return std::max(0.f, GetProperty(ImplementedPropertiesAndBuffName::Attack)* // ATK *
			(Hit.FirstCardBonus + (CardDamageValue[int(Hit.HitCard._color)][Hit.Pointer] * std::min(1.f + Hit.CardMod, 5.f)))* //(カード補正 * min(5, 1 + -カードバフ) + 1stボーナス[0.5]) *
			ClassAtkBonus[int(UnitClass)] * //Class補正 *
			TriangleModifier[int(UnitClass)][int(Hit.Enemy->UnitClass)] * //Class相性補正 * 
			AttributeModifier[int(UnitAttribute)][int(Hit.Enemy->UnitAttribute)] * // Servant相性補正 *
			RandomGenerator::RandomModifier()* //乱数補正[0.9 - 1.1] *
			0.23f* // 攻撃補正[0.23] *
			(1 + GetProperty(ImplementedPropertiesAndBuffName::PercentAttackMod) - Hit.Enemy->GetProperty(ImplementedPropertiesAndBuffName::PercentDefenceMod))* //(1 + -攻撃力バフ + -敵防御力バフ) * 
			(Hit.HitCard._isCritical + 1)* //クリティカル補正[2] *
			(Hit.HasEX ? Hit.IsBAQBrave ? 3.5f : 2.f : 1.f)* //Extraボーナス[2 / 3.5] *
			(1 + std::min(Hit.TotalSpecificDamage, 5.f) - std::min(Hit.TotalSpecificDefence, 5.f) + std::min(Hit.HitCard._isCritical * GetProperty(ImplementedPropertiesAndBuffName::CriticalDamageMod), 5.f)) + //(1 + min(5, +-特攻威力バフ) + min(5, +-敵特防威力バフ) + min(5, +-クリティカル威力バフ))
			std::min(GetProperty(ImplementedPropertiesAndBuffName::ConstAttackMod), 1000.f) - std::min(Hit.Enemy->GetProperty(ImplementedPropertiesAndBuffName::ConstDefenceMod), 1000.f) + (GetProperty(ImplementedPropertiesAndBuffName::Attack) * Hit.BusterChainMod)); //+ min(1000, +-ダメージバフ) + min(1000, +-敵ダメージバフ) + (攻撃力 * Busterチェインボーナス[0.2])

	}

	const std::vector<float> Jack::GetBusterHitDamageMul() const
	{
		return {
			0.66f
		};
	}

	const std::vector<float> Jack::GetArtHitDamageMul() const
	{
		return {
			0.33f
		};
	}

	const std::vector<float> Jack::GetQuickHitDamageMul() const
	{
		return {
			0.07f,
			0.15f,
			0.23f,
			0.15f
		};
	}

	const std::vector<float> Jack::GetExHitDamageMul() const
	{
		return {
			0.10f,
			0.20f,
			0.30f,
			0.15f
		};
	}
	void Jack::Skill1Function(std::shared_ptr<Unit>ServentRef) {
		const static float Skill1QuickPerformanceUp[10]{
			0.30f,
			0.32f,
			0.34f,
			0.36f,
			0.38f,
			0.40f,
			0.42f,
			0.44f,
			0.46f,
			0.50f
		};
		auto PercentQuickPerformance = std::make_shared<RegularBuff>(ImplementedPropertiesAndBuffName::QuickPerformanceMod, Skill1QuickPerformanceUp[Skill1Level], 1, -1);
		auto Evasion = std::make_shared<UniqueBuff>(ImplementedPropertiesAndBuffName::Evasion, 1, -1);
		if (!FindUniqueBuff(ImplementedPropertiesAndBuffName::Evasion)) {
			UniqueBuffs[ImplementedPropertiesAndBuffName::Evasion] = Evasion;
			RegisterRoundStartSpecial(Evasion);
		}
		AddRegularBuff(PercentQuickPerformance);
		RegisterRoundEndSpecial(PercentQuickPerformance);
	}
	void Jack::Skill2Function(std::shared_ptr<Unit>ServentRef) {
		const static float Skill2CriticalChanceDown[10]{
			0.1f,
			0.12f,
			0.14f,
			0.16f,
			0.18f,
			0.20f,
			0.22f,
			0.24f,
			0.26f,
			0.3f
		};
		auto CriticalChanceDown = std::make_shared<RegularBuff>(ImplementedPropertiesAndBuffName::CriticalChanceMod, Skill2CriticalChanceDown[Skill1Level], 1, -1);
		ServentRef->AddDebuff(CriticalChanceDown);
	}
	void Jack::Skill3Function(std::shared_ptr<Unit>ServentRef) {
		const static float Skill3HealthRecover[10]{
			500.f,
			700.f,
			900.f,
			1100.f,
			1300.f,
			1500.f,
			1700.f,
			1900.f,
			2100.f,
			2500.f
		};
		ServentRef->ChangeHealth(Skill3HealthRecover[Skill3Level]);
	}
}
