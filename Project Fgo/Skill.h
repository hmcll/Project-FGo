#pragma once
#include <functional>
#include <memory>
class Unit;

typedef std::function<void(std::shared_ptr<Unit>, std::shared_ptr<Unit>)> SpecialSkillFunctionType;

class SpecialSkill {
public:
	SpecialSkillFunctionType SpecialSkillFunction;
	std::string Tag;
	void operator()(std::shared_ptr<Unit> UnitRef, std::shared_ptr<Unit> Self);
	bool IsBuff = false;
	SpecialSkill(std::string _Tag);
	int RegisterCount = 1;
private:
	SpecialSkill();
};
class Buff :public SpecialSkill {
public:
	int RoundCount, NumCount;
	bool IsUnique;
	virtual void RemoveBuff(std::shared_ptr<Unit> UnitRef) = 0;
protected:
	Buff(std::string name, bool _IsUnique, int _RoundCount, int _NumCount);
};

class UniqueBuff :public Buff{
public:
	UniqueBuff(std::string name, int _RoundCount, int _NumCount);
	void RemoveBuff(std::shared_ptr<Unit> UnitRef) override {};
};

class RegularBuff :public Buff {
	float Mod;
public:
	RegularBuff(std::string name,float _Mod, int _RoundCount, int _NumCount);
	void RemoveBuff(std::shared_ptr<Unit> UnitRef) override;
	void AddBuff(std::shared_ptr<Unit> UnitRef);
};
class ImplementedSpecialFunctions{
public:
	static SpecialSkillFunctionType NoFunction;
};