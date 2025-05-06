#include "Utils.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

Utils::Utils()
: background(nullptr), music(nullptr), currentLineIndex(0), lastLineTime(0), lineTimer(800)
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

void Utils::loadImages(SDL_Renderer* renderer, const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        SDL_Log("Failed to load image %s: %s", path.c_str(), IMG_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (texture)
    {
        images.push_back(texture);
    }
}

void Utils::renderImages(SDL_Renderer* renderer, size_t index, SDL_Rect dst)
{
    if (showImage && index<images.size() && images[index])
    {
        SDL_RenderCopy(renderer, images[index], nullptr, &dst);
    }
}

void Utils::unloadImages()
{
    for(SDL_Texture* tex : images)
    {
        if (tex) SDL_DestroyTexture(tex);
    }
    images.clear();
}

void Utils::playBackgroundMusic(const std::string& path)
{
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

void Utils::playSound(const std::string& path)
{
    chunk = Mix_LoadWAV(path.c_str());
    if(chunk)
    {
        Mix_PlayChannel(-1,chunk,0);
    }
}

void Utils::loadTextboxFromFile(const std::string& filename)
{
    textboxLines.clear();
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
        textboxLines.push_back(line);
    }
    currentLineIndex = 0;
    linesToShow.clear();
    lastLineTime = SDL_GetTicks();
    lineTimer = 0;
    showTextbox = true;

    nextTextboxFrame();
}

void Utils::updateTextbox(Uint32 currentTime)
{
if (!showTextbox) return;

    if (currentBatchIndex >= currentBatch.size())
    {
        if (currentLineIndex < textboxLines.size())
        {
            nextTextboxFrame();
        }
        return;
    }

    if (currentTime - lastLineTime >= lineTimer)
    {
        linesToShow.push_back(currentBatch[currentBatchIndex]);
        currentBatchIndex++;
        lastLineTime = currentTime;
        lineTimer = 800;
    }
}

void Utils::renderTextbox(SDL_Renderer* renderer, TTF_Font* font)
 {
    if (!showTextbox) return;
    SDL_Rect textboxRect = { 50, 400, 700, 150 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &textboxRect);

    int y = 410;
    for (const std::string& line : linesToShow) {
        renderText(renderer, font, line, 60, y);
        y += 30;
    }
}

void Utils::nextTextboxFrame()
{
    linesToShow.clear();
    currentBatch.clear();
    currentBatchIndex = 0;

    int count = 0;
    while (currentLineIndex < textboxLines.size() && count <= 3)
    {
        currentBatch.push_back(textboxLines[currentLineIndex]);
        currentLineIndex++;
        count++;
    }

    lastLineTime = SDL_GetTicks();
}

bool Utils::textboxFinished()
{
    return currentLineIndex >= textboxLines.size() && currentBatchIndex >= currentBatch.size();
}

void Utils::resetTextbox()
 {
    textboxLines.clear();
    linesToShow.clear();
    currentLineIndex = 0;
    currentBatchIndex = 0;
    currentBatch.clear();
    showTextbox = false;
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
    unloadImages();
    stopMusic();
    textboxLines.clear();
    linesToShow.clear();
    currentLineIndex = 0;
    showBackground = false;
    showImage = false;
    playMusic = false;
    showTextbox = false;
}
