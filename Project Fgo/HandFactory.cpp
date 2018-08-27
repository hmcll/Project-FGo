#include "HandFactory.h"
#include "HelperClasses.h"
#include "RandomGenerator.h"
#include "Servent.h"
#include "IOHolder.h"
#include "DataProcessor.h"

void HandFactory::Hand::CallNP(std::shared_ptr<Unit> UnitTo)
{
	float TotalSpecificDamage = 0, TotalSpecificDefence = 0;

	for (unsigned TraitIterator = 0; TraitIterator < Traits.size(); TraitIterator++) {
		if (Traits[TraitIterator]) {
			TotalSpecificDefence += UnitTo->GetProperty(SpecificDefenceUpTraitName[TraitIterator]);
		}
	}
	for (unsigned TraitIterator = 0; TraitIterator < Traits.size(); TraitIterator++) {
		if (UnitTo->Traits[TraitIterator]) {
			TotalSpecificDamage += GetProperty(SpecificDamageUpTraitName[TraitIterator]);
		}
	}

	float TotalDamage = GetProperty(ImplementedPropertiesAndBuffName::Attack)* //ATK *
		GetProperty(ImplementedPropertiesAndBuffName::NPMultiplier) * //宝具倍率 *
		0.23f* //攻撃補正[0.23] *
		ClassAtkBonus[int(UnitClass)] * //Class補正 *
		TriangleModifier[int(UnitClass)][int(UnitTo->UnitClass)] * //Class相性補正 *
		AttributeModifier[int(UnitAttribute)][int(UnitTo->UnitAttribute)] * //Servant相性補正 *
		RandomGenerator::RandomModifier()* //乱数補正[0.9 - 1.1] *
		(1 + GetProperty(ImplementedPropertiesAndBuffName::PercentAttackMod) - UnitTo->GetProperty(ImplementedPropertiesAndBuffName::PercentDefenceMod))* //(1 + -攻撃力バフ - 敵防御力バフ) *  
		(1 + std::min(TotalSpecificDamage, 5.f) + std::min(GetProperty(ImplementedPropertiesAndBuffName::NoblePhantasmDamageMod), 5.f)) * //(1 + min(5, +-特攻威力バフ) + min(5, +-宝具威力バフ)) *
		1.f//宝具特攻
		+ std::min(GetProperty(ImplementedPropertiesAndBuffName::ConstAttackMod), 1000.f)//+ min(1000, +-ダメージバフ) + min(1000, +-敵ダメージバフ)
		;

	float TotalNp = UnitTo->GetProperty(ImplementedPropertiesAndBuffName::NPChargeDef)*
		std::min(5.f, 1 + UnitTo->GetProperty(ImplementedPropertiesAndBuffName::NPBuff))*
		std::min(5.f, 1 + UnitTo->GetProperty(ImplementedPropertiesAndBuffName::NPDefBuff))*
		GetProperty(ImplementedPropertiesAndBuffName::ATDR)
		;

	std::static_pointer_cast<Servent>(UnitTo)->ChargeNP(TotalNp);
	UnitTo->ChangeHealth(-1 * TotalDamage);

	IOHolder::OutLine(Name, "对", UnitTo->Name, "造成了", TotalDamage, "点伤害");
	system("timeout 3 >nul");
}

void HandFactory::Hand::UseSkill(std::shared_ptr<Unit> UnitTo)
{
}

