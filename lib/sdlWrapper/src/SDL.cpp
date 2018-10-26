#include "SDL.hpp"

sdl::SDL::SDL() {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw SdlException("SDL could not initialize!");
	// Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		printf("Warning: Linear texture filtering not enabled!");

	renderer = std::make_shared<Renderer>();
	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
		throw SdlException("SDL_image could not initialize!");
}

sdl::SDL::~SDL() {
	IMG_Quit();
	SDL_Quit();
}

sdl::SDL &sdl::SDL::getInstance() {
	static SDL instance;
	return instance; //TODO: release resources?
}

std::shared_ptr<sdl::Renderer> sdl::SDL::getRenderer() { return renderer; }

void sdl::SDL::delay(std::chrono::milliseconds time) {
	SDL_Delay((unsigned int) time.count());
}

std::shared_ptr<sdl::Texture> sdl::SDL::loadTexture(std::string path) {
	SDL_Surface *tempSurface = IMG_Load(path.c_str());
	if (tempSurface == nullptr)
		throw SdlException("Could not load texture");

	SDL_Texture *rawTexture =
		SDL_CreateTextureFromSurface(renderer->getRawRenderer(), tempSurface);
	if (rawTexture == nullptr)
		throw SdlException("Could not create texture");

	SDL_FreeSurface(tempSurface);
	return std::make_shared<Texture>(rawTexture);
}
