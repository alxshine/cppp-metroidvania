#include "game/ResourceManager.hpp"

#include <fstream>
#include <iostream>

namespace fs = std::filesystem;
using namespace game;

std::unique_ptr<Mob> ResourceManager::makeMob(const game_definitions::Mob &mobdef)
{
	// walking animation
	const sdl::Texture &walkingAnimationTexture = getTexture(mobdef.walkingAnimation.spritesheet);
	const sdl::Animation walkingAnimation{walkingAnimationTexture, mobdef.walkingAnimation.frames,
	                                      mobdef.walkingAnimation.timePerFrame};

	// idle animation
	OptionalAnimation idleAnimation = [&]() {
		if (mobdef.idleAnimation.spritesheet == "")
			return OptionalAnimation{};
		const sdl::Texture &tex = getTexture(mobdef.idleAnimation.spritesheet);
		return std::make_unique<sdl::Animation>(tex, mobdef.idleAnimation.frames, mobdef.idleAnimation.timePerFrame);
	}();

	// TODO handle health, behaviou, boundingbox, attacks

	return std::make_unique<Mob>(mobdef.name, walkingAnimation, std::move(idleAnimation));
}

std::unique_ptr<Item> ResourceManager::makeItem(const game_definitions::Item &itemdef)
{
	throw "not implemented";
}

std::unique_ptr<Room> ResourceManager::makeRoom(const game_definitions::Room &roomDef)
{
	throw "not implemented";
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

			// rooms.emplace(room.name, makeRoom(room));
		} else if (ext == ".item") {
			std::fstream fin(f, std::ios::in);
			game_definitions::Item item;
			fin >> item;

			// items.emplace(item.name, makeItem(item));
		}
	} catch (game_definitions::ParseException &e) {
		std::cerr << "Invalid definition " << f << ": " << e.what() << "\n";
		exit(1);
	}
}

ResourceManager::ResourceManager(const std::string &path_to_definitions, const std::string &path_to_assets)
    : sdl(sdl::SDL::getInstance())
{

	for (auto &f : fs::recursive_directory_iterator(path_to_assets)) {
		if (!f.is_regular_file())
			continue;

		auto &path = f.path();
		if (path.extension() != ".png")
			continue;

		loadTexture(path.stem(), path);
	}

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
	// TODO return default mob -- should we really, instead of throwing?
	throw "Could not load mob " + name + "\n";
}

Room ResourceManager::getRoom(const std::string &name) const
{
	if (rooms.count(name))
		return *rooms.at(name);
	// TODO return default room -- should we really, instead of throwing?
	throw "Could not load room " + name + "\n";
}

Item ResourceManager::getItem(const std::string &name) const
{
	if (items.count(name))
		return *items.at(name);
	// TODO return default item -- should we really, instead of throwing?
	throw "Could not load item " + name + "\n";
}

const sdl::Texture &ResourceManager::getTexture(const std::string &id)
{
	if (textures.count(id))
		return *textures.at(id);
	//return *textures.at("default"); //TODO: return default texture
	throw "Could not load texture " + id + "\n";
}

void ResourceManager::loadTexture(const std::string &id, const std::string &path)
{
	try {
		textures.emplace(id, sdl.loadTexture(path));
	} catch (SdlException &e) {
		std::cerr << e.what() << std::endl;
	}
}