void HandFactory::Hand::Attack(std::shared_ptr<Unit> UnitTo)
{
	bool IsCrit = RandomGenerator::CriticalDistributor(10);
	float cardMod = 0, TotalSpecificDamage = 0, TotalSpecificDefence = 0;

	for (unsigned TraitIterator = 0; TraitIterator < Traits.size(); TraitIterator++) {
		if (Traits[TraitIterator]) {
			TotalSpecificDefence += UnitTo->GetProperty(SpecificDefenceUpTraitName[TraitIterator]);
		}
	}
	for (unsigned TraitIterator = 0; TraitIterator < Traits.size(); TraitIterator++) {
		if (UnitTo->Traits[TraitIterator]) {
			TotalSpecificDamage += GetProperty(SpecificDamageUpTraitName[TraitIterator]);
		}
	}

	float TotalDamage = std::max(0.f,
		GetProperty(ImplementedPropertiesAndBuffName::Attack)* // ATK *
		ClassAtkBonus[int(UnitClass)] * //Class補正 *
		TriangleModifier[int(UnitClass)][int(UnitTo->UnitClass)] * //Class相性補正 * 
		AttributeModifier[int(UnitAttribute)][int(UnitTo->UnitAttribute)] * // Servant相性補正 *
		RandomGenerator::RandomModifier()* //乱数補正[0.9 - 1.1] *
		0.23f* // 攻撃補正[0.23] *
		(1 + GetProperty(ImplementedPropertiesAndBuffName::PercentAttackMod) - UnitTo->GetProperty(ImplementedPropertiesAndBuffName::PercentDefenceMod))* //(1 + -攻撃力バフ + -敵防御力バフ) * 
		(IsCrit + 1)* //クリティカル補正[2] *
		(1 + std::min(TotalSpecificDamage, 5.f) - std::min(TotalSpecificDefence, 5.f) + std::min(IsCrit * GetProperty(ImplementedPropertiesAndBuffName::CriticalDamageMod), 5.f)) + //(1 + min(5, +-特攻威力バフ) + min(5, +-敵特防威力バフ) + min(5, +-クリティカル威力バフ))
		std::min(GetProperty(ImplementedPropertiesAndBuffName::ConstAttackMod), 1000.f) - std::min(UnitTo->GetProperty(ImplementedPropertiesAndBuffName::ConstDefenceMod), 1000.f)); //+ min(1000, +-ダメージバフ) + min(1000, +-敵ダメージバフ) + (攻撃力 * Busterチェインボーナス[0.2])

	float TotalNp = UnitTo->GetProperty(ImplementedPropertiesAndBuffName::NPChargeDef)*
		std::min(5.f, 1 + UnitTo->GetProperty(ImplementedPropertiesAndBuffName::NPBuff))*
		std::min(5.f, 1 + UnitTo->GetProperty(ImplementedPropertiesAndBuffName::NPDefBuff))*
		GetProperty(ImplementedPropertiesAndBuffName::ATDR)*
		(IsCrit + 1)
		;
	
	std::static_pointer_cast<Servent>(UnitTo)->ChargeNP(TotalNp);
	UnitTo->ChangeHealth(-1 * TotalDamage);

	IOHolder::OutLine(Name, "对", UnitTo->Name, "造成了", TotalDamage, "点伤害");
	system("timeout 3 >nul");

}

HandFactory::Hand::Hand(int id, int lvl)
{
	auto M = ImplemetedMobProperties.at(id);
	GetProperty(ImplementedPropertiesAndBuffName::CR) = M.CR;
	GetProperty(ImplementedPropertiesAndBuffName::DSR) = M.DSR;
	GetProperty(ImplementedPropertiesAndBuffName::DTDR) = M.DTDR;
	GetProperty(ImplementedPropertiesAndBuffName::ATDR) = M.ATDR;
	GetProperty(ImplementedPropertiesAndBuffName::NPMultiplier) = M.NPMultiplier;
	AT = M.AT;
	CT = M.CT;
	GetProperty(ImplementedPropertiesAndBuffName::DeathRate) = M.DR;
	UnitAttribute = M.MobAttribute;
	UnitClass = M.MobClass;
	auto HA = M.HealthAndAttackFromLvl(lvl);
	GetProperty(ImplementedPropertiesAndBuffName::Health) = HA.first;
	GetProperty(ImplementedPropertiesAndBuffName::Attack) = HA.second;
	GetProperty(ImplementedPropertiesAndBuffName::MaxHealth) = HA.first;
	Name = M.Name;
	NoblePhantasmName = M.NPName;
}

void HandFactory::Hand::Move(std::shared_ptr<Unit> To)
{
	if (!To) {
		ct++;
		if (ct > CT)ct = CT;
		return;
	}
	else if (CT == ct) {
		ct = 0;
		CallNP(To);
	}
	else {
		Attack(To);
		ct++;
		if (ct > CT) ct = CT;
	}

}

std::shared_ptr<EnemyMob> HandFactory::MakeHand(int id, int lvl)
{
	return std::make_shared<Hand>(id,lvl);
}
