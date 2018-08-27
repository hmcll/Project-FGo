#pragma once
#include "EnemyMob.h"

class HandFactory {
	class Hand : public EnemyMob {
		virtual void CallNP(std::shared_ptr<Unit> UnitTo) override;
		virtual void UseSkill(std::shared_ptr<Unit> UnitTo) override;
		virtual void Attack(std::shared_ptr<Unit> UnitTo) override;
	public:
		Hand(int id, int lvl);
		virtual void Move(std::shared_ptr<Unit> To) override;

	};
public:
	static std::shared_ptr<EnemyMob> MakeHand(int id, int lvl);
};