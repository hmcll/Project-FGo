#include "GameInstance.h"
#include "Throwables.h"
#include "IOHolder.h"
#include "CommonProperties.h"
#include "ServentFactory.h"
#include "HandFactory.h"
#include "RandomGenerator.h"
#include "Skill.h"
#include "DataProcessor.h"
#include "AIHandler.h"

#include <deque>
#include <set>
#include <array>

std::shared_ptr<GameInstance> GameInstance::Instance = NULL;

void GameInstance::Initalize()
{
	Instance = std::make_shared<GameInstance>();
}

void GameInstance::ShutDown()
{
	Instance.reset();
}

std::shared_ptr<GameInstance> GameInstance::Get()
{
	if (!Instance)
		Initalize();
	return Instance;
}

void GameInstance::StartGame()
{
	LoadMap();
	while (LoadBattle())
	{
		try {
			for (;;) {
				TurnNumber++;

				int ServentNum = 0;
				for (auto i : OnFieldMyServentCollection) if (i)ServentNum++;
				if (!CardStack.size()) {
					for (int i = 0; i < 5 * ServentNum; i++) {
						CardStack.push_back(i);
					}
					RandomGenerator::RandomShuffle(CardStack);
				}
				StageStartRound();
				Refresh();

				StageMyRoundSkill();
				Refresh();

				StageMyRoundCard();
				Refresh();

				StageMyRoundEnd();
				StageEnemyRoundMove();
				StageEnemyRoundEnd();
			}
		}
		catch (EndOfRound)
		{
			continue;
		}
		catch (GameFailed)
		{
			IOHolder::Out("GameFailed");
			system("timeout 3 >nul");
			return;
		}
	}
	IOHolder::Out("Battle Finished");
	system("timeout 3 >nul");
}

void GameInstance::LoadMap()
{
	BattleNumberTotal = 3;
	TurnNumber = 0;
	BattleNumber = 0;
	auto Mashu = ServentFactory::CreateServent("Mashu", 3, 10, 10, 10, 9730, 12877);

	ReplacementMyServentCollection.push_back(Mashu);
	ReplacementMyServentCollection.push_back(ServentFactory::CreateServent("Jack", 3, 10, 10, 10, 9730, 12877));
}

bool GameInstance::LoadBattle()
{
	BattleNumber++;
	if (BattleNumber > BattleNumberTotal) return false;
	for (int i = 0; i < 3; i++) {
		ReplacementEnemyMobCollection.push_back(HandFactory::MakeHand(10001, 10));
	}
	for (int i = 0; i < 3; i++) {
		if (ReplacementMyServentCollection.empty())break;
		OnFieldMyServentCollection[i] = ReplacementMyServentCollection.front();
		OnFieldMyServentCollection[i]->OnEnter();
		ReplacementMyServentCollection.pop_front();
	}
	for (int i = 0; i < 3; i++) {
		if (ReplacementEnemyMobCollection.empty())break;
		OnFieldEnemyMobCollection[i] = ReplacementEnemyMobCollection.front();
		OnFieldEnemyMobCollection[i]->OnEnter();
		ReplacementEnemyMobCollection.pop_front();
	}
	return true;
}

void GameInstance::StageStartRound()
{
	for (auto i : OnFieldMyServentCollection) {
		if (!i)break;
		i->OnRoundStart();
		i->Skill1.DecreaseCoolDownCount();
		i->Skill2.DecreaseCoolDownCount();
		i->Skill3.DecreaseCoolDownCount();
	}
}

