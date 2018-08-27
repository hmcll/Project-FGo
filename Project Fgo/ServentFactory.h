#pragma once
#include "Servent.h"

class ServentFactory {
public:
	static std::shared_ptr<Servent> CreateServent(std::string name, int NPLevel, int Skill1Level, int Skill2Level, int Skill3Level, int HP, int Atk);
private:
	ServentFactory() {};
	static std::unordered_map<std::string, std::function<std::shared_ptr<Servent>(int, int, int, int, int, int)>> CreateServentFunctionStack;
};