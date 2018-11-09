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

namespace game {

class ResourceManager final {
	friend void parse_definition(std::filesystem::path f, ResourceManager &res);

  public:
	/**
	 * @brief Load all definitions and assets
	 *
	 * @param path_to_definitions path to the game definitions folder
	 */
	ResourceManager(const std::string &path_to_definitions);

	Mob getMob(const std::string &name) const;
	Item getItem(const std::string &name) const;
	Room getRoom(const std::string &name) const;

  private:
	/**
	 * @brief Get a texture or the default if it doesn't exist.
	 *
	 * @param id of the texture - we use the relative path
	 *
	 * @return a const ref to the texture
	 */
	const Sdl::Texture &getTexture(const std::string &id) const;

	const std::unordered_map<std::unique_ptr<Mob>> mobs;
	const std::unordered_map<std::unique_ptr<Item>> items;
	const std::unordered_map<std::unique_ptr<Room>> rooms;
	const std::unordered_map<std::unique_ptr<Sdl::Texture>> textures;
};
} // namespace game

#endif /* RESOURCEMANAGER_H */
