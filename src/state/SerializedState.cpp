#include "state/SerializedState.hpp"
#include "gamedef/entity_definitions.hpp"

using namespace game;
using namespace game_definitions;

std::ostream &game::operator<<(std::ostream &out, SerializedState &state)
{
	out << state.unlockedAreas << std::endl << state.currentRoomName << std::endl;

	for (auto s : state.visitedRooms) {
		out << s << " ";
	}
	out << "endlist" << std::endl;

	for (auto s : state.inventory) {
		out << s << " ";
	}
	out << "endlist" << std::endl;

	return out << state.playerState.position.x << " " << state.playerState.position.y << std::endl;
}
std::istream &game::operator>>(std::istream &in, SerializedState &state)
{
	in >> state.unlockedAreas;
	in >> state.currentRoomName;
	while (true) {
		std::string nextKeyword;
		in >> nextKeyword;
		if (nextKeyword == "endlist")
			break;

		state.visitedRooms.insert(nextKeyword);
	}
	while (true) {
		std::string nextKeyword;
		in >> nextKeyword;
		if (nextKeyword == "endlist")
			break;

		state.inventory.insert(nextKeyword);
	}
	in >> state.playerState.position.x >> state.playerState.position.y;

	return in;
}
