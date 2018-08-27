#include "AIHandler.h"
#include "GameInstance.h"
#include "EnemyMob.h"
#include "Servent.h"
#include "RandomGenerator.h"


void AIHandler::Move()
{
	auto & OnFieldEnemyMobCollection = GameInstance::Get()->OnFieldEnemyMobCollection;
	auto & OnFieldMyServentCollection = GameInstance::Get()->OnFieldMyServentCollection;
	int AT[3] = {
		EnemyMob::GetAT(OnFieldEnemyMobCollection[0]),
		EnemyMob::GetAT(OnFieldEnemyMobCollection[1]),
		EnemyMob::GetAT(OnFieldEnemyMobCollection[2])
	};
	int MoveLeft = std::min(3, AT[0] + AT[1] + AT[2]);
	std::vector<std::shared_ptr<Servent>> ToServentCollection;
	for (auto i : OnFieldMyServentCollection) {
		if (i)
			ToServentCollection.push_back(i);
	}
	while(MoveLeft > 0 && AT[0] + AT[1] + AT[2] > 0) {
		if (!ToServentCollection.size())return;
		for (int i = 0; i < 3, MoveLeft>0; i++) {
			if (AT[i]) {
				AT[i]--;
				MoveLeft--;
				int To = RandomGenerator::RandomSelect(ToServentCollection);
				OnFieldEnemyMobCollection[i]->Move(ToServentCollection[To]);
				if (ToServentCollection[To]->GetProperty(ImplementedPropertiesAndBuffName::Health) == 0) {
					ToServentCollection.erase(ToServentCollection._Make_iterator_offset(To));
				}
			}
		}
	}
}
