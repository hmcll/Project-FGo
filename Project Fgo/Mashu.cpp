#include "Mashu.h"
#include "GameInstance.h"
#include "Skill.h"
#include "RandomGenerator.h"

namespace ImplementedServents {

	IMPLEMENTED_SERVENT_IMPLEMENTATION(Mashu)

	Mashu::Mashu(int NPLevel, int _Skill1Level, int _Skill2Level, int _Skill3Level, int HP, int Atk)
	{
		NPLevel--;
		_Skill1Level--;
		_Skill2Level--;
		_Skill3Level--;

		NoblePhantasmColor = CardColor::Art;
		NoblePhantasmName = "Lord Camelot";


		RegisterProperties(HP, Atk, 1.f, 2.f, 2.f, 3.f, 2.f, 2.f, 1.f, NPLevel, "Mashu", Class::Shielder, 0.0084f, 0.03f, 99.f, 0.099f, 0.245f, Attribute::Earth, _Skill1Level, _Skill2Level, _Skill3Level);

		GetProperty(ImplementedPropertiesAndBuffName::DebuffMod) = 0.2f;
		GetProperty(ImplementedPropertiesAndBuffName::QuickPerformanceMod) = 0.06f;

		RegisterTraits(Trait::DemiServants, Trait::EarthOrSky, Trait::Female, Trait::Humanoid, Trait::Riding, Trait::Servant, Trait::WeakToEnumaElish, Trait::Lawful, Trait::Good);

		Skill1.CoolDown = Skill1Level == 9 ? 5 : Skill1Level > 4 ? 6 : 7;
		Skill1.ChooseSkillTarget = Skill::NoNeedToChoose;
		Skill1.Name = "ExaltedImperviousWallofSnowflakes";

		Skill2.CoolDown = Skill2Level == 9 ? 7 : Skill2Level > 4 ? 8 : 9;
		Skill2.ChooseSkillTarget = Skill::TeamMate;
		Skill2.Name = "ObscurantWallofChalk";

		Skill3.CoolDown = Skill3Level == 9 ? 6 : Skill3Level > 4 ? 7 : 8;
		Skill3.ChooseSkillTarget = Skill::NoNeedToChoose;
		Skill3.Name = "ShieldofRousingResolution";
	}

	void Mashu::CallNoblePhantasm(std::shared_ptr<EnemyMob> To)
	{
		const static float NPPercentDefenceUp[5] = {
			0.3f,
			0.35f,
			0.4f,
			0.45f,
			0.5f
		};
		const static int NPConstDefenceUp[5] = {
			100,
			550,
			775,
			888,
			1000
		};
		int NPChargeLevel = GetProperty(ImplementedPropertiesAndBuffName::NP) / 100;
		int NPLevel = GetProperty(ImplementedPropertiesAndBuffName::NoblePhantasmLevel);
		for (auto ServentIterator : GameInstance::Get()->OnFieldMyServentCollection)
		{
			if (!ServentIterator) continue;
			auto PercentDefenceIncrease = std::make_shared<RegularBuff>(ImplementedPropertiesAndBuffName::PercentDefenceMod, NPPercentDefenceUp[NPChargeLevel], 3, -1);
			auto ConstDefenceIncrease = std::make_shared<RegularBuff>(ImplementedPropertiesAndBuffName::ConstDefenceMod, NPConstDefenceUp[NPLevel], 3, -1);
			PercentDefenceIncrease->AddBuff(shared_from_this());
			ConstDefenceIncrease->AddBuff(shared_from_this());
			ServentIterator->AddRegularBuff(PercentDefenceIncrease);
			ServentIterator->AddRegularBuff(ConstDefenceIncrease);
			ServentIterator->RegisterRoundStartSpecial(PercentDefenceIncrease);
			ServentIterator->RegisterRoundStartSpecial(ConstDefenceIncrease);
			if (!(ServentIterator->Name == Name)) {
				auto PercentAttackIncrease = std::make_shared<RegularBuff>(ImplementedPropertiesAndBuffName::PercentAttackMod, 0.3f, 3, -1);
				PercentAttackIncrease->AddBuff(ServentIterator);
				ServentIterator->AddRegularBuff(PercentAttackIncrease);
				ServentIterator->RegisterRoundEndSpecial(PercentAttackIncrease);
			}
		}
	}

