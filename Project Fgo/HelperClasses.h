#pragma once
#include "CommonProperties.h"

class Servent;
class Unit;
struct Card {
	CardColor _color;
	std::shared_ptr<Servent> _servent;
	bool _isCritical;
	bool _isNP;
};
struct HitProperties {
	Card HitCard;
	std::shared_ptr<Unit> Enemy;

	int Pointer;

	bool HasEX;
	bool IsBAQBrave;

	float FirstCardBonus;
	float CardMod;
	float TotalSpecificDamage;
	float TotalSpecificDefence;
	float BusterChainMod;
};
struct Skill
{
	int CoolDown, CoolDownCount;

	void DecreaseCoolDownCount() { if (CoolDownCount != 0) CoolDownCount--; }
	enum{
		TeamMate,
		Enemy,
		NoNeedToChoose
	} ChooseSkillTarget;
	std::string Name;
};
