#include "Skill.h"
#include "Unit.h"

void SpecialSkill::operator()(std::shared_ptr<Unit> UnitRef, std::shared_ptr<Unit> Self)
{
	SpecialSkillFunction(UnitRef, Self);
}

SpecialSkill::SpecialSkill(std::string _Tag):
	Tag(_Tag)
{
}

Buff::Buff(std::string name, bool _IsUnique, int _RoundCount, int _NumCount) :
	SpecialSkill(name),
	IsUnique(_IsUnique),
	RoundCount(_RoundCount),
	NumCount(_NumCount)
{
	IsBuff = true;
	SpecialSkillFunction = ImplementedSpecialFunctions::NoFunction;
}

UniqueBuff::UniqueBuff(std::string name, int _RoundCount, int _NumCount):
	Buff(name,true,_RoundCount,_NumCount)
{
}

RegularBuff::RegularBuff(std::string name,float _Mod, int _RoundCount, int _NumCount):
Buff(name, false, _RoundCount, _NumCount),
Mod(_Mod)
{
}

void RegularBuff::RemoveBuff(std::shared_ptr<Unit> UnitRef)
{
	UnitRef->GetProperty(Tag) -= Mod;
}

void RegularBuff::AddBuff(std::shared_ptr<Unit> UnitRef)
{
	UnitRef->GetProperty(Tag) += Mod;
}

SpecialSkillFunctionType ImplementedSpecialFunctions::NoFunction = [](std::shared_ptr<Unit>, std::shared_ptr<Unit>) {

};