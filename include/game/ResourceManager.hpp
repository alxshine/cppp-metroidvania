#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

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

class ResourceManager final {
  public:
	/**
	 * @brief Load all definitions and assets
	 *
	 * @param path_to_definitions path to the game definitions folder
	 */
	ResourceManager(const std::string &path_to_definitions, const sdl::SDL &sdl);

	Mob getMob(const std::string &name) const;
	Item getItem(const std::string &name) const;
	Room getRoom(const std::string &name) const;

  private:
	/**
	 * @brief Get a texture, or try to load it, or return the default if it doesn't exist.
	 *
	 * @param id of the texture - we use the relative path
	 *
	 * @return a const ref to the texture
	 */
	const sdl::Texture &getOrLoadTexture(const std::string &id);

	// Factory methods

	std::unique_ptr<Mob> makeMob(const game_definitions::Mob &mobdef);
	std::unique_ptr<Item> makeItem(const game_definitions::Item &itemdef);
	std::unique_ptr<Room> makeRoom(const game_definitions::Room &roomdef);

	void parseDefinition(std::filesystem::path f);

	// Members

	const sdl::SDL &sdl;
	std::unordered_map<std::string, std::unique_ptr<Mob>> mobs;
	std::unordered_map<std::string, std::unique_ptr<Item>> items;
	std::unordered_map<std::string, std::unique_ptr<Room>> rooms;
	std::unordered_map<std::string, std::unique_ptr<sdl::Texture>> textures;
};
} // namespace game

#endif /* RESOURCEMANAGER_H */
