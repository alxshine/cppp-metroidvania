#include <fstream>
#include <iostream>

#include "LoadAllDefinitions.hpp"

int main(int argc, char *argv[])
{
	using namespace game_definitions;
	std::ios_base::sync_with_stdio(false);

	load_all_definitions("game_definitions");

	// std::fstream in(argv[1], std::ios::in);
	// Mob mob;
	// in >> mob;
    //
	// std::cout << mob << std::endl;

	return 0;
}