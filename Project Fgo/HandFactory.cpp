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
		GetProperty(ImplementedPropertiesAndBuffName::NPMultiplier) * //���߱��� *
		0.23f* //�����a��[0.23] *
		ClassAtkBonus[int(UnitClass)] * //Class�a�� *
		TriangleModifier[int(UnitClass)][int(UnitTo->UnitClass)] * //Class�����a�� *
		AttributeModifier[int(UnitAttribute)][int(UnitTo->UnitAttribute)] * //Servant�����a�� *
		RandomGenerator::RandomModifier()* //�����a��[0.9 - 1.1] *
		(1 + GetProperty(ImplementedPropertiesAndBuffName::PercentAttackMod) - UnitTo->GetProperty(ImplementedPropertiesAndBuffName::PercentDefenceMod))* //(1 + -�������Х� - ���������Х�) *  
		(1 + std::min(TotalSpecificDamage, 5.f) + std::min(GetProperty(ImplementedPropertiesAndBuffName::NoblePhantasmDamageMod), 5.f)) * //(1 + min(5, +-�ع������Х�) + min(5, +-���������Х�)) *
		1.f//�����ع�
		+ std::min(GetProperty(ImplementedPropertiesAndBuffName::ConstAttackMod), 1000.f)//+ min(1000, +-����`���Х�) + min(1000, +-������`���Х�)
		;

	float TotalNp = UnitTo->GetProperty(ImplementedPropertiesAndBuffName::NPChargeDef)*
		std::min(5.f, 1 + UnitTo->GetProperty(ImplementedPropertiesAndBuffName::NPBuff))*
		std::min(5.f, 1 + UnitTo->GetProperty(ImplementedPropertiesAndBuffName::NPDefBuff))*
		GetProperty(ImplementedPropertiesAndBuffName::ATDR)
		;

	std::static_pointer_cast<Servent>(UnitTo)->ChargeNP(TotalNp);
	UnitTo->ChangeHealth(-1 * TotalDamage);

	IOHolder::OutLine(Name, "��", UnitTo->Name, "�����", TotalDamage, "���˺�");
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
		ClassAtkBonus[int(UnitClass)] * //Class�a�� *
		TriangleModifier[int(UnitClass)][int(UnitTo->UnitClass)] * //Class�����a�� * 
		AttributeModifier[int(UnitAttribute)][int(UnitTo->UnitAttribute)] * // Servant�����a�� *
		RandomGenerator::RandomModifier()* //�����a��[0.9 - 1.1] *
		0.23f* // �����a��[0.23] *
		(1 + GetProperty(ImplementedPropertiesAndBuffName::PercentAttackMod) - UnitTo->GetProperty(ImplementedPropertiesAndBuffName::PercentDefenceMod))* //(1 + -�������Х� + -���������Х�) * 
		(IsCrit + 1)* //����ƥ������a��[2] *
		(1 + std::min(TotalSpecificDamage, 5.f) - std::min(TotalSpecificDefence, 5.f) + std::min(IsCrit * GetProperty(ImplementedPropertiesAndBuffName::CriticalDamageMod), 5.f)) + //(1 + min(5, +-�ع������Х�) + min(5, +-���ط������Х�) + min(5, +-����ƥ����������Х�))
		std::min(GetProperty(ImplementedPropertiesAndBuffName::ConstAttackMod), 1000.f) - std::min(UnitTo->GetProperty(ImplementedPropertiesAndBuffName::ConstDefenceMod), 1000.f)); //+ min(1000, +-����`���Х�) + min(1000, +-������`���Х�) + (������ * Buster��������ܩ`�ʥ�[0.2])

	float TotalNp = UnitTo->GetProperty(ImplementedPropertiesAndBuffName::NPChargeDef)*
		std::min(5.f, 1 + UnitTo->GetProperty(ImplementedPropertiesAndBuffName::NPBuff))*
		std::min(5.f, 1 + UnitTo->GetProperty(ImplementedPropertiesAndBuffName::NPDefBuff))*
		GetProperty(ImplementedPropertiesAndBuffName::ATDR)*
		(IsCrit + 1)
		;
	
	std::static_pointer_cast<Servent>(UnitTo)->ChargeNP(TotalNp);
	UnitTo->ChangeHealth(-1 * TotalDamage);

	IOHolder::OutLine(Name, "��", UnitTo->Name, "�����", TotalDamage, "���˺�");
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
