#include "game/ResourceManager.hpp"
#include "gamedef/DefinitionLoading.hpp"

#include <fstream>
#include <iostream>

namespace fs = std::filesystem;
using namespace game;

void parse_definition(fs::path f, ResourceManager &res)
{
	std::string ext(f.extension());

	try {
		if (ext == ".mob") {
			std::fstream fin(f, std::ios::in);
			game_definitions::Mob mob;
			fin >> mob;
			res.mobs.insert({mob.name, Mob{mob, res}});

			// TODO we might want to parse all mobs before the rooms...
		} else if (ext == ".room") {
			std::fstream fin(f, std::ios::in);
			game_definitions::Room room;
			fin >> room;
			// std::cout << room; // TODO actually handle the room
		} else if (ext == ".item") {
			std::fstream fin(f, std::ios::in);
			game_definitions::Item item;
			fin >> item;
			// std::cout << item; // TODO actually handle the item
		}
	} catch (game_definitions::ParseException &e) {
		std::cerr << "Invalid definition " << f << ": " << e.what() << "\n";
		exit(1);
	}
}

ResourceManager::ResourceManager(const std::string &path_to_definitions)
{
	for (auto &f : fs::recursive_directory_iterator(path_to_definitions)) {
		if (!f.is_regular_file())
			continue;
		parse_definition(f, *this);
	}
}

/*************
 *  Getters  *
 *************/

Mob ResourceManager::getMob(const std::string &name) const
{
	if (mobs.contains(name))
		return mobs.at(name);
	// TODO return default mob
}

Room ResourceManager::getRoom(const std::string &name) const
{
	if (rooms.contains(name))
		return rooms.at(name);
	// TODO return default room
}

Item ResourceManager::getItem(const std::string &name) const
{
	if (items.contains(name))
		return items.at(name);
	// TODO return default item
}

const Sdl::Texture &ResourceManager::getTexture(const std::string &id) const
{
	if (textures.contains(id))
		return textures.at(id);
	// TODO return default texture
}
