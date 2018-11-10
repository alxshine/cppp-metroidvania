#include "game/ResourceManager.hpp"

#include <fstream>
#include <iostream>

namespace fs = std::filesystem;
using namespace game;

std::unique_ptr<Mob> ResourceManager::makeMob(const game_definitions::Mob &mobdef)
{
	const sdl::Texture &walkingAnimationTexture = getOrLoadTexture(mobdef.walkingAnimation.spritesheet);
}

void ResourceManager::parseDefinition(fs::path f)
{
	std::string ext(f.extension());

	try {
		if (ext == ".mob") {
			std::fstream fin(f, std::ios::in);
			game_definitions::Mob mob;
			fin >> mob;

			mobs.emplace(mob.name, makeMob(mob));
			// TODO we might want to parse all mobs before the rooms...
		} else if (ext == ".room") {
			std::fstream fin(f, std::ios::in);
			game_definitions::Room room;
			fin >> room;

			rooms.emplace(room.name, makeRoom(room));
		} else if (ext == ".item") {
			std::fstream fin(f, std::ios::in);
			game_definitions::Item item;
			fin >> item;

			items.emplace(item.name, makeItem(item));
		}
	} catch (game_definitions::ParseException &e) {
		std::cerr << "Invalid definition " << f << ": " << e.what() << "\n";
		exit(1);
	}
}

ResourceManager::ResourceManager(const std::string &path_to_definitions, const sdl::SDL &sdl) : sdl{sdl}
{
	for (auto &f : fs::recursive_directory_iterator(path_to_definitions)) {
		if (!f.is_regular_file())
			continue;
		parseDefinition(f);
	}
}

/*************
 *  Getters  *
 *************/

Mob ResourceManager::getMob(const std::string &name) const
{
	if (mobs.count(name))
		return *mobs.at(name);
	// TODO return default mob
}

Room ResourceManager::getRoom(const std::string &name) const
{
	if (rooms.count(name))
		return *rooms.at(name);
	// TODO return default room
}

Item ResourceManager::getItem(const std::string &name) const
{
	if (items.count(name))
		return *items.at(name);
	// TODO return default item
}

const sdl::Texture &ResourceManager::getOrLoadTexture(const std::string &id)
{
	if (textures.count(id))
		return *textures.at(id);
	// TODO try to load texture
	// TODO return default texture if loading fails
}
