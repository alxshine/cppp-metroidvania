#include "game/ResourceManager.hpp"

#include <fstream>
#include <iostream>

namespace fs = std::filesystem;
using namespace game;

template <typename T>
bool game::contained(std::vector<T> vec, T x)
{
	return std::find(vec.begin(), vec.end(), x) != vec.end();
}

std::vector<std::string> ResourceManager::imageFormats{".png"};
// TODO technically, sounds could also be mp3 etc...
std::vector<std::string> ResourceManager::soundFormats{".wav"};
std::vector<std::string> ResourceManager::musicFormats{".mp3", ".ogg", ".flac"};

std::unique_ptr<Mob> ResourceManager::makeMob(const game_definitions::Mob &mobdef) const
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

std::unique_ptr<Item> ResourceManager::makeItem(const game_definitions::Item &itemdef) const
{
	throw itemdef.name + " not implemented";
}

std::unique_ptr<Room> ResourceManager::makeRoom(const game_definitions::Room &roomDef) const
{
	Room::Layout layout;
	for (auto &layer : roomDef.layout) {
		Room::Layer newLayer;
		for (auto &row : layer) {
			Room::Row newRow;
			for (auto &tile : row) {
				sdl::Sprite sprite{getTexture(roomDef.tileset), tile.rectangle};
				Room::Tile newTile{sprite};
				newRow.emplace_back(newTile);
			}
			newLayer.push_back(newRow);
		}
		layout.push_back(newLayer);
	}

	return std::make_unique<game::Room>(roomDef.name, getTexture(roomDef.background), getMusic(roomDef.music),
	                                    roomDef.location, std::move(layout));
}

void ResourceManager::parseDefinition(fs::path f)
{
	std::string ext(f.extension());
	std::cout << "Parsing " << f << std::endl;

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
		if (contained(imageFormats, static_cast<std::string>(path.extension())))
			loadTexture(path.filename(), path);

		if (contained(soundFormats, static_cast<std::string>(path.extension())))
			loadSound(path.filename(), path);

		if (contained(musicFormats, static_cast<std::string>(path.extension())))
			loadMusic(path.filename(), path);
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
	throw ResourceNotFoundException("Could not load mob " + name + "\n");
}

Room ResourceManager::getRoom(const std::string &name) const
{
	if (rooms.count(name))
		return *rooms.at(name);
	// TODO return default room -- should we really, instead of throwing?
	throw ResourceNotFoundException("Could not load room " + name + "\n");
}

Item ResourceManager::getItem(const std::string &name) const
{
	if (items.count(name))
		return *items.at(name);
	// TODO return default item -- should we really, instead of throwing?
	throw ResourceNotFoundException("Could not load item " + name + "\n");
}

const sdl::Texture &ResourceManager::getTexture(const std::string &id) const
{
	if (textures.count(id))
		return *textures.at(id);
	// return *textures.at("default"); //TODO: return default texture
	throw "Could not load texture " + id + "\n";
}

const sdl::SoundEffect &ResourceManager::getSound(const std::string &id) const
{
	if (sounds.count(id))
		return *sounds.at(id);
	throw "Could not load sound " + id + "\n";
}

const sdl::Music &ResourceManager::getMusic(const std::string &id) const
{
	if (music.count(id))
		return *music.at(id);
	throw "Could not load music " + id + "\n";
}

void ResourceManager::loadTexture(const std::string &id, const std::string &path)
{
	try {
		textures.emplace(id, sdl.loadTexture(path));
	} catch (SdlException &e) {
		std::cerr << e.what() << std::endl;
	}
}

void ResourceManager::loadSound(const std::string &id, const std::string &path)
{
	try {
		sounds.emplace(id, sdl.loadSound(path));
	} catch (SdlException &e) {
		std::cerr << e.what() << std::endl;
	}
}

void ResourceManager::loadMusic(const std::string &id, const std::string &path)
{
	try {
		music.emplace(id, sdl.loadMusic(path));
	} catch (SdlException &e) {
		std::cerr << e.what() << std::endl;
	}
}
