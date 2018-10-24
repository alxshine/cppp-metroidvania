#include "gamedef/LoadAllDefinitions.hpp"
#include "gamedef/DefinitionLoading.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

void handle_file(fs::path f)
{
	std::string ext(f.extension());

	try {
		if (ext == ".mob") {
			std::fstream fin(f, std::ios::in);
			game_definitions::Mob mob;
			fin >> mob;
			std::cout << mob; // TODO actually handle the mob
		} else if (ext == ".room") {
			std::fstream fin(f, std::ios::in);
			game_definitions::Room room;
			fin >> room;
			// std::cout << room; // TODO actually handle the room
		}
	} catch (game_definitions::ParseException &e) {
		std::cerr << "Invalid definition " << f << ": " << e.what() << "\n";
		exit(1);
	}
}

void game_definitions::load_all_definitions(std::string base)
{
	for (auto &f : fs::recursive_directory_iterator(base)) {
		if (!f.is_regular_file())
			continue;
		handle_file(f);
	}
}
