#ifndef SERIALIZED_STATE_H
#define SERIALIZED_STATE_H

#include <string>
#include <set>
#include <iostream>

#include "game/constants.hpp"

namespace game {

struct SerializedState final {
	static const int compatibleVersion = 1;

	// GATING:
	// Spawn boss only if unlocked area is same as area containing room (avoids respawn)
	// Allow travel through doors only if next room is in area <= unlockedArea
	unsigned int unlockedAreas = 0;
	std::string currentRoomName;
	std::set<std::string> visitedRooms;
	std::set<std::string> inventory;

	struct PlayerState {
		Position position;
	} playerState;
};

std::ostream &operator<<(std::ostream &out, SerializedState &state);
std::istream &operator>>(std::istream &in, SerializedState &state);

} // namespace game

#endif /* SERIALIZED_STATE_H */
