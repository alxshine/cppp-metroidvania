#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "Item.hpp"
#include "Mob.hpp"
#include "Room.hpp"
#include "SDL.hpp"
#include "gamedef/DefinitionLoading.hpp"

namespace game {
template <typename T>
bool contained(std::vector<T> vec, T x);

class ResourceManager final {
  public:
	/**
	 * @brief Load all definitions and assets
	 *
	 * @param path_to_definitions path to the game definitions folder
	 */
	ResourceManager(const std::string &path_to_definitions, const std::string &path_to_assets);

	Mob getMob(const std::string &name) const;
	Item getItem(const std::string &name) const;
	Room getRoom(const std::string &name) const;
	const sdl::SoundEffect &getSound(const std::string &name) const;
	const sdl::Music &getMusic(const std::string &name) const;

  private:
	static std::vector<std::string> imageFormats;
	static std::vector<std::string> soundFormats;
	static std::vector<std::string> musicFormats;

	/**
	 * @brief Get a texture, or try to load it, or return the default if it doesn't exist.
	 *
	 * @param id of the texture - we use the relative path
	 *
	 * @return a const ref to the texture
	 */
	const sdl::Texture &getTexture(const std::string &id) const;

	void loadTexture(const std::string &id, const std::string &path);
	void loadSound(const std::string &id, const std::string &path);
	void loadMusic(const std::string &id, const std::string &path);

	// Factory methods

	std::unique_ptr<Mob> makeMob(const game_definitions::Mob &mobdef) const;
	std::unique_ptr<Item> makeItem(const game_definitions::Item &itemdef) const;
	std::unique_ptr<Room> makeRoom(const game_definitions::Room &roomdef) const;

	void parseDefinition(std::filesystem::path f);

	// Members

	const sdl::SDL &sdl;
	std::unordered_map<std::string, std::unique_ptr<Mob>> mobs;
	std::unordered_map<std::string, std::unique_ptr<Item>> items;
	std::unordered_map<std::string, std::unique_ptr<Room>> rooms;
	std::unordered_map<std::string, std::unique_ptr<sdl::Texture>> textures;
	std::unordered_map<std::string, std::unique_ptr<sdl::SoundEffect>> sounds;
	std::unordered_map<std::string, std::unique_ptr<sdl::Music>> music;
};
} // namespace game

#endif /* RESOURCEMANAGER_H */