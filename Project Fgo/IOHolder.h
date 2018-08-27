#pragma once
#include <iostream>
class IOHolder{
	IOHolder();
public:
	template<typename T, typename ... Other>
	static void Out(const T& out, const Other& ... o) {
		std::cout << out;
		Out(o...);
	}

	template<typename T>
	static void Out(const T& out) {
		std::cout << out;
	}

	template<typename T, typename ... Other>
	static void OutLine(const T& out, const Other& ... o) {
		std::cout << out;
		OutLine(o...);
	}

	static void OutLine() {
		std::cout << std::endl;
	}

	template<typename T>
	static void GetInput(std::string Tag, T& in) {
		std::cout << Tag;
		std::cin >> in;
	}
};