#include "ServentFactory.h"
#include "ImplementedServents.h"

std::unordered_map<std::string, std::function<std::shared_ptr<Servent>(int, int, int, int, int, int)>> InitalizeCreateServentFunctionStack() {
	std::unordered_map<std::string, std::function<std::shared_ptr<Servent>(int, int, int, int, int, int)>> ret;
	ret.insert(std::make_pair("Mashu", ImplementedServents::Mashu::CreateServent));
	ret.insert(std::make_pair("Jack", ImplementedServents::Jack::CreateServent));
	return ret;
}

std::unordered_map<std::string, std::function<std::shared_ptr<Servent>(int, int, int, int, int, int)>> ServentFactory::CreateServentFunctionStack = InitalizeCreateServentFunctionStack();

std::shared_ptr<Servent> ServentFactory::CreateServent(std::string name, int NPLevel, int Skill1Level, int Skill2Level, int Skill3Level, int HP, int Atk)
{
	if (CreateServentFunctionStack.count(name)) {
		return CreateServentFunctionStack[name](NPLevel, Skill1Level, Skill2Level, Skill3Level, HP, Atk);
	}
	return nullptr;
}
