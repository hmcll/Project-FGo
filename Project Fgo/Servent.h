#pragma once
#include "Unit.h"
#include "HelperClasses.h"

#define IMPLEMENTED_SERVENT_DECLERATION(CLASS) \
public:\
static std::shared_ptr<CLASS> CreateServent(int NPLevel, int Skill1Level, int Skill2Level, int Skill3Level, int HP, int Atk);\
virtual float CommonCard_Calculation(HitProperties hit) override;\
virtual const std::vector<float> GetBusterHitDamageMul() const override;\
virtual const std::vector<float> GetArtHitDamageMul()const override;\
virtual const std::vector<float> GetQuickHitDamageMul()const override;\
virtual const std::vector<float> GetExHitDamageMul()const override;\
virtual void Skill1Function(std::shared_ptr<Unit> UnitRef) override;\
virtual void Skill2Function(std::shared_ptr<Unit> UnitRef) override;\
virtual void Skill3Function(std::shared_ptr<Unit> UnitRef) override;


#define IMPLEMENTED_SERVENT_IMPLEMENTATION(CLASS)\
std::shared_ptr<CLASS> CLASS::CreateServent(int NPLevel, int Skill1Level, int Skill2Level, int Skill3Level, int HP, int Atk){return std::make_shared<CLASS>(NPLevel, Skill1Level, Skill2Level, Skill3Level, HP, Atk);}

class EnemyMob;
class Servent :public Unit {
public:
	CardColor NoblePhantasmColor;
	Skill Skill1, Skill2, Skill3;
	int Skill1Level, Skill2Level, Skill3Level;

	virtual const std::vector<float> GetBusterHitDamageMul() const = 0;
	virtual const std::vector<float> GetArtHitDamageMul() const = 0;
	virtual const std::vector<float> GetQuickHitDamageMul() const = 0;
	virtual const std::vector<float> GetExHitDamageMul() const = 0;

	virtual float CommonCard_Calculation(HitProperties Hit) = 0;
	virtual void CallNoblePhantasm(std::shared_ptr<EnemyMob> To) = 0;

	void UseSkill1(std::shared_ptr<Unit> ServentRef);
	void UseSkill2(std::shared_ptr<Unit> ServentRef);
	void UseSkill3(std::shared_ptr<Unit> ServentRef);

	void ChargeNP(float num);

	std::string GetCard(int id);
	CardColor GetCardColor(int id);
protected:
	void RegisterProperties(float Attack, float Health, float BusterHit, float ArtHit, float QuickHit, float EXHit, float BusterNum, float ArtNum, float QuickNum, float NPLevel, std::string _Name, Class _UnitClass, float NPChargeAtk, float NPChargeDef, float StarAbsorbtion, float StarGeneration, float DeathRate, Attribute _UnitAttribute, int _Skill1Level, int _Skill2Level, int _Skill3Level);

	virtual void Skill1Function(std::shared_ptr<Unit> UnitRef) = 0;
	virtual void Skill2Function(std::shared_ptr<Unit> UnitRef) = 0;
	virtual void Skill3Function(std::shared_ptr<Unit> UnitRef) = 0;

};