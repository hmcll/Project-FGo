#include "Servent.h"

void Servent::UseSkill1(std::shared_ptr<Unit> ServentRef)
{
	if (!Skill1.CoolDownCount) {
		Skill1.CoolDownCount = Skill1.CoolDown;
		Skill1Function(ServentRef);
	}
}

void Servent::UseSkill2(std::shared_ptr<Unit> ServentRef)
{
	if (!Skill2.CoolDownCount) {
		Skill2.CoolDownCount = Skill2.CoolDown;
		Skill2Function(ServentRef);
	}
}
void Servent::UseSkill3(std::shared_ptr<Unit> ServentRef)
{
	if (!Skill3.CoolDownCount) {
		Skill3.CoolDownCount = Skill3.CoolDown;
		Skill3Function(ServentRef);
	}
}

void Servent::ChargeNP(float num)
{
	GetProperty(ImplementedPropertiesAndBuffName::NP) += num;
	if (GetProperty(ImplementedPropertiesAndBuffName::NP) > GetProperty(ImplementedPropertiesAndBuffName::MaxNP))GetProperty(ImplementedPropertiesAndBuffName::NP) = GetProperty(ImplementedPropertiesAndBuffName::MaxNP);
}

std::string Servent::GetCard(int id)
{
	id++;
	return Name +
		(id > GetProperty(ImplementedPropertiesAndBuffName::BusterNum) ?
			id > (GetProperty(ImplementedPropertiesAndBuffName::BusterNum) + GetProperty(ImplementedPropertiesAndBuffName::ArtHit)) ?
			id > 5 ?
			NoblePhantasmName : "Quick"
			: "Art"
			: "Buster");
}

CardColor Servent::GetCardColor(int id)
{
	id++;
	return id > GetProperty(ImplementedPropertiesAndBuffName::BusterNum) ?
		id > (GetProperty(ImplementedPropertiesAndBuffName::BusterNum) + GetProperty(ImplementedPropertiesAndBuffName::ArtHit)) ?
		id > 5 ?
		NoblePhantasmColor : CardColor::Quick
		: CardColor::Art
		: CardColor::Buster;
}

void Servent::RegisterProperties(float Attack, float Health, float BusterHit, float ArtHit, float QuickHit, float EXHit, float BusterNum, float ArtNum, float QuickNum, float NPLevel, std::string _Name, Class _UnitClass, float NPChargeAtk, float NPChargeDef, float StarAbsorbtion, float StarGeneration, float DeathRate, Attribute _UnitAttribute, int _Skill1Level, int _Skill2Level, int _Skill3Level) {
	GetProperty(ImplementedPropertiesAndBuffName::Attack) = Attack;
	GetProperty(ImplementedPropertiesAndBuffName::Health) = Health;
	GetProperty(ImplementedPropertiesAndBuffName::MaxHealth) = Health;
	GetProperty(ImplementedPropertiesAndBuffName::BusterHit) = BusterHit;
	GetProperty(ImplementedPropertiesAndBuffName::ArtHit) = ArtHit;
	GetProperty(ImplementedPropertiesAndBuffName::QuickHit) = QuickHit;
	GetProperty(ImplementedPropertiesAndBuffName::EXHit) = EXHit;
	GetProperty(ImplementedPropertiesAndBuffName::BusterNum) = BusterNum;
	GetProperty(ImplementedPropertiesAndBuffName::ArtNum) = ArtNum;
	GetProperty(ImplementedPropertiesAndBuffName::QuickNum) = QuickNum;
	GetProperty(ImplementedPropertiesAndBuffName::MaxNP) = (NPLevel == 0) ? 1.f : (NPLevel == 4) ? 3.f : 2.f;
	GetProperty(ImplementedPropertiesAndBuffName::NPChargeAtk) = NPChargeAtk;
	GetProperty(ImplementedPropertiesAndBuffName::NPChargeDef) = NPChargeDef;
	GetProperty(ImplementedPropertiesAndBuffName::StarAbsorption) = StarAbsorbtion;
	GetProperty(ImplementedPropertiesAndBuffName::StarGeneration) = StarGeneration;
	GetProperty(ImplementedPropertiesAndBuffName::DeathRate) = DeathRate;
	GetProperty(ImplementedPropertiesAndBuffName::DebuffSuccessRate) = 1.f;
	GetProperty(ImplementedPropertiesAndBuffName::NoblePhantasmLevel) = NPLevel;
	Name = _Name;
	UnitClass = _UnitClass;
	UnitAttribute = _UnitAttribute;
	Skill1.CoolDownCount = 0;
	Skill2.CoolDownCount = 0;
	Skill3.CoolDownCount = 0;
	Skill1Level = _Skill1Level;
	Skill2Level = _Skill2Level;
	Skill3Level = _Skill3Level;

}
