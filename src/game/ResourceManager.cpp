#include "game/ResourceManager.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;
using namespace game;
using namespace std::chrono_literals;

template <typename T>
bool game::contained(std::vector<T> vec, T x)
{
	return std::find(vec.begin(), vec.end(), x) != vec.end();
}

std::unique_ptr<Player> ResourceManager::makePlayer() const
{
	const sdl::Texture &spritesheet = getTexture("adventurer.png");
	const sdl::Animation idleAnimation{
	    spritesheet, {{0, 0, 50, 36}, {50, 0, 50, 36}, {100, 0, 50, 36}, {150, 0, 50, 36}}, 200ms};

	const sdl::Animation walkingAnimation{spritesheet,
	                                      {{50, 37, 50, 36},
	                                       {100, 37, 50, 36},
	                                       {150, 37, 50, 36},
	                                       {200, 37, 50, 36},
	                                       {250, 37, 50, 36},
	                                       {300, 37, 50, 36}},
	                                      150ms};

	const sdl::Animation airUpAnimation{spritesheet, {{100, 74, 50, 36}, {150, 74, 50, 36}, {200, 74, 50, 36}}, 100ms};
	const sdl::Animation airDownAnimation{spritesheet, {{50, 111, 50, 36}, {100, 111, 50, 36}}, 100ms};

	const sdl::Animation hurtAnimation{
	    spritesheet, {{150, 296, 50, 36}, {200, 296, 50, 36}, {250, 296, 50, 36}}, 100ms};
	const sdl::Animation deathAnimation{spritesheet,
	                                    {{300, 296, 50, 36},
	                                     {0, 333, 50, 36},
	                                     {50, 333, 50, 36},
	                                     {100, 333, 50, 36},
	                                     {150, 333, 50, 36},
	                                     {200, 333, 50, 36},
	                                     {250, 333, 50, 36}},
	                                    300ms};

	const sdl::Animation attackAnim1{spritesheet,
	                                 {{0, 222, 50, 36},
	                                  {50, 222, 50, 36},
	                                  {100, 222, 50, 36},
	                                  {150, 222, 50, 36},
	                                  {200, 222, 50, 36},
	                                  {250, 222, 50, 36},
	                                  {300, 222, 50, 36}},
	                                 50ms};

	const sdl::Animation attackAnim2{
	    spritesheet, {{0, 258, 50, 36}, {50, 258, 50, 36}, {100, 258, 50, 36}, {150, 258, 50, 36}}, 50ms};

	const sdl::Animation attackAnim3{spritesheet,
	                                 {{200, 258, 50, 36},
	                                  {250, 258, 50, 36},
	                                  {300, 258, 50, 36},
	                                  {0, 295, 50, 36},
	                                  {50, 295, 50, 36},
	                                  {100, 295, 50, 36}},
	                                 60ms};

	Attack attack1{{5, -20, 10, 20}, attackAnim1, 1, {2}};
	Attack attack2{{5, -20, 10, 20}, attackAnim2, 2, {1}};
	Attack attack3{{5, -20, 10, 20}, attackAnim3, 3, {2}};
	const std::vector<Attack> attacks{attack1, attack2, attack3};
	return std::make_unique<Player>(idleAnimation, walkingAnimation, airUpAnimation, airDownAnimation, deathAnimation,
	                                hurtAnimation, attacks);
}

std::unique_ptr<Mob> ResourceManager::makeMob(const game_definitions::Mob &mobdef) const
{
	// walking animation
	const sdl::Texture &walkingAnimationTexture = getTexture(mobdef.walkingAnimation.spritesheet);
	const sdl::Animation walkingAnimation{walkingAnimationTexture, mobdef.walkingAnimation.frames,
	                                      mobdef.walkingAnimation.timePerFrame};

	// death animation
	const sdl::Texture &deathAnimationTexture = getTexture(mobdef.deathAnimation.spritesheet);
	const sdl::Animation deathAnimation{deathAnimationTexture, mobdef.deathAnimation.frames,
	                                    mobdef.deathAnimation.timePerFrame};

	// hurt animation
	const sdl::Texture &hurtAnimationTexture = getTexture(mobdef.hurtAnimation.spritesheet);
	const sdl::Animation hurtAnimation{hurtAnimationTexture, mobdef.hurtAnimation.frames,
	                                   mobdef.hurtAnimation.timePerFrame};

	// idle animation
	const sdl::Texture &idleAnimationTexture = getTexture(mobdef.walkingAnimation.spritesheet);
	const sdl::Animation idleAnimation = {idleAnimationTexture, mobdef.idleAnimation.frames,
	                                      mobdef.idleAnimation.timePerFrame};

	// TODO handle behaviour
	std::shared_ptr<AI> ai;
	if (mobdef.behaviour == "patrolling")
		ai = patrollingAI;
	else if (mobdef.behaviour == "standing")
		ai = standingAI;
	else
		ai = idleAI;

	// attacks
	std::vector<game::Attack> attacks{};
	for (auto &adef : mobdef.attacks) {
		const auto &texture = getTexture(adef.animation.spritesheet);
		sdl::Animation animation{texture, adef.animation.frames, adef.animation.timePerFrame};

		attacks.emplace_back(adef.hitbox, animation, adef.damage, adef.damageFrames);
	}

	return std::make_unique<Mob>(mobdef.name, mobdef.health, mobdef.poise,
	                             std::chrono::milliseconds(mobdef.recoveryWindow), mobdef.speedPerSecond, mobdef.hitbox,
	                             mobdef.drawSize, walkingAnimation, deathAnimation, hurtAnimation, idleAnimation,
	                             attacks, ai);
}

