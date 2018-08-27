#pragma once
#pragma once
#include <string>
#include <unordered_map>
#include <functional>

enum class Class : int {
	Saber = 0,
	Archer,
	Lancer,
	Rider,
	Assassin,
	Caster,
	Berserker,
	Shielder
};

const static std::string ClassName[]{
	"Saber    ",
	"Archer   ",
	"Lancer   ",
	"Rider    ",
	"Assassin ",
	"Caster   ",
	"Berserker",
	"Shielder "
};

const static float ClassAtkBonus[]{
	1.f,
	0.95f,
	1.05f,
	1.f,
	0.9f,
	0.9f,
	1.1f,
	1.f
};

const static float TriangleModifier[8][8]{
	//saber
	{
		1.f,0.5f,2.f,1.f,1.f,1.f,2.f,1.f
	},
	//archer
		{
			2.f,1.f,0.5f,1.f,1.f,1.f,2.f,1.f
		},
	//lancer
		{
			0.5f,2.f,1.f,1.f,1.f,1.f,2.f,1.f
		},
	//Rider
		{
			1.f,1.f,1.f,1.f,0.5f,2.f,2.f,1.f
		},
	//Assassin
		{
			1.f,1.f,1.f,2.f,1.f,0.5f,2.f,1.f
		},
	//Caster
		{
			1.f,1.f,1.f,0.5f,2.f,1.f,2.f,1.f
		},
	//Berserker
		{
			1.5f,1.5f,1.5f,1.5f,1.5f,1.5f,1.5f,1.f
		},
	//Shilder
		{
			1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f
		},
};

enum class Attribute {
	Earth = 0,
	Sky,
	Man,
	Star,
	Beast,
};

const static float AttributeModifier[5][5]{
	//Earth
	{
		1.f,
		0.9f,
		1.1f,
		1.f,
		1.f
	},
	//Sky
	{
		1.1f,
		1.f,
		0.9f,
		1.f,
		1.f
	},
	//Man
	{
		0.9f,
		1.1f,
		1.f,
		1.f,
		1.f
	},
	//Star
	{
		1.f,
		1.f,
		1.f,
		1.f,
		1.1f
	},
	//Beast
	{
		1.f,
		1.f,
		1.f,
		1.1f,
		1.f
	},
};

enum class Trait {
	Beast = 0,
	Demonic,
	Divine,
	Dragon,
	Greek,
	King,
	Riding,
	Roman,
	Artoria,
	Saberface,
	BrynhildrsBeloved,
	Servant,
	Mecha,
	EarthOrSky,
	DemiServants,
	Humanoid,
	WeakToEnumaElish,
	Lawful,
	Balanced,
	Chaotic,
	Good,
	Neutral,
	Evil,
	Female,
	Male
};

const std::string SpecificDamageUpTraitName[] = {
	"BeastSpecificDamageUp",
	"DemonicSpecificDamageUp",
	"DivineSpecificDamageUp",
	"DragonSpecificDamageUp",
	"GreekSpecificDamageUp",
	"KingSpecificDamageUp",
	"RidingSpecificDamageUp",
	"RomanSpecificDamageUp",
	"ArtoriaSpecificDamageUp",
	"SaberfaceSpecificDamageUp",
	"BrynhildrsBelovedSpecificDamageUp",
	"ServantSpecificDamageUp",
	"MechaSpecificDamageUp",
	"EarthOrSkySpecificDamageUp",
	"DemiServantsSpecificDamageUp",
	"HumanoidSpecificDamageUp",
	"WeakToEnumaElishSpecificDamageUp",
	"LawfulSpecificDamageUp",
	"BalancedSpecificDamageUp",
	"ChaoticSpecificDamageUp",
	"GoodSpecificDamageUp",
	"NeutralSpecificDamageUp",
	"EvilSpecificDamageUp",
	"FemaleSpecificDamageUp",
	"MaleSpecificDamageUp"
};

const std::string SpecificDefenceUpTraitName[] = {
	"BeastSpecificDefenceUp",
	"DemonicSpecificDefenceUp",
	"DivineSpecificDefenceUp",
	"DragonSpecificDefenceUp",
	"GreekSpecificDefenceUp",
	"KingSpecificDefenceUp",
	"RidingSpecificDefenceUp",
	"RomanSpecificDefenceUp",
	"ArtoriaSpecificDefenceUp",
	"SaberfaceSpecificDefenceUp",
	"BrynhildrsBelovedSpecificDefenceUp",
	"ServantSpecificDefenceUp",
	"MechaSpecificDefenceUp",
	"EarthOrSkySpecificDefenceUp",
	"DemiServantsSpecificDefenceUp",
	"HumanoidSpecificDefenceUp",
	"WeakToEnumaElishSpecificDefenceUp",
	"LawfulSpecificDefenceUp",
	"BalancedSpecificDefenceUp",
	"ChaoticSpecificDefenceUp",
	"GoodSpecificDefenceUp",
	"NeutralSpecificDefenceUp",
	"EvilSpecificDefenceUp",
	"FemaleSpecificDefenceUp",
	"MaleSpecificDefenceUp"
};

enum class CardColor :int {
	Buster = 0, Art, Quick, EX
};

const static std::string CardColorName[4]{
	"Buster",
	"Art",
	"Quick",
	"EX"
};

const static float CardDamageValue[4][4] = {
	{
		1.5f,1.8f,2.1f,1.f
	},
	{
		1.f,1.2f,1.4f,1.f
	},
	{
		0.8f,0.96f,1.12f,1.f
	},
	{
		1.f,1.f,1.f,1.f
	}
};

