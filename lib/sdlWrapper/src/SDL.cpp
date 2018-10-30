#include "SDL.hpp"

sdl::SDL::SDL()
{
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

	if (TTF_Init() < 0)
		throw SdlException("SDL_ttf could not initialize!");
}

sdl::SDL::~SDL()
{
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

sdl::SDL &sdl::SDL::getInstance()
{
	static SDL instance;
	return instance; // TODO: release resources?
}

std::shared_ptr<sdl::Renderer> sdl::SDL::getRenderer()
{
	return renderer;
}

void sdl::SDL::delay(std::chrono::milliseconds time)
{
	SDL_Delay((unsigned int)time.count());
}

std::shared_ptr<sdl::Texture> sdl::SDL::loadTexture(std::string path)
{
	SDL_Surface *tempSurface = IMG_Load(path.c_str());
	if (tempSurface == nullptr)
		throw SdlException("Could not load texture");

	SDL_Texture *rawTexture = SDL_CreateTextureFromSurface(renderer->getRawRenderer(), tempSurface);
	if (rawTexture == nullptr)
		throw SdlException("Could not create texture");

	SDL_FreeSurface(tempSurface);
	return std::make_shared<Texture>(rawTexture);
}

std::shared_ptr<sdl::Font> sdl::SDL::loadFont(std::string path, unsigned int size)
{
	TTF_Font *font = TTF_OpenFont(path.c_str(), size);
	if (font == nullptr)
		throw SdlException("Could not load font '" + path + "':" + TTF_GetError());

	return std::make_shared<sdl::Font>(font);
}

sdl::Text sdl::SDL::generateText(std::shared_ptr<Font> font, std::string text, Color color, TextRendering rendering,
                                 Color bgColor)
{
	SDL_Surface *tempSurface = nullptr;
	switch (rendering) {
	case TextRendering::Solid:
		tempSurface = TTF_RenderText_Solid(font->rawFont, text.c_str(), {color.r, color.g, color.b, color.a});
		break;
	case TextRendering::Shaded:
		tempSurface = TTF_RenderText_Shaded(font->rawFont, text.c_str(), {color.r, color.g, color.b, color.a},
		                                    {bgColor.r, bgColor.g, bgColor.b, bgColor.a});
		break;
	case TextRendering::Blended:
		tempSurface = TTF_RenderText_Blended(font.get()->rawFont, text.c_str(), {color.r, color.g, color.b, color.a});
		break;
	}
	if (tempSurface == nullptr)
		throw SdlException("Could not draw text");

	SDL_Texture *rawTexture = SDL_CreateTextureFromSurface(renderer->getRawRenderer(), tempSurface);
	if (rawTexture == nullptr)
		throw SdlException("Could not create texture");

	SDL_FreeSurface(tempSurface);

	Rectangle sourceRect = {0, 0, 0, 0};
	SDL_QueryTexture(rawTexture, nullptr, nullptr, &sourceRect.w, &sourceRect.h);
	return {std::make_shared<Texture>(rawTexture), sourceRect};
}