std::unique_ptr<Item> ResourceManager::makeItem(const game_definitions::Item &itemdef) const
{
	const sdl::Texture &tex = getTexture(itemdef.animation.spritesheet);
	const sdl::Animation anim{tex, itemdef.animation.frames, itemdef.animation.timePerFrame};

	// TODO behaviour

	return std::make_unique<Item>(itemdef.name, itemdef.hitbox, itemdef.drawSize, anim);
}

std::unique_ptr<Room> ResourceManager::makeRoom(const game_definitions::Room &roomDef) const
{
	auto all_equal_size = [](auto vec) {
		return std::all_of(vec.cbegin(), vec.cend(), [&](auto el) { return el.size() == vec.cbegin()->size(); });
	};

	if (!all_equal_size(roomDef.layout))
		throw std::runtime_error("Unequal layers in room: " + roomDef.name);

	// transform layout definition to renderable layout
	Room::Layout layout;
	CollisionMap collisionMap;
	for (auto &layer : roomDef.layout) {
		bool firstLayer = layout.size() == 0;
		Room::Layer newLayer;
		// check equal row sizes
		if (!all_equal_size(layer))
			throw std::runtime_error("Unequal rows in room: " + roomDef.name);
		for (unsigned r = 0; r < layer.size(); r++) {
			auto &row = layer[r];
			if (firstLayer)
				collisionMap.emplace_back(row.size());
			auto &collRow = collisionMap[r];
			std::vector<Collision> collisionRow;
			Room::Row newRow;
			for (unsigned t = 0; t < row.size(); t++) {
				auto &tile = row[t];
				auto &collisionTile = collRow[t];
				collisionTile = std::max(tile.collision, collisionTile);
				sdl::Sprite sprite{getTexture(roomDef.tileset), tile.rectangle};
				Room::Tile newTile{sprite};
				newRow.emplace_back(newTile);
			}
			newLayer.emplace_back(newRow);
		}
		layout.emplace_back(newLayer);
	}

	// Add mobs
	std::vector<game::Mob> mobs;
	for (auto m : roomDef.mobs) {
		game::Mob mob = getMob(m.id);
		mob.movable.reposition(m.position);
		mobs.emplace_back(mob);
	}

	// Add items
	std::vector<game::Item> items;
	for (auto i : roomDef.items) {
		game::Item item = getItem(i.id);
		item.movable.reposition(i.position);
		items.emplace_back(item);
	}

	// Add doors
	std::vector<game::Door> doors;
	for (auto i : roomDef.doors) {
		game::Item item = getItem(i.itemId);
		item.movable.reposition(i.position);
		game::Door door{i.name, item, i.direction, i.targetRoom, i.targetDoorName};
		doors.emplace_back(door);
	}

	return std::make_unique<game::Room>(roomDef.name, getTexture(roomDef.background), getMusic(roomDef.music),
	                                    roomDef.location, roomDef.gatingArea, std::move(layout),
	                                    std::move(collisionMap), std::move(mobs), std::move(items), std::move(doors));
}

void ResourceManager::parseDefinitions(std::string definitionPath)
{
	std::cout << "Parsing game definitions from: " << definitionPath << std::endl;
	std::vector<fs::path> roomFiles, mobFiles, itemFiles;
	for (auto &f : fs::recursive_directory_iterator(definitionPath)) {
		if (!f.is_regular_file())
			continue;
		std::string ext{f.path().extension()};
		if (ext == ".mob")
			mobFiles.push_back(f);
		else if (ext == ".room")
			roomFiles.push_back(f);
		else if (ext == ".item")
			itemFiles.push_back(f);
	}

	for (auto &f : mobFiles) {
		parseDefinition(f);
	}
	for (auto &f : itemFiles) {
		parseDefinition(f);
	}
	for (auto &f : roomFiles) {
		parseDefinition(f);
	}
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

ResourceManager::ResourceManager(const std::string &path_to_definitions, const std::string &path_to_assets)
    : sdl(sdl::SDL::getInstance()), idleAI(std::make_shared<IdleAI>()), standingAI(std::make_shared<StandingAI>()),
      patrollingAI(std::make_shared<PatrollingAI>())
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

	parseDefinitions(path_to_definitions);
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
