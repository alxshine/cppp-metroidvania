#ifndef LOADALLDEFINITIONS_H
#define LOADALLDEFINITIONS_H

#include <string>

#include "EntityDefinitions.hpp"
#include "game/ResourceManager.hpp"

namespace game_definitions {
	void load_all_definitions(std::string base, game::ResourceManager &res);
}

#endif /* LOADALLDEFINITIONS_H */
