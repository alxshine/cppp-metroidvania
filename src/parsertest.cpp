#include "DefinitionLoading.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
	using namespace game_definitions;

	std::fstream in(argv[1], std::ios::in);
	Mob mob;
	in >> mob;

	std::cout << mob << std::endl;

	return 0;
}
