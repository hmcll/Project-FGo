#pragma once
#include "Unit.h"

class EnemyMob :public Unit {
protected:
	int CT, AT;
	virtual void CallNP(std::shared_ptr<Unit> UnitTo) = 0;
	virtual void UseSkill(std::shared_ptr<Unit> UnitTo) = 0;
	virtual void Attack(std::shared_ptr<Unit> UnitTo) = 0;
public:
	int ct = 0;
	static int GetAT(std::shared_ptr<EnemyMob>This) { return This ? This->AT : 0 ; }
	virtual void Move(std::shared_ptr<Unit> To) = 0;
};