	float Mashu::CommonCard_Calculation(HitProperties Hit)
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


	const std::vector<float> Mashu::GetBusterHitDamageMul() const
	{
		return {};
	}
	const std::vector<float> Mashu::GetArtHitDamageMul() const
	{
		return { 0.33f };
	}
	const std::vector<float> Mashu::GetQuickHitDamageMul() const
	{
		return { 0.25f };
	}
	const std::vector<float> Mashu::GetExHitDamageMul() const
	{
		return {
			0.12f,
			0.25f,
		};
	}
	void Mashu::Skill1Function(std::shared_ptr<Unit>ServentRef) {
		const static float Skill1PercentDefenceUp[10]{
			0.15f,
			0.155f,
			0.16f,
			0.165f,
			0.17f,
			0.175f,
			0.18f,
			0.185f,
			0.19f,
			0.2f
		};
		for (auto &Iterator : GameInstance::Get()->OnFieldMyServentCollection) {
			auto PercentDefenceIncrease = std::make_shared<RegularBuff>(ImplementedPropertiesAndBuffName::PercentDefenceMod, Skill1PercentDefenceUp[Skill1Level], 3, -1);
			auto ConstDefenceIncrease = std::make_shared<RegularBuff>(ImplementedPropertiesAndBuffName::ConstDefenceMod, 2000.f, -1, 1);
			if (!Iterator)continue;
			Iterator->GetProperty(ImplementedPropertiesAndBuffName::PercentDefenceMod) += Skill1PercentDefenceUp[Skill1Level];
			Iterator->GetProperty(ImplementedPropertiesAndBuffName::ConstDefenceMod) += 2000;
			Iterator->AddRegularBuff(PercentDefenceIncrease);
			Iterator->AddRegularBuff(ConstDefenceIncrease);
			Iterator->RegisterRoundStartSpecial(PercentDefenceIncrease);
			Iterator->RegisterHitSpecial(ConstDefenceIncrease);
		}
	}
	void Mashu::Skill2Function(std::shared_ptr<Unit>ServentRef) {
		const static float Skill2NPUp[10]{
			0.1f,
			0.11f,
			0.12f,
			0.13f,
			0.14f,
			0.15f,
			0.16f,
			0.17f,
			0.18f,
			1.2f
		};
		if (!ServentRef->FindUniqueBuff(ImplementedPropertiesAndBuffName::Invincibility)) {
			auto Invincibility = std::make_shared<UniqueBuff>(ImplementedPropertiesAndBuffName::Invincibility, 1, -1);
			ServentRef->UniqueBuffs["Invincibility"] = Invincibility;
			ServentRef->RegisterRoundStartSpecial(Invincibility);
		}
		std::static_pointer_cast<Servent>(ServentRef)->ChargeNP(Skill2NPUp[Skill2Level]);
	}
	void Mashu::Skill3Function(std::shared_ptr<Unit>ServentRef) {
		const static float Skill3NPBuff[10]{
			2.0f,
			2.2f,
			2.4f,
			2.6f,
			2.8f,
			3.f,
			3.2f,
			3.4f,
			3.6f,
			4.f
		};
		const static float Skill3TCBuff[10]{
			1.0f,
			1.2f,
			1.4f,
			1.6f,
			1.8f,
			2.f,
			2.2f,
			2.4f,
			2.6f,
			3.f
		};

		auto NPBuff = std::make_shared<RegularBuff>(ImplementedPropertiesAndBuffName::NPBuff, Skill3NPBuff[Skill3Level], 1, -1);
		auto TargetConcentration = std::make_shared<RegularBuff>(ImplementedPropertiesAndBuffName::TargetConcentrationMod, Skill3TCBuff[Skill3Level], 1, -1);
		GetProperty(ImplementedPropertiesAndBuffName::NPBuff) += Skill3NPBuff[Skill3Level];
		GetProperty(ImplementedPropertiesAndBuffName::TargetConcentrationMod) += Skill3TCBuff[Skill3Level];
		AddRegularBuff(NPBuff);
		AddRegularBuff(TargetConcentration);
		RegisterRoundEndSpecial(NPBuff);
		RegisterRoundStartSpecial(TargetConcentration);
	}

}