const static float CardNPMod[4][4] = {
	{ 0.f,0.f,0.f,1.f },
{ 3.f,4.5f,6.f,1.f },
{ 1.f,1.5f,2.f,1.f },
{ 1.f ,1.f ,1.f ,1.f }
};

const static float CardStarMod[4][4] = {
	{ 0.1f,.15f,0.2f,1.f },
{ 0.f,0.f,0.f,1.f },
{ 0.8f,1.3f,1.8f,1.f },
{ 1.f ,1.f ,1.f ,1.f }
};

struct MobProperties {
	float NPMultiplier, DR, CR, DSR, DTDR, ATDR;
	int CT, AT;
	Class MobClass;
	Attribute MobAttribute;
	std::string Name, NPName;
	std::function <std::pair<float, float>(float)> HealthAndAttackFromLvl;
	MobProperties(float _NPMultiplier, float  _DR, float _CR, float _DSR, float _DTDR, float _ATDR, int _CT, int _AT, std::string _Name, std::string _NPName, Class _MobClass, Attribute _MobAttribute, std::function <std::pair<float, float>(float)> _HealthAndAttackFromLvl) :
		NPMultiplier(_NPMultiplier),
		DR(_DR),
		CR(_CR),
		DSR(_DSR),
		DTDR(_DTDR),
		ATDR(_ATDR),
		CT(_CT),
		AT(_AT),
		Name(_Name),
		NPName(_NPName),
		MobClass(_MobClass),
		MobAttribute(_MobAttribute),
		HealthAndAttackFromLvl(_HealthAndAttackFromLvl) {
	}
};

const static std::unordered_map<int, MobProperties> ImplemetedMobProperties{
	std::make_pair(10001, MobProperties(3.f, 1.f, 10.f, 0.f, 1.2f, 1.2f, 5, 1, "Door of the Undead", "Open Sesame", Class::Caster, Attribute::Sky, [](float lvl) {return std::make_pair(98 * lvl + 392, 40 * lvl + 402); })),
	std::make_pair(10002, MobProperties(3.5f, 1.f, 10.f, 0.f, 1.2f, 1.2f, 5, 1, "Door of the Breave", "Open Sesame", Class::Caster, Attribute::Sky, [](float lvl) {return std::make_pair(196 * lvl + 588, 107.8f * lvl + 274); })),
	std::make_pair(10003, MobProperties(4.f, 0.8f, 10.f, 0.f, 1.2f, 1.2f, 5, 1, "Door of the Saint", "Open Sesame", Class::Caster, Attribute::Sky, [](float lvl) {return std::make_pair(352.8f * lvl + 352, 117.6f * lvl + 459); })),
	std::make_pair(10004, MobProperties(5.f, 0.5f, 10.f, 0.f, 1.2f, 1.2f, 5, 1, "Door of the Champion", "Open Sesame", Class::Caster, Attribute::Sky, [](float lvl) {return std::make_pair(-303.8f * lvl + 35868, 137.2f * lvl + 633); })),
};

namespace ImplementedPropertiesAndBuffName {
	const std::string Attack = "Attack";
	const std::string Health = "Health";
	const std::string MaxHealth = "MaxHealth";
	const std::string NoblePhantasmLevel = "NoblePhantasmLevel";

	const std::string BusterHit = "BusterHit";
	const std::string ArtHit = "ArtHit";
	const std::string QuickHit = "QuickHit";
	const std::string EXHit = "EXHit";
	const std::string BusterNum = "BusterNum";
	const std::string ArtNum = "ArtNum";
	const std::string QuickNum = "QuickNum";

	const std::string BusterResistance = "BusterResistance";
	const std::string ArtResistance = "ArtResistance";
	const std::string QuickResistance = "QuickResistance";
	const std::string BusterPerformanceMod = "BusterPerformanceMod";
	const std::string ArtPerformanceMod = "ArtPerformanceMod";
	const std::string QuickPerformanceMod = "QuickPerformanceMod";

	const std::string MaxNP = "MaxNP";
	const std::string NP = "NP";
	const std::string NPBuff = "NPBuff";
	const std::string NPDefBuff = "NPDefBuff";
	const std::string NPChargeAtk = "NPChargeAtk";
	const std::string NPChargeDef = "NPChargeDef";

	const std::string StarAbsorption = "StarAbsorption";
	const std::string StarGeneration = "StarGeneration";
	const std::string StarGenerationMod = "StarGenerationMod";
	const std::string StarGenerationResistance = "StarGenerationResistance";

	const std::string DeathRate = "DeathRate";
	const std::string DebuffMod = "DebuffMod";
	const std::string DebuffSuccessRate = "DebuffSuccessRate";

	const std::string PercentDefenceMod = "PercentDefenceMod";
	const std::string ConstDefenceMod = "ConstDefenceMod";
	const std::string NoblePhantasmDamageMod = "NoblePhantasmDamageMod";
	const std::string PercentAttackMod = "PercentAttackMod";
	const std::string ConstAttackMod = "ConstAttackMod";
	const std::string CriticalDamageMod = "CriticalDamageMod";
	const std::string CriticalChanceMod = "CriticalChanceMod";
	const std::string HealthRecoverMod = "HealthRecoverMod";
	const std::string TargetConcentrationMod = "TargetConcentrationMod";

	const std::string Invincibility = "Invincibility";
	const std::string DebuffImmunity = "DebuffImmunity";
	const std::string Guts = "Guts";
	const std::string IgnoreEvasion = "IgnoreEvasion";
	const std::string IgnoreInvincibility = "IgnoreInvincibility";
	const std::string Evasion = "Evasion";

	const std::string CR = "CR";
	const std::string DSR = "DSR";
	const std::string DTDR = "DTDR";
	const std::string ATDR = "ATDR";
	const std::string NPMultiplier = "NPMultiplier";
}