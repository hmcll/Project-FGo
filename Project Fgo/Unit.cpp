#include "Unit.h"
#include "Skill.h"
#include "RandomGenerator.h"
#include <utility>

void Unit::RegisterAttackSpecial(std::shared_ptr<SpecialSkill> SK)
{
	if (SK->IsBuff) std::static_pointer_cast<Buff>(SK)->RegisterCount++;
	AttackSpecialEffectFunctionCollection.push_back(SK);
}

void Unit::OnAttack(std::shared_ptr<Unit> UnitRef)
{
	UpdateFunctionCollection(AttackSpecialEffectFunctionCollection, UnitRef);
}

void Unit::RegisterHitSpecial(std::shared_ptr<SpecialSkill> SK)
{
	if (SK->IsBuff) std::static_pointer_cast<Buff>(SK)->RegisterCount++;
	HitSpecialEffectFunctionCollection.push_back(SK);
}

void Unit::OnHit()
{
	UpdateFunctionCollection(HitSpecialEffectFunctionCollection, nullptr);
}

void Unit::RegisterDebuffSpecial(std::shared_ptr<SpecialSkill> SK)
{
	if (SK->IsBuff) std::static_pointer_cast<Buff>(SK)->RegisterCount++;
	DebuffSpecialEffectFunctionCollection.push_back(SK);
}

void Unit::OnDebuff()
{
	UpdateFunctionCollection(DebuffSpecialEffectFunctionCollection, nullptr);
}

void Unit::RegisterDeathSpecial(std::shared_ptr<SpecialSkill> SK)
{
	if (SK->IsBuff) std::static_pointer_cast<Buff>(SK)->RegisterCount++;
	DeathSpecialEffectFunctionCollection.push_back(SK);
}

void Unit::OnDeath()
{
	UpdateFunctionCollection(DeathSpecialEffectFunctionCollection, nullptr);
}

void Unit::RegisterEnterSpecial(std::shared_ptr<SpecialSkill> SK)
{
	if (SK->IsBuff) std::static_pointer_cast<Buff>(SK)->RegisterCount++;
	EnterSpecialEffectFunctionCollection.push_back(SK);
}

void Unit::OnEnter()
{
	UpdateFunctionCollection(EnterSpecialEffectFunctionCollection, nullptr);
}

void Unit::RegisterRoundStartSpecial(std::shared_ptr<SpecialSkill> SK)
{
	if (SK->IsBuff) std::static_pointer_cast<Buff>(SK)->RegisterCount++;
	RoundStartSpecialEffectFunctionCollection.push_back(SK);
}

void Unit::OnRoundStart()
{
	for (auto SpecialFunctionIterator = RoundStartSpecialEffectFunctionCollection.begin(); SpecialFunctionIterator != RoundStartSpecialEffectFunctionCollection.end();) {
		if (!((*SpecialFunctionIterator).use_count() == (**SpecialFunctionIterator).RegisterCount)){
			SpecialFunctionIterator = RoundStartSpecialEffectFunctionCollection.erase(SpecialFunctionIterator);
			continue;
		}
		(**SpecialFunctionIterator)(nullptr, shared_from_this());
		if ((*SpecialFunctionIterator)->IsBuff) {
			auto ThisBuff = std::static_pointer_cast<Buff>(*SpecialFunctionIterator);
			if (!(--ThisBuff->RoundCount)) {
				ThisBuff->RemoveBuff(shared_from_this());
				if (ThisBuff->IsUnique) {
					UniqueBuffs[ThisBuff->Tag].reset();
				}
				else {
					RegularBuffs[ThisBuff->Tag].erase(*SpecialFunctionIterator);
				}
				SpecialFunctionIterator = RoundStartSpecialEffectFunctionCollection.erase(SpecialFunctionIterator);
				continue;
			}
		}
		SpecialFunctionIterator++;
	}
}

void Unit::RegisterRoundEndSpecial(std::shared_ptr<SpecialSkill> SK)
{
	if (SK->IsBuff) std::static_pointer_cast<Buff>(SK)->RegisterCount++;
	RoundEndSpecialEffectFunctionCollection.push_back(SK);
}

void Unit::OnRoundEnd()
{
	for (auto SpecialFunctionIterator = RoundEndSpecialEffectFunctionCollection.begin(); SpecialFunctionIterator != RoundEndSpecialEffectFunctionCollection.end();) {
		if (!((*SpecialFunctionIterator).use_count() == (**SpecialFunctionIterator).RegisterCount)) {
			SpecialFunctionIterator = RoundEndSpecialEffectFunctionCollection.erase(SpecialFunctionIterator);
			continue;
		}
		(**SpecialFunctionIterator)(nullptr, shared_from_this());
		if ((*SpecialFunctionIterator)->IsBuff) {
			auto ThisBuff = std::static_pointer_cast<Buff>(*SpecialFunctionIterator);
			if (!(--ThisBuff->RoundCount)) {
				ThisBuff->RemoveBuff(shared_from_this());
				if (ThisBuff->IsUnique) {
					UniqueBuffs[ThisBuff->Tag].reset();
				}
				else {
					RegularBuffs[ThisBuff->Tag].erase(*SpecialFunctionIterator);
				}
				SpecialFunctionIterator = RoundEndSpecialEffectFunctionCollection.erase(SpecialFunctionIterator);
				continue;
			}
		}
		SpecialFunctionIterator++;
	}
}

void Unit::ChangeHealth(float health)
{
	if (health > 0)health *= (1 + GetProperty(ImplementedPropertiesAndBuffName::HealthRecoverMod));
	else if (UniqueBuffs[ImplementedPropertiesAndBuffName::Invincibility] || UniqueBuffs[ImplementedPropertiesAndBuffName::Evasion])health = 0;
	GetProperty(ImplementedPropertiesAndBuffName::Health) += health;
	if (GetProperty(ImplementedPropertiesAndBuffName::Health) > GetProperty(ImplementedPropertiesAndBuffName::MaxHealth) ){
		GetProperty(ImplementedPropertiesAndBuffName::Health) = GetProperty(ImplementedPropertiesAndBuffName::MaxHealth);
	}
	if (GetProperty(ImplementedPropertiesAndBuffName::Health) < 0) {
		GetProperty(ImplementedPropertiesAndBuffName::Health)= 0;
	}
	if (health <= 0)
		OnHit();
}

void Unit::AddDebuff(std::shared_ptr<Buff> debuff)
{
	if (GetProperty(ImplementedPropertiesAndBuffName::DebuffImmunity)) {
		OnDebuff();
		return;
	}
	float SuccessRate = GetProperty(ImplementedPropertiesAndBuffName::DebuffSuccessRate) - GetProperty(ImplementedPropertiesAndBuffName::DebuffMod);
	float Succeed = SuccessRate - RandomGenerator::DebuffDistributor();
	if (Succeed > 0) {
		if (debuff->IsUnique) {
			if (!FindUniqueBuff(debuff->Tag)) {
				UniqueBuffs[debuff->Tag] = debuff;
				RegisterRoundStartSpecial(debuff);
			}
		}
		else {
			AddRegularBuff(std::static_pointer_cast<RegularBuff>(debuff));
			RegisterRoundStartSpecial(debuff);
		}
	}
}

inline void Unit::UpdateFunctionCollection(FunctionCollectionType & SpecialEffectFunctionCollection, std::shared_ptr<Unit> UnitRef)
{
	for (auto SpecialFunctionIterator = SpecialEffectFunctionCollection.begin(); SpecialFunctionIterator != SpecialEffectFunctionCollection.end();) {
		if (!((*SpecialFunctionIterator).use_count() == (**SpecialFunctionIterator).RegisterCount)) {
			SpecialFunctionIterator = SpecialEffectFunctionCollection.erase(SpecialFunctionIterator);
			continue;
		}
		(**SpecialFunctionIterator)(UnitRef, shared_from_this());
		if ((*SpecialFunctionIterator)->IsBuff) {
			auto ThisBuff = std::static_pointer_cast<Buff>(*SpecialFunctionIterator);
			if (!(--ThisBuff->NumCount)) {
				ThisBuff->RemoveBuff(shared_from_this());
				if (ThisBuff->IsUnique) {
					UniqueBuffs[ThisBuff->Tag].reset();
				}
				else {
					RegularBuffs[ThisBuff->Tag].erase(*SpecialFunctionIterator);
				}
				SpecialFunctionIterator = SpecialEffectFunctionCollection.erase(SpecialFunctionIterator);
				continue;
			}
		}
		SpecialFunctionIterator++;
	}
}

float & Unit::GetProperty(std::string PropertyName)
{
	try{
		return Properties[PropertyName];
	}
	catch (std::out_of_range) {
		Properties.insert(std::make_pair(PropertyName, 0.f));
		return Properties[PropertyName];
	}
}

bool Unit::FindUniqueBuff(std::string BuffName)
{
	if (!UniqueBuffs.count(BuffName)) {
		UniqueBuffs.insert(std::make_pair(BuffName, std::make_shared<SpecialSkill>(BuffName)));
		UniqueBuffs[BuffName].reset();
		return false;
	}
	else if (UniqueBuffs[BuffName].use_count() == 0) {
		return false;
	}
	return true;
}

void Unit::AddRegularBuff(std::shared_ptr<RegularBuff> Buff)
{
	if (!UniqueBuffs.count(Buff->Tag)) {
		RegularBuffs.insert(std::make_pair(Buff->Tag, std::unordered_set<std::shared_ptr<SpecialSkill>>()));
	}
	RegularBuffs[Buff->Tag].insert(Buff);
}
