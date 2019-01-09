#include "SDL.hpp"

sdl::SDL::SDL()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		throw SdlException("SDL could not initialize!");
	// Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		printf("Warning: Linear texture filtering not enabled!");

	renderer = std::make_unique<Renderer>();
	// Initialize PNG loading
	constexpr int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
		throw SdlException("SDL_image could not initialize!");

	if (TTF_Init() < 0)
		throw SdlException("SDL_ttf could not initialize!");

	int mixFlags = MIX_INIT_OGG | MIX_INIT_MOD | MIX_INIT_MP3;
	if (!(Mix_Init(mixFlags) & mixFlags))
		throw SdlException("SDL_mix could not initialize!");

	constexpr int stereo = 2;
	constexpr int chunksize = 1024;
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, stereo, chunksize))
		throw SdlException("Could not open audio device!");
}

sdl::SDL::~SDL()
{
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

sdl::SDL &sdl::SDL::getInstance()
{
	static SDL instance;
	return instance;
}

const sdl::Renderer &sdl::SDL::getRenderer()
{
	return *renderer;
}

void sdl::SDL::delay(std::chrono::milliseconds time)
{
	SDL_Delay((unsigned int)time.count());
}

std::unique_ptr<sdl::Texture> sdl::SDL::loadTexture(const std::string &path) const
{
	SDL_Surface *tempSurface = IMG_Load(path.c_str());
	if (tempSurface == nullptr)
		throw SdlException("Could not load texture '" + path + "': " + SDL_GetError());

	SDL_Texture *rawTexture = SDL_CreateTextureFromSurface(renderer->getRawRenderer(), tempSurface);
	SDL_FreeSurface(tempSurface); // Free *before* throwing exception

	if (rawTexture == nullptr)
		throw SdlException("Could not load texture '" + path + "': " + SDL_GetError());

	return std::make_unique<Texture>(rawTexture);
}

std::unique_ptr<sdl::Font> sdl::SDL::loadFont(const std::string &path, unsigned int size)
{
	TTF_Font *font = TTF_OpenFont(path.c_str(), size);
	if (font == nullptr)
		throw SdlException("Could not load font '" + path + "': " + TTF_GetError());

	return std::make_unique<sdl::Font>(font);
}

std::unique_ptr<sdl::SoundEffect> sdl::SDL::loadSound(const std::string &path) const
{
	// LoadWAV can load all supported formats. I know, I know, horrible name!
	Mix_Chunk *chunk = Mix_LoadWAV(path.c_str());
	if (chunk == nullptr)
		throw SdlException("Could not load sound effect '" + path + "': " + Mix_GetError());

	return std::make_unique<sdl::SoundEffect>(chunk);
}

std::unique_ptr<sdl::Music> sdl::SDL::loadMusic(const std::string &path) const
{
	Mix_Music *music = Mix_LoadMUS(path.c_str());
	if (music == nullptr)
		throw SdlException("Could not load music '" + path + "': " + Mix_GetError());

	return std::make_unique<sdl::Music>(music);
}

sdl::Text sdl::SDL::generateText(const Font &font, const std::string &text, Color color, TextRendering rendering,
                                 Color bgColor)
{
	SDL_Surface *tempSurface = nullptr;
	switch (rendering) {
	case TextRendering::Solid:
		tempSurface = TTF_RenderText_Solid(font.rawFont, text.c_str(), color);
		break;
	case TextRendering::Shaded:
		tempSurface = TTF_RenderText_Shaded(font.rawFont, text.c_str(), color, bgColor);
		break;
	case TextRendering::Blended:
		tempSurface = TTF_RenderText_Blended(font.rawFont, text.c_str(), color);
		break;
	}
	if (tempSurface == nullptr)
		throw SdlException("Could not draw text");

	SDL_Texture *rawTexture = SDL_CreateTextureFromSurface(renderer->getRawRenderer(), tempSurface);
	SDL_FreeSurface(tempSurface); // Free *before* throwing exception

	if (rawTexture == nullptr)
		throw SdlException("Could not create texture");

	// Store text texture in a sprite, including it's size
	Rectangle sourceRect = {0, 0, 0, 0};
	SDL_QueryTexture(rawTexture, nullptr, nullptr, &sourceRect.w, &sourceRect.h);
	return {rawTexture, sourceRect};
}
