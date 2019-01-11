#include "state/SerializedState.hpp"
#include "gamedef/entity_definitions.hpp"

using namespace game;
using namespace game_definitions;

std::ostream &game::operator<<(std::ostream &out, SerializedState &state)
{
	return out << state.unlockedAreas << std::endl
	           << state.currentRoomName << std::endl
	           << state.playerState.position.x << " " << state.playerState.position.y << std::endl;
}
std::istream &game::operator>>(std::istream &in, SerializedState &state)
{
	in >> state.unlockedAreas;
	in >> state.currentRoomName;
	in >> state.playerState.position.x >> state.playerState.position.y;

	return in;
}
