#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "SDL.hpp"
#include "game/Game.hpp"

using namespace std;
using namespace std::chrono_literals;
using namespace sdl;
using namespace game;

void printTimePoint(string name, const GameClock::time_point &t)
{
	cout << name + ": " << std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count() << endl;
}

int main()
{
	try {
		Game game("game_definitions", "assets", "First", {50, 180});
		game.initialize();
		game.runMainLoop();

	} catch (SdlException &e) {
		cerr << e.what() << endl;
	} catch (GameException &e) {
		cerr << e.what() << endl;
	} catch (const string &e) {
		cerr << e << endl;
	}
}
