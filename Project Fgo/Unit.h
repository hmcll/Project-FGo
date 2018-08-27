#pragma once
#include <string>
#include <array>
#include <vector>
#include <unordered_set>
#include "CommonProperties.h"
#include "Skill.h"

class SpecialSkill;
class RegularBuff;
class Unit :public std::enable_shared_from_this<Unit>{
public:
	std::string Name, NoblePhantasmName;
	Class UnitClass;
	Attribute UnitAttribute;
	std::array<bool, 25> Traits;
	std::unordered_map<std::string, std::shared_ptr<SpecialSkill>> UniqueBuffs;
	std::unordered_map<std::string, std::unordered_set<std::shared_ptr<SpecialSkill>>> RegularBuffs;
	std::unordered_map<std::string, float> Properties;

private:
	//Registered Function Collections

	typedef std::vector<std::shared_ptr<SpecialSkill>> FunctionCollectionType;

	FunctionCollectionType AttackSpecialEffectFunctionCollection;

	FunctionCollectionType HitSpecialEffectFunctionCollection;

	FunctionCollectionType DebuffSpecialEffectFunctionCollection;

	FunctionCollectionType DeathSpecialEffectFunctionCollection;

	FunctionCollectionType EnterSpecialEffectFunctionCollection;

	FunctionCollectionType RoundStartSpecialEffectFunctionCollection;

	FunctionCollectionType RoundEndSpecialEffectFunctionCollection;

public:

	void RegisterAttackSpecial(std::shared_ptr<SpecialSkill> SK);

	void OnAttack(std::shared_ptr<Unit> UnitRef);

	void RegisterHitSpecial(std::shared_ptr<SpecialSkill> SK);

	void OnHit();

	void RegisterDebuffSpecial(std::shared_ptr<SpecialSkill> SK);

	void OnDebuff();

	void RegisterDeathSpecial(std::shared_ptr<SpecialSkill> SK);

	void OnDeath();

	void RegisterEnterSpecial(std::shared_ptr<SpecialSkill> SK);

	void OnEnter();

	void RegisterRoundStartSpecial(std::shared_ptr<SpecialSkill> SK);

	void OnRoundStart();

	void RegisterRoundEndSpecial(std::shared_ptr<SpecialSkill> SK);

	void OnRoundEnd();

	void ChangeHealth(float health);

	void AddDebuff(std::shared_ptr<Buff> debuff);

private:
	inline void UpdateFunctionCollection(FunctionCollectionType& SpecialEffectFunctionCollection, std::shared_ptr<Unit> UnitRef);

protected:
	template<typename T, typename ...RestTraits>
	void RegisterTraits(const T& ThisTrait, const RestTraits& ... Rest) {
		Traits[int(ThisTrait)] = true;
		RegisterTraits(Rest...);
	}

	template <typename T>
	void RegisterTraits(const T& ThisTrait) {
		Traits[int(ThisTrait)] = true;
	}
public:
	float& GetProperty(std::string PropertyName);

	bool FindUniqueBuff(std::string BuffName);
	void AddRegularBuff(std::shared_ptr<RegularBuff> Buff);
};