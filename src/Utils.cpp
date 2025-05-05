#include "Utils.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

Utils::Utils()
: background(nullptr), image(nullptr), music(nullptr), currentLine(0)
{}

Utils::~Utils()
{
    cleanup();
}

void Utils::loadBackground(SDL_Renderer* renderer, const std::string& path)
{
    unloadBackground();
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface)
    {
        background = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void Utils::renderBackground(SDL_Renderer* renderer)
{
    if (showBackground && background)
    {
        SDL_RenderCopy(renderer, background, nullptr, nullptr);
    }
}

void Utils::unloadBackground()
{
    if (background)
    {
        SDL_DestroyTexture(background);
        background = nullptr;
    }
}

void Utils::loadImage(SDL_Renderer* renderer, const std::string& path)
{
    unloadImage();
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface)
    {
        image = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void Utils::renderImage(SDL_Renderer* renderer, SDL_Rect dst)
{
    if (showImage && image)
    {
        SDL_RenderCopy(renderer, image, nullptr, &dst);
    }
}

void Utils::unloadImage()
{
    if (image)
    {
        SDL_DestroyTexture(image);
        image = nullptr;
    }
}

void Utils::playBackgroundMusic(const std::string& path)
{
    stopMusic();
    music = Mix_LoadMUS(path.c_str());
    if (music)
    {
        Mix_PlayMusic(music, -1);
    }
}

void Utils::stopMusic()
{
    if (Mix_PlayingMusic())
    {
        Mix_HaltMusic();
    }
    if (music)
    {
        Mix_FreeMusic(music);
        music = nullptr;
    }
}

void Utils::loadTextboxFromFile(const std::string& path)
{
    textboxLines.clear();
    currentLine = 0;
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line))
    {
        textboxLines.push_back(line);
    }
}

void Utils::updateTextbox() {
    if (currentLine < textboxLines.size()) {
        currentLine++;
    }
}

void Utils::renderTextbox(SDL_Renderer* renderer, TTF_Font* font)
{
    if (!showTextbox || currentLine == 0 || currentLine > textboxLines.size()) return;

    int start = std::max(0, currentLine - 3);
    for (int i = start; i < currentLine; ++i)
    {
        renderText(renderer, font, textboxLines[i], 50, 400 + (i - start) * 40);
    }
}

bool Utils::textboxFinished()
{
    return currentLine >= textboxLines.size();
}

void Utils::resetTextbox()
{
    currentLine = 0;
}

void Utils::renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y)
{
    SDL_Color color = {255, 255, 255};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (surface)
    {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = {x, y, surface->w, surface->h};
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, texture, nullptr, &dst);
        SDL_DestroyTexture(texture);
    }
}

void Utils::cleanup()
{
    unloadBackground();
    unloadImage();
    stopMusic();
    textboxLines.clear();
    currentLine = 0;
    showBackground = false;
    showImage = false;
    playMusic = false;
    showTextbox = false;
}
