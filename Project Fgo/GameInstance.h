#pragma once
#include <memory>
#include <array>
#include <vector>
#include <deque>
#include <set>

class EnemyMob;
class Servent;
struct Card;

class GameInstance {
public:
	static void Initalize();
	static void ShutDown();
	static std::shared_ptr<GameInstance> Instance;
	static std::shared_ptr<GameInstance> Get();

	void StartGame();

private:
	void LoadMap();
	bool LoadBattle();
	void StageStartRound();
	void StageMyRoundSkill();
	void StageMyRoundCard();
	void StageMyRoundEnd();
	void StageEnemyRoundMove();
	void StageEnemyRoundEnd();
	void Refresh();

	int TurnNumber = 0, BattleNumber = 0, BattleNumberTotal;

	void ShowCards();

	void ShowCards(std::array<int, 5> CriticalStarDistribution);

	void LoadCardSequence(std::vector<Card>& CardSequence, std::set<std::shared_ptr<Servent>>&ServentSequence, const std::array<int, 5>& CriticalStarDistribution);

	std::vector <std::shared_ptr<Servent>> ReadyNoblePhantasm;

	void CardEnd(int& EnemyID);

public:
	std::array<std::shared_ptr<EnemyMob>, 3> OnFieldEnemyMobCollection;
	std::array<std::shared_ptr<Servent>, 3> OnFieldMyServentCollection;

	std::deque<std::shared_ptr<EnemyMob>> ReplacementEnemyMobCollection;
	std::deque<std::shared_ptr<Servent>> ReplacementMyServentCollection;

	std::deque<int> CardStack;
	int Star = 0;
};