void GameInstance::StageMyRoundSkill()
{
	for (;;) {
		ShowCards();
		std::string a;
		IOHolder::GetInput("是否使用技能(y/n)", a);
		if (!(a == "y"))
			return;
		int ServentNum, SkillNum;
		IOHolder::GetInput("请输入从者序号:", ServentNum);
		IOHolder::GetInput("请输入技能序号:", SkillNum);
		std::shared_ptr<Servent> ServentChosen = OnFieldMyServentCollection[ServentNum];
		int ServentTargetNum;
		switch (SkillNum)
		{
		case 1:
			switch (ServentChosen->Skill1.ChooseSkillTarget)
			{
			case Skill::TeamMate:
				IOHolder::GetInput("请输入己方从者序号:", ServentTargetNum);
				ServentChosen->UseSkill1(OnFieldMyServentCollection[ServentTargetNum]);
				break;
			case Skill::Enemy:
				IOHolder::GetInput("请输入敌方从者序号:", ServentTargetNum);
				ServentChosen->UseSkill1(OnFieldEnemyMobCollection[ServentTargetNum]);
				break;
			case Skill::NoNeedToChoose:
				ServentChosen->UseSkill1(nullptr);
				break;
			default:
				break;
			}
			break;
		case 2:
			switch (ServentChosen->Skill2.ChooseSkillTarget)
			{
			case Skill::TeamMate:
				IOHolder::GetInput("请输入己方从者序号:", ServentTargetNum);
				ServentChosen->UseSkill2(OnFieldMyServentCollection[ServentTargetNum]);
				break;
			case Skill::Enemy:
				IOHolder::GetInput("请输入敌方从者序号:", ServentTargetNum);
				ServentChosen->UseSkill2(OnFieldEnemyMobCollection[ServentTargetNum]);
				break;
			case Skill::NoNeedToChoose:
				ServentChosen->UseSkill2(nullptr);
				break;
			default:
				break;
			}
			break;
		case 3:
			switch (ServentChosen->Skill3.ChooseSkillTarget)
			{
			case Skill::TeamMate:
				IOHolder::GetInput("请输入己方从者序号:", ServentTargetNum);
				ServentChosen->UseSkill3(OnFieldMyServentCollection[ServentTargetNum]);
				break;
			case Skill::Enemy:
				IOHolder::GetInput("请输入敌方从者序号:", ServentTargetNum);
				ServentChosen->UseSkill3(OnFieldEnemyMobCollection[ServentTargetNum]);
				break;
			case Skill::NoNeedToChoose:
				ServentChosen->UseSkill3(nullptr);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		Refresh();
	}
}

void GameInstance::StageMyRoundCard()
{
	Refresh();
	std::array<int, 5>CriticalStarDistribution;

	DataProcessor::DistributeStar(CriticalStarDistribution);

	ShowCards(CriticalStarDistribution);

	std::vector<Card> CardSequence;
	std::set<std::shared_ptr<Servent>> ServentSequence;
	LoadCardSequence(CardSequence, ServentSequence, CriticalStarDistribution);

	int EnemyID;
	IOHolder::GetInput("请输入锁定对象", EnemyID);

	auto TempServent = CardSequence.at(0)._servent;
	bool hasEX = true;
	std::array<bool, 4> ChangeAttackServent;
	ChangeAttackServent.fill(false);
	for (int i = 0; i < 3; i++) {
		if (CardSequence[i]._servent != TempServent) {
			ChangeAttackServent[i - 1] = true;
			TempServent = CardSequence[i]._servent;
			hasEX = false;
			ChangeAttackServent[i] = true;
		}
		if (CardSequence[i]._isNP) {
			ChangeAttackServent[i] = true;
		}
	}
	ChangeAttackServent[3] = true;
	float busterChainMod = 0.f;
	float firstCardBonus = CardSequence[0]._color == CardColor::Buster ? 0.5f : 0.f;
	bool isBAQBrave =
		(CardSequence[0]._color == CardColor::Buster && CardSequence[1]._color == CardColor::Art && CardSequence[2]._color == CardColor::Quick) ||
		(CardSequence[0]._color == CardColor::Buster&& CardSequence[1]._color == CardColor::Quick) && CardSequence[2]._color == CardColor::Art ||
		(CardSequence[0]._color == CardColor::Art && CardSequence[1]._color == CardColor::Quick && CardSequence[2]._color == CardColor::Buster) ||
		(CardSequence[0]._color == CardColor::Art && CardSequence[1]._color == CardColor::Buster) && CardSequence[2]._color == CardColor::Quick ||
		(CardSequence[0]._color == CardColor::Quick && CardSequence[1]._color == CardColor::Buster && CardSequence[2]._color == CardColor::Art) ||
		(CardSequence[0]._color == CardColor::Quick && CardSequence[1]._color == CardColor::Art) && CardSequence[2]._color == CardColor::Buster;
	isBAQBrave = hasEX ? isBAQBrave : false;
	CardColor FirstCardColor = CardSequence[0]._color;
	if (FirstCardColor == CardSequence[1]._color&& FirstCardColor == CardSequence[2]._color) {
		switch (FirstCardColor)
		{
		case CardColor::Buster:
			busterChainMod = 0.2f;
			break;
		case CardColor::Art:
			for (auto& i : ServentSequence)if(i)i->ChargeNP(0.2f);
			break;
		case CardColor::Quick:
			Star += 10;
			break;
		case CardColor::EX:
			break;
		default:
			break;
		}
	}
	if (hasEX) {
		Card ExCard;
		ExCard._color = CardColor::EX;
		ExCard._isNP = false;
		ExCard._isCritical = false;
		ExCard._servent = TempServent;
		CardSequence.push_back(ExCard);
	}

	int pointer = 0;

	for (auto ThisCard : CardSequence) {
		if (!ThisCard._servent) continue;
		float cardMod = 0, TotalSpecificDamage = 0, TotalSpecificDefence = 0;
		if (ThisCard._isNP) {
			ThisCard._servent->CallNoblePhantasm(OnFieldEnemyMobCollection[EnemyID]);
			ThisCard._servent->GetProperty(ImplementedPropertiesAndBuffName::NP) = 0;
			try {
				CardEnd(EnemyID);
			}
			catch (std::out_of_range) {
				break;
			}
		}
		else {
			DataProcessor::Card_Prepare(OnFieldEnemyMobCollection[EnemyID], ThisCard, cardMod, TotalSpecificDamage, TotalSpecificDefence);
			HitProperties hit;
			hit.HitCard = ThisCard;
			hit.FirstCardBonus = firstCardBonus;
			hit.Pointer = pointer;
			hit.CardMod = cardMod;
			hit.Enemy = OnFieldEnemyMobCollection[EnemyID];
			hit.HasEX = hasEX;
			hit.IsBAQBrave = isBAQBrave;
			hit.TotalSpecificDamage = TotalSpecificDamage;
			hit.TotalSpecificDefence = TotalSpecificDefence;
			hit.BusterChainMod = busterChainMod;

			const float TotalDamage = ThisCard._servent->CommonCard_Calculation(hit);

			float TempDamage = TotalDamage;
			std::vector<int> DamageStack;
			auto CardDamageMul =
				ThisCard._color == CardColor::Art ? ThisCard._servent->GetArtHitDamageMul() :
				ThisCard._color == CardColor::Buster ? ThisCard._servent->GetBusterHitDamageMul() :
				ThisCard._color == CardColor::Quick ? ThisCard._servent->GetQuickHitDamageMul() :
				ThisCard._servent->GetExHitDamageMul();

			for (float DamageMul : CardDamageMul) {
				DamageStack.push_back(DamageMul * TotalDamage);
				TempDamage -= DamageMul * TotalDamage;
			}
			DamageStack.push_back(int(TempDamage));

			DataProcessor::Total_Calculation(DamageStack, pointer, ThisCard, OnFieldEnemyMobCollection[EnemyID], FirstCardColor);

			OnFieldEnemyMobCollection[EnemyID]->ChangeHealth(-1 * TotalDamage);

			IOHolder::OutLine(ThisCard._servent->Name, "对", OnFieldEnemyMobCollection[EnemyID]->Name, "造成了", TotalDamage, "点伤害");
			system("timeout 3 >nul");
			if (ChangeAttackServent[pointer]) {
				try
				{
					CardEnd(EnemyID);
				}
				catch (const std::out_of_range&)
				{
					break;
				}
			}
		}
		pointer++;
	}
}

void GameInstance::StageMyRoundEnd()
{
	for (int i = 0; i < 5; i++)CardStack.pop_front();
	for (auto&i : OnFieldMyServentCollection) {
		if (i)i->OnRoundEnd();
	}
	for (auto&i : OnFieldEnemyMobCollection) {
		if (i)i->OnRoundStart();
	}
}

void GameInstance::StageEnemyRoundMove()
{
	AIHandler::Move();
}

void GameInstance::StageEnemyRoundEnd()
{
	for (auto& i : OnFieldMyServentCollection) {
		if (!i&&ReplacementMyServentCollection.size()) {
			i = ReplacementMyServentCollection[0];
			i->OnEnter();
			ReplacementMyServentCollection.pop_front();
		}
	}
	int num = 0;
	for (auto i : OnFieldMyServentCollection)if (i)num++;
	if (!num)throw GameFailed();
	for (auto&i : OnFieldEnemyMobCollection) {
		if (!i&&ReplacementEnemyMobCollection.size()) {
			i = ReplacementEnemyMobCollection[0];
			i->OnEnter();
			ReplacementEnemyMobCollection.pop_front();
		}
	}
	num = 0;
	for (auto i : OnFieldEnemyMobCollection)if (i)num++;
	if (!num)throw EndOfRound();

	for (auto&i : OnFieldEnemyMobCollection)if (i)i->OnRoundEnd();
}

void GameInstance::Refresh()
{
	system("cls");
	IOHolder::OutLine("************************************************************");
	IOHolder::OutLine("*Battle:", BattleNumber, "/", BattleNumberTotal, "\t\tEnemy:", ReplacementEnemyMobCollection.size() + OnFieldEnemyMobCollection.size(), "\t\tTurn:", TurnNumber, "回合\t\t星星:", Star);
	IOHolder::OutLine("*\t\t\t\t\t\t\t\t\t\t\t\t\t*");
	IOHolder::Out("*");
	for (auto i : OnFieldEnemyMobCollection) {
		if (!i)IOHolder::Out("\t\t");
		else IOHolder::Out(i->Name, "");
	}
	IOHolder::Out("\t");

	for (auto i : OnFieldMyServentCollection) {
		if (!i) IOHolder::Out("\t\t");
		else IOHolder::Out(i->Name, "\t\t");
	}

	IOHolder::OutLine("*");

	IOHolder::Out("*");

	for (auto i : OnFieldEnemyMobCollection) {
		if (!i) IOHolder::Out("\t\t");
		else IOHolder::Out(ClassName[(int)i->UnitClass], "\t");
	}
	IOHolder::Out("\t");

	for (auto i : OnFieldMyServentCollection) {
		if (!i) IOHolder::Out("\t\t");
		else IOHolder::Out(ClassName[(int)i->UnitClass], "\t");
	}

	IOHolder::OutLine("*");

	IOHolder::Out("*");

	for (auto i : OnFieldEnemyMobCollection) {
		if (!i) IOHolder::Out("\t\t");
		else IOHolder::Out(i->GetProperty(ImplementedPropertiesAndBuffName::Health), "\t\t");
	}
	IOHolder::Out("\t");

	for (auto i : OnFieldMyServentCollection) {
		if (!i) IOHolder::Out("\t\t");
		else IOHolder::Out(i->GetProperty(ImplementedPropertiesAndBuffName::Health), "\t\t");
	}

	IOHolder::OutLine("*");

	IOHolder::Out("*");

	for (auto i : OnFieldEnemyMobCollection) {
		if (!i) IOHolder::Out("\t\t");
		else IOHolder::Out(i->ct, "\t\t");
	}
	IOHolder::Out("\t");

	for (auto i : OnFieldMyServentCollection) {
		if (!i) IOHolder::Out("\t\t");
		else IOHolder::Out(int(i->GetProperty(ImplementedPropertiesAndBuffName::NP) * 100), "\t\t");
	}

	IOHolder::OutLine("*");
	IOHolder::Out("*");

	IOHolder::Out("\t\t\t\t\t\t\t");

	for (auto i : OnFieldMyServentCollection) {
		if (!i) IOHolder::Out("\t\t");
		else IOHolder::Out("Skill1 : ", i->Skill1.CoolDownCount, "\t");
	}

	IOHolder::OutLine("*");
	IOHolder::Out("*");

	IOHolder::Out("\t\t\t\t\t\t\t");

	for (auto i : OnFieldMyServentCollection) {
		if (!i) IOHolder::Out("\t\t");
		else IOHolder::Out("Skill2 : ", i->Skill2.CoolDownCount, "\t");
	}

	IOHolder::OutLine("*");
	IOHolder::Out("*");

	IOHolder::Out("\t\t\t\t\t\t\t");

	for (auto i : OnFieldMyServentCollection) {
		if (!i) IOHolder::Out("\t\t");
		else IOHolder::Out("Skill3 : ", i->Skill3.CoolDownCount, "\t");
	}

	IOHolder::OutLine("*");
	IOHolder::Out("*");

	for (auto i : OnFieldEnemyMobCollection) {
		if (!i)continue;
		IOHolder::Out(i->Name, ": ");
		for (auto j : i->UniqueBuffs) {
			if (j.second.use_count())
				if (j.second->IsBuff) {
					IOHolder::Out(j.first, ":");
					if (std::static_pointer_cast<Buff>(j.second)->NumCount > 0) {
						IOHolder::Out(std::static_pointer_cast<Buff>(j.second)->NumCount, " 次");
					}
					if (std::static_pointer_cast<Buff>(j.second)->RoundCount > 0) {
						IOHolder::Out(std::static_pointer_cast<Buff>(j.second)->RoundCount, " 回合");
					}
					IOHolder::Out("\t");
				}
				else
					IOHolder::Out(j.first, "\t");
		}
		for (auto j : i->RegularBuffs) {
			if (j.second.size())
				for (auto s : j.second) {
					if (s->IsBuff) {
						IOHolder::Out(j.first, ":");
						if (std::static_pointer_cast<Buff>(s)->NumCount > 0) {
							IOHolder::Out(std::static_pointer_cast<Buff>(s)->NumCount, " 次");
						}
						if (std::static_pointer_cast<Buff>(s)->RoundCount > 0) {
							IOHolder::Out(std::static_pointer_cast<Buff>(s)->RoundCount, " 回合");
						}
						IOHolder::Out("\t");
					}
				}
			else
				IOHolder::Out(j.first, "\t");
		}
		IOHolder::OutLine();
		IOHolder::Out("*");
	}
	for (auto i : OnFieldMyServentCollection) {
		if (!i)continue;
		IOHolder::Out(i->Name, ": ");
		for (auto j : i->UniqueBuffs) {
			if (j.second.use_count())
				if (j.second->IsBuff) {
					IOHolder::Out(j.first, ":");
					if (std::static_pointer_cast<Buff>(j.second)->NumCount > 0) {
						IOHolder::Out(std::static_pointer_cast<Buff>(j.second)->NumCount, " 次");
					}
					if (std::static_pointer_cast<Buff>(j.second)->RoundCount > 0) {
						IOHolder::Out(std::static_pointer_cast<Buff>(j.second)->RoundCount, " 回合");
					}
					IOHolder::Out("\t");
				}
				else
					IOHolder::Out(j.first, "\t");
		}
		for (auto j : i->RegularBuffs) {
			if (j.second.size())
				for (auto s : j.second) {
					if (s->IsBuff) {
						IOHolder::Out(j.first, ":");
						if (std::static_pointer_cast<Buff>(s)->NumCount > 0) {
							IOHolder::Out(std::static_pointer_cast<Buff>(s)->NumCount, " 次");
						}
						if (std::static_pointer_cast<Buff>(s)->RoundCount > 0) {
							IOHolder::Out(std::static_pointer_cast<Buff>(s)->RoundCount, " 回合");
						}
						IOHolder::Out("\t");
					}
				}
		}
		IOHolder::OutLine();
		IOHolder::Out("*");
	}
}

void GameInstance::ShowCards()
{
	IOHolder::OutLine("指令卡:");
	int i = 0;
	std::vector<std::shared_ptr<Servent>>ServentStack;
	for (auto i : OnFieldMyServentCollection)if (i)ServentStack.push_back(i);
	for (; i < 5; i++) {
		int ServentID = CardStack[i] / 5;
		int CardID = CardStack[i] % 5;
		IOHolder::OutLine(i + 1, ".", ServentStack[ServentID]->GetCard(CardID));
	}
	ReadyNoblePhantasm.clear();
	for (auto Servent : OnFieldMyServentCollection) {
		if (!Servent)continue;
		if (Servent->GetProperty(ImplementedPropertiesAndBuffName::NP) >= 1) {
			IOHolder::OutLine(i + 1, ".", Servent->GetCard(6));
			ReadyNoblePhantasm.push_back(Servent);
		}
	}
}

void GameInstance::ShowCards(std::array<int, 5> CriticalStarDistribution)
{
	IOHolder::OutLine("指令卡:");
	int i = 0;
	std::vector<std::shared_ptr<Servent>>ServentStack;
	for (auto i : OnFieldMyServentCollection)if (i)ServentStack.push_back(i);
	for (; i < 5; i++) {
		int ServentID = CardStack[i] / 5;
		int CardID = CardStack[i] % 5;
		IOHolder::OutLine(i + 1, ".", ServentStack[ServentID]->GetCard(CardID), "\t", CriticalStarDistribution[i], "0%");
	}
	ReadyNoblePhantasm.clear();
	for (auto Servent : OnFieldMyServentCollection) {
		if (!Servent)continue;
		if (Servent->GetProperty(ImplementedPropertiesAndBuffName::NP) >= 1) {
			IOHolder::OutLine(i + 1, ".", Servent->GetCard(6));
			ReadyNoblePhantasm.push_back(Servent);
		}
	}
}

void GameInstance::LoadCardSequence(std::vector<Card>& CardSequence, std::set<std::shared_ptr<Servent>>& ServentSequence, const std::array<int, 5>& CriticalStarDistribution)
{
	int CardSequence_Input;
	IOHolder::GetInput("请输入指令卡顺序", CardSequence_Input);

	std::vector<std::shared_ptr<Servent>>ServentStack;
	for (auto i : OnFieldMyServentCollection)
		if (i)
			ServentStack.push_back(i);


	for (int i = 0; i < 3; i++) {
		int CurrentCard = CardSequence_Input % 10 - 1;
		Card InputCard;
		int ServentID = CardStack[CurrentCard] / 5;
		int CardID = CardStack[CurrentCard] % 5;
		if (CurrentCard < 5) {
			std::shared_ptr<Servent> CardServent = ServentStack[ServentID];
			InputCard._color = CardServent->GetCardColor(CardID);
			InputCard._isNP = false;
			InputCard._isCritical = RandomGenerator::CriticalDistributor(CriticalStarDistribution[CardID] * 10);
			ServentSequence.insert(CardServent);
			InputCard._servent = CardServent;
		}
		else {
			ServentSequence.insert(ReadyNoblePhantasm[CurrentCard - 5]);
			InputCard._color = ReadyNoblePhantasm[CurrentCard - 5]->NoblePhantasmColor;
			InputCard._isNP = true;
			InputCard._isCritical = false;
			InputCard._servent = ReadyNoblePhantasm[CurrentCard - 5];
		}
		CardSequence.push_back(InputCard);
		CardSequence_Input /= 10;
	}
	std::iter_swap(CardSequence.begin(), CardSequence.end() - 1);
}

void GameInstance::CardEnd(int & EnemyID)
{
	if (OnFieldEnemyMobCollection[EnemyID]->GetProperty(ImplementedPropertiesAndBuffName::Health) == 0) {
		OnFieldEnemyMobCollection[EnemyID]->OnDeath();
		OnFieldEnemyMobCollection[EnemyID].reset();
		EnemyID = 2;
		while (!OnFieldEnemyMobCollection.at(EnemyID)) {
			EnemyID--;
		}
	}
}
