#pragma once
#include "Servent.h"

namespace ImplementedServents {
	class Mashu :public Servent{

	IMPLEMENTED_SERVENT_DECLERATION(Mashu)

	public:
		Mashu(int NPLevel, int Skill1Level, int Skill2Level, int Skill3Level, int HP, int Atk);

		virtual void CallNoblePhantasm(std::shared_ptr<EnemyMob> To) override;
	};